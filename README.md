# Tetris Project
This is a personal project I'm currently working on. My goal is to recreate a playable version of the game Tetris from scratch on the Arduino platform. In the long term, I aim to have a custom LED display and button controls that this will work with. In the short term, however, the focus is simply on accurately implementing the game mechanics. 

## Main Scripts
There are two main scripts here currently. The second of these is a partial rewrite to use an object class for the active falling block for streamlining elsewhere, and is likely to be the main script going forward.

## Component Testing
This File is composed of sketches and scripts developed for familiarization or testing purposes before integration into the main script.

#### RandBag_Test2
This sketch is a standalone psuedo-random sequence generator, attempting to create a random but non-repeating order of the numbers 1 through 7 in order to approximate the tetromino generation algorithm actually used by Tetris games. This has now been integrated into the main Tetris script.

#### TetrisAudio_Test
This audio test sketch is a standalone investigation into the potential use of sound effects and backing music synced to the onscreen game using a DFPlayer Mini board connected to the main microcontroller via serial port. This is not likely to be integrated into the main script until further into development, and sound effects will not be provided in this repository.

#### WS2812b_LED_Test
This sketch has little to do with the main scripts, and was largely written in order to familiarize myself with these particular LED's and the FastLED Arduino library. It is nevertheless here for posterity.

#### Tetris_LED_Wrap_Test
In the design of the LED Pixel grid, the even rows are wired in reverse from the array logic of the base Tetris code. This sketch was designed in order to remedy this problem in software.

## Resources
[Tetris Guideline](https://tetris.wiki/Tetris_Guideline) -These guidelines are to be used for inspiration and troubleshooting of the base mechanics of this project.
