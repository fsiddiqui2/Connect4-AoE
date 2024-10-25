//Group members: Navkarn Daroch, Immanual, Mosum
// Enable critical libraries
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

// Set parameters of OLED board
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Address and Variables for Accelerometer
int ADXL345 = 0x53;         // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
float X_max, Y_max, Z_max;  // Store maximum values

// Set any other additional items on your own...

// Set magnet, RGB LED, and start and finish time, and average and scale speed
int Sensor = 6;    // Set Sensor Pin to digital pin 2
int BlueLED = 7;   // Set blue LED color to digital pin 8
int RedLED = 8;    // Set red LED color to digital pin 10
int GreenLED = 9;  // Set red LED color to digital pin 7
int State = 0;     // Set variable for storing state of the hall sensor
int X = 0;         // Set variable for recording

double startmil = 0;   // Record low time in milliseconds
double startsec = 0;   // Record low time in seconds
double finishmil = 0;  // Record high time in milliseconds
double finishsec = 0;  // Record high time in seconds
double Average = 0;   //Defines/records average speed
double Scale = 0;     //Defines/records scale speed

// Setup Phase
// Setup your inputs and outputs...
void setup() {
  Serial.begin(9600);
  pinMode(Sensor, INPUT);     //Indicates that the sensor detects the magnet and records/reads data    
  pinMode(BlueLED, OUTPUT);   //All three LEDS changed to be output, allowing the LEDs to send a voltage and become lit. 
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
    Serial.println("Nope");      //Done by the professor while troubleshooting the code 
  }
  
  display.clearDisplay();
  delay(500);  // Pause for 0.5 seconds

  // Setup Accelerometer...
  Wire.begin();  // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345);  // Start communicating with the device
  Wire.write(0x2D);                 // Access/talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8);  // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
  Wire.endTransmission();
  delay(10);

  // Calibration notice to user, saying to put board on flat surface. Give user 3-5 seconds...
  ///

  // Calibration Begins, offset it
  // X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);
  // Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);
  // Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);
  display.setCursor(1, 45);
  display.print("0");
  display.display();
  delay(1000);

  ///Sets up OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.clearDisplay();
  display.setCursor(15, 10);                             //Defines location of text on OLED display
  display.print("Calibration is");                       //Defines what is to be printed on the display
  display.setCursor(15, 40);
  display.print("in Progress");
  display.display();                                     //Allows for the text to be displayed
  delay(2000);                                           //Creates a pocket of time between this text and the folllowing text, allows for this text to remain on the display for 2 seconds


  // already displays initial x,y,z

  // Inform user that calibration is complete
  ////////
  display.clearDisplay();
  display.setCursor(1, 1);
  display.print("Calibration Complete");
  display.setCursor(1, 15);
  display.print("X_Acceleration = ");
  display.setCursor(90, 15);
  display.print(X_out);
  display.setCursor(1, 30);
  display.print("Y_Acceleration = ");
  display.setCursor(90, 30);
  display.print(Y_out);
  display.setCursor(1, 45);
  display.print("Z_Acceleration = ");
  display.setCursor(90, 45);
  display.print(Z_out);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Inform user that car is ready to race...

  display.setCursor(15, 10);
  display.print("Ready to Race");
  display.display();
  display.clearDisplay();
}

/*
  // Loop Phase
  void loop() {
  // Now for the tricky part. Use what you learned from the Arduino lessons to code this.
  // Speak with instructor about any concerns, refer to the project document for details of tasks
  // To calculate the average and scale speed of your car...
  // Average = ( ( 433 / Elapsed Time ) * 0.05682 )
  // Scaled = Average * 25
  // The above equations will need to be altered to be compatible with the code, but should help you start
  // Good luck!
  }


  //OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
*/

void loop() {
  State = digitalRead(Sensor);   //Defines sensor
  Serial.println(State);                           

  if ((State == LOW) && X == 0) {          ///Reading magnet 1(Sensor previosuly read HIGH state, indicating that a magnet had not been detected. The sensor now reads LOW state, magnet 1 read)
    display.clearDisplay();
    display.display();
    digitalWrite(GreenLED, HIGH);          ///Green LED on while other two are off
    digitalWrite(BlueLED, LOW);
    digitalWrite(RedLED, LOW);
    startmil = millis();
    startsec = startmil / 1000;
    Serial.print("Start Time: ");
    Serial.print(startsec);
    Serial.println(" seconds.");            ///Begins to record time 
    display.setCursor(15, 10);
    display.print("Magnet 1");              ///Displays message (Magnet 1)
    display.display();
    delay(100);
    X = 1;
  }

  else if ((State == LOW) && (X == 1)) {      ///Reading magnet two(Sensor previosuly read HIGH state, indicating that a magnet had not been detected. The sensor now reads LOW state, magnet 2 read)
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, HIGH);
    digitalWrite(RedLED, LOW);                ///Blue LED on while other two are off
    display.setCursor(15, 10);
    display.print("Magnet 2");                ///Displays message (Magnet 2)
    display.display();
    delay(100);
    X = 2;
  }

  else if ((State == LOW) && X == 2) {          ///Reading magnet 3(Sensor previosuly read HIGH state, indicating that a magnet had not been detected. The sensor now reads LOW state, magnet 3 read)
    digitalWrite(GreenLED, LOW);
    digitalWrite(BlueLED, LOW);
    digitalWrite(RedLED, HIGH);                 ///Blue LED on while other two are off
    finishmil = millis();
    finishsec = finishmil / 1000;
    Serial.print("End Time: ");
    Serial.print(finishsec);
    Serial.println(" seconds.");                ///Stops recording time (records finish/elapsed time)
    display.setCursor(15, 10);
    display.print("Magnet 3");                  ///Displays message (Magnet 3)
    delay(100);
    display.display();
    X = 3;
  }

  if (X == 3) {                                     ///Occuring after reading magnet 3, loops until sensor detects magnet 4
    State = digitalRead(Sensor);

    if ((State == HIGH)) {                          ///State = HIGH indicates that no magnet has been read/no magnet nearby
      display.clearDisplay();

      display.setCursor(1, 30);
      display.print("X_Acceleration = ");
      display.setCursor(90, 30);
      display.print(X_max);                         ///Displays the maximum X-axis acceleration recorded
      display.display();
      delay(1000);

      display.clearDisplay();
      display.setCursor(1, 30);
      display.print("Y_Acceleration = ");           
      display.setCursor(90, 30);
      display.print(Y_max);                        ///Displays the maximum Y-axis acceleration recorded
      display.display();
      delay(1000);

      display.clearDisplay();
      display.setCursor(1, 30);
      display.print("Z_Acceleration = ");
      display.setCursor(90, 30);
      display.print(Z_max);                        ///Displays the maximum Z-axis acceleration recorded
      display.display();
      delay(1000);
      display.clearDisplay();

      Average = ((433 / (finishsec - startsec)) * 0.05682);                ///Defines the formula for the average speed
      //Average = (Wire.read() | Wire.read() << 8);
      display.setCursor(1, 30);
      display.print("Average = ");
      display.print(Average);                                              ///Displays the calculated average speed
      display.display();
      delay(1000);
      display.clearDisplay();

      Scale = (Average * 25);                                              ///Defines the formula for the scale speed
      //Scale = (Wire.read() | Wire.read() << 8);
      display.setCursor(1, 30);
      display.print("Scale = ");
      display.print(Scale);                                                ///Displays the calculated scale speed
      display.display();
      delay(1000);
      display.clearDisplay();

      display.setCursor(1, 30);
      display.print("finishsec = ");
      display.setCursor(90, 30);
      display.print(finishsec);                                             ///Displays the elapsed time(represnted by finishsec)
      display.display();
      delay(1000);
      display.clearDisplay();
    }
    if ((State == LOW)) {                                       ///Reading magnet 4
      digitalWrite(GreenLED, LOW);
      digitalWrite(BlueLED, LOW);
      digitalWrite(RedLED, LOW);
      display.setCursor(15, 10);
      display.print("Resetting...");
      display.display();
      delay(2000);
      display.print("Ready to Race");                           ///Vehicle resets(code loops)
      display.display();
      X = 0;
    }
  }



  //Accelerometer
  if ((X > 1) && (X < 3)) {
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32);  // Start with register 0x32 (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    // Read 6 registers total, each axis value is stored in 2 registers
    Wire.requestFrom(ADXL345, 6, true);
    // Record the X, Y, and Z values
    X_out = (Wire.read() | Wire.read() << 8);  // X-axis value
    X_out = X_out / 256;                       //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
    Y_out = (Wire.read() | Wire.read() << 8);  // Y-axis value
    Y_out = Y_out / 256;
    Z_out = (Wire.read() | Wire.read() << 8);  // Z-axis value
    Z_out = Z_out / 256;
    if (X_out > X_max) {                  //If the X-axis acceleration read proves greater than that of the recorded maximum X-axis acceleration, then the X-axis acceleration which was read becomes the maximum X-axis acceleration
      X_max = X_out;
    }
    if (Y_out > Y_max) {                  //If the X-axis acceleration read proves greater than that of the recorded maximum X-axis acceleration, then the X-axis acceleration which was read becomes the maximum X-axis acceleration
      Y_max = Y_out;
    }
    if (Z_out > Z_max) {                  //If the X-axis acceleration read proves greater than that of the recorded maximum X-axis acceleration, then the X-axis acceleration which was read becomes the maximum X-axis acceleration
      Z_max = Z_out;
    }
  }
}
