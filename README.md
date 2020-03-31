# Tetris Project
This is a personal project I'm currently working on. My goal is to recreate a playable version of the game Tetris from scratch on the Arduino platform. In the long term, I aim to have a custom LED display and button controls that this will work with. In the short term, however, the focus is simply on accurately implementing the game mechanics. 

The random bag test sketch is a standalone psuedo-random sequence generator, attempting to create a random but non-repeating order of the numbers 1 through 7 in order to approximate the tetromino generation algorithm actually used by Tetris games. This has now been integrated into the main Tetris script. 

The audio test sketch is a standalone investigation into the potential use of sound effects and backing music synced to the onscreen game. This is not likely to be integrated into the main script until further into development, and sound effects will not be provided in this repository.
