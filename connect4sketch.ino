/*
  This code simulates connect 4 on an arduino using buttons as inputs
  Link to the arduino simulation: https://www.tinkercad.com/things/gFf8MpUx4rc-connect4aoe-final
  This code is for a class project
*/

#include <Adafruit_NeoPixel.h>
#define NUMPIXELS      6 // number of neopixels in strip
Adafruit_NeoPixel row1 = Adafruit_NeoPixel(NUMPIXELS, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row2 = Adafruit_NeoPixel(NUMPIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row3 = Adafruit_NeoPixel(NUMPIXELS, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row4 = Adafruit_NeoPixel(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel row5 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel led_rows[5] = {row1, row2, row3, row4, row5};
int button_pins[5] = {8, 9, 10, 11, 12};

const int COLS = 5;
const int ROWS = 5;
int grid[ROWS][COLS]; //grid to keep track of led states

int turn = 0;
bool boardFull = false;
bool win = false;

//color of led for players
int player1RGB[3] = {255, 0, 0};
int player2RGB[3] = {0, 0, 255};

//arrays to store winning 4-in-a-row
int winRows[4];
int winCols[4];

//setup leds and grid
void setup() {
  for(int i=0; i<5;i++){
    led_rows[i].begin();
  }
  Serial.begin(9600);
  initializeGrid();
}

//game loop
void loop() {
  if (!boardFull && !win) {
    int player = (turn % 2 == 0) ? 1 : 2;
    int move = getPlayerMove();
    
    makeMove(player, move);
    checkForWin(player, move);
    checkForTie();
    
    printGrid();
    turn++;
    delay(1000);
  }
  else {
    endGame();
    resetGame();
  }
}


//set up grid
void initializeGrid() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      grid[i][j] = 0;
    }
  }
  printGrid();
}

//read in a move from the player (via buttons)
int getPlayerMove() {
  int move;
  int i = 0;
  while(true){
    if(digitalRead(i+8) == HIGH && grid[0][i] == 0){
      break;
    }
    i++;
    if(i==5){
      i=0;
    }
  }
  move = i;
  return move;
}


//update the grid according to a player's move
void makeMove(int player, int move) {
  for (int row = ROWS - 1; row >= 0; row--) {
    if (grid[row][move] == 0) {
      //update grid
      grid[row][move] = player;
      
      //light up the corresponding led
      if(player==1){
        led_rows[row].setPixelColor(move, led_rows[row].Color(player1RGB[0], player1RGB[1], player1RGB[2]));
      }
      else{
        led_rows[row].setPixelColor(move, led_rows[row].Color(player2RGB[0], player2RGB[1], player2RGB[2]));
      }
      led_rows[row].show();
      
      break;
    }
  }
}

void checkForWin(int player, int move) {
  // Check horizontal
  for (int c = 0; c <= COLS - 4; c++) {
    for (int r = 0; r < ROWS; r++) {
      if (grid[r][c] == player && grid[r][c+1] == player && 
          grid[r][c+2] == player && grid[r][c+3] == player) {
        win = true;
        for (int i = 0; i < 4; i++) {
          winRows[i] = r;
          winCols[i] = c + i;
        }
        return;
      }
    }
  }

  // Check vertical
  for (int c = 0; c < COLS; c++) {
    for (int r = 0; r <= ROWS - 4; r++) {
      if (grid[r][c] == player && grid[r+1][c] == player && 
          grid[r+2][c] == player && grid[r+3][c] == player) {
        win = true;
        for (int i = 0; i < 4; i++) {
          winRows[i] = r + i;
          winCols[i] = c;
        }
        return;
      }
    }
  }

  // Check diagonal (top-left to bottom-right)
  for (int c = 0; c <= COLS - 4; c++) {
    for (int r = 0; r <= ROWS - 4; r++) {
      if (grid[r][c] == player && grid[r+1][c+1] == player && 
          grid[r+2][c+2] == player && grid[r+3][c+3] == player) {
        win = true;
        for (int i = 0; i < 4; i++) {
          winRows[i] = r + i;
          winCols[i] = c + i;
        }
        return;
      }
    }
  }

  // Check diagonal (bottom-left to top-right)
  for (int c = 0; c <= COLS - 4; c++) {
    for (int r = 3; r < ROWS; r++) {
      if (grid[r][c] == player && grid[r-1][c+1] == player && 
          grid[r-2][c+2] == player && grid[r-3][c+3] == player) {
        win = true;
        for (int i = 0; i < 4; i++) {
          winRows[i] = r - i;
          winCols[i] = c + i;
        }
        return;
      }
    }
  }
}


//check if the board is full (can lead to a tie)
void checkForTie() {
  boardFull = true;
  for (int c = 0; c < COLS; c++) {
    if (grid[0][c] == 0) {
      boardFull = false;
      break;
    }
  }
}

//print grid state for debugging / basic UI
void printGrid() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      Serial.print(grid[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

//end the game
void endGame() {
  if (boardFull && !win) {
    Serial.println("Tie!");
  } 
  else {
    Serial.print("Player ");
    Serial.print((turn + 1) % 2 + 1);
    Serial.println(" wins!");
    
    for (int i = 0; i < 4; i++) {
      grid[winRows[i]][winCols[i]] = 9; // Use 9 to represent winning positions
    }
    printGrid();
    
	//flashes the winning 4-in-row
    for(int repeat = 0; repeat < 20; repeat++){
      for (int i = 0; i < 4; i++) {
        //change the 4 led colors to white
        led_rows[winRows[i]].setPixelColor(winCols[i], led_rows[winRows[i]].Color(100, 100, 100));
        led_rows[winRows[i]].show();
      }
      
      delay(100);
      
      for (int i = 0; i < 4; i++) {
        //change 4 leds back to red or yellow 
        if((turn % 2) == 1){
      		led_rows[winRows[i]].setPixelColor(winCols[i], led_rows[winRows[i]].Color(player1RGB[0], player1RGB[1], player1RGB[2]));
        }
        else{
          led_rows[winRows[i]].setPixelColor(winCols[i], led_rows[winRows[i]].Color(player2RGB[0], player2RGB[1], player2RGB[2]));
        }
        led_rows[winRows[i]].show();
      }
      
      delay(100);
    }
    
  }
}

//method to reset the leds and game variables
void resetGame() {
  initializeGrid();
  for(int row = 0; row < 5; row++){
    led_rows[row].clear();
    led_rows[row].show();
  }
  boardFull = false;
  win = false;
  turn = 0;
}
