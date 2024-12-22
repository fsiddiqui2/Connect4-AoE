# Connect4-AoE

## Description
This project was done as a final project for the Art of Engineering course at Columbia University.
The code was used to create a physical Connect-4 game with an arduino. This Connect-4 differs from others in that no coins are involved. Instead, the players throw a light ball into one of five columns, which triggers a limit switch before returning to the player. The code then lights up the corresponsing LED, and also keeps track of the game state with a 2D array. Checks are done for a win after each move and logic for a tied game is also included. An extra button allows a players turn to be skipped, if for example they miss their throw. 

The code originally used an Adafruit library to work with tinkercad's online simulator, but was switched to the FastLED library to work with the actual addressaable LEDs our team had. The code implemntation also makes it easy to scale the size of the board and is  playable from the terminal interface.

### Compiling c++ source code
`g++ -std=c++11 connect4.cpp -o connect4C`

### Running c++ executable
`./connect4C`
