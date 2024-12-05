// C++ code

#include <FastLED.h>
#define NUMPIXELS 23 // number of neopixels per strip
#define NUM_ROWS 5

CRGB leds[NUM_ROWS][NUMPIXELS]; // Create a 2D array for LEDs

int limit_switch_pins[5] = {8, 9, 10, 11, 12};

const int COLS = 5;
const int ROWS = 5;
int grid[ROWS][COLS]; // Grid to keep track of LED states

int turn = 0;
bool boardFull = false;
bool win = false;

// Color of LED for players
CRGB player1Color = CRGB(255, 0, 0);
CRGB player2Color = CRGB(0, 0, 255);

// Arrays to store winning 4-in-a-row
int winRows[4];
int winCols[4];

// Setup LEDs and grid
void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUMPIXELS);
  FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUMPIXELS);
  FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUMPIXELS);
  FastLED.addLeds<NEOPIXEL, 5>(leds[3], NUMPIXELS);
  FastLED.addLeds<NEOPIXEL, 6>(leds[4], NUMPIXELS);

  //set power limit
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  //clear leds
  FastLED.clear();
  FastLED.show();

  Serial.begin(9600);
  initializeGrid();
}

// Game loop
void loop() {
  if (!boardFull && !win) {
    int move = getPlayerMove();
    int player = (turn % 2 == 0) ? 1 : 2;
    
    makeMove(player, move);
    checkForWin(player, move);
    checkForTie();
    
    printGrid();
    turn++;
    delay(500);
  }
  else {
    endGame();
    resetGame();
  }
}

// Set up grid
void initializeGrid() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      grid[i][j] = 0;
    }
  }
}

// Read in a move from the player (via buttons)
int getPlayerMove() {
  int move;
  int i = 0;
  
  while (true) {
    //button to skip a players turn manually (when they miss a column)
    if(digitalRead(13) == LOW){
      while(true){
        if(digitalRead(13) == HIGH){
          turn++;
          Serial.println("turn swapped?");
          break;
        }
      }
    }

    //waits until a limit switch is triggered
    if (digitalRead(i + limit_switch_pins[0]) == HIGH && grid[0][i] == 0) {
      break;
    }
    i++;
    if (i == COLS) {
      i = 0;
    }
  }
  
  move = i;
  
  return move;
}

// Update the grid according to a player's move
void makeMove(int player, int move) {
  for (int row = ROWS - 1; row >= 0; row--) {
    if (grid[row][move] == 0) {
      // Update grid
      grid[row][move] = player;
      
      // Light up the corresponding LED
      leds[row][2 + move*5] = (player == 1) ? player1Color : player2Color;
      FastLED.show();
      
      break;
    }
  }
}

// Check for win conditions
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

// Check if the board is full (can lead to a tie)
void checkForTie() {
   boardFull = true;
   for (int c = 0; c < COLS; c++) {
     if (grid[0][c] == 0) {
       boardFull = false;
       break;
     }
   }
}

// Print grid state for debugging / basic UI
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

// End the game
void endGame() {
  if (boardFull && !win) {
    Serial.println("Tie!");
  } 
  else {
    Serial.print("Player ");
    Serial.print((turn + 1) % 2 + 1);
    Serial.println(" wins!");
  
    // Flash winning LEDs 
    for(int repeat = 0; repeat < 20; repeat++){
      for (int i = 0; i < 4; i++) {
        //change the 4 led colors to white
        leds[winRows[i]][2 + 5*winCols[i]] = CRGB(255, 255, 255);
        FastLED.show();
      }
      delay(100);

      for (int i = 0; i < 4; i++) {
        //change 4 leds back to red or yellow 
        leds[winRows[i]][2 + 5*winCols[i]] = ((turn % 2) == 1) ? player1Color : player2Color;
        FastLED.show();
      }

      delay(100);
    }
  }
}

// Method to reset the LEDs and game variables
void resetGame() {
   initializeGrid();
   fill_solid(&leds[0][0], NUM_ROWS * NUMPIXELS, CRGB::Black);
   FastLED.show();
   
   boardFull = false;
   win = false;
   turn = 0;
}
