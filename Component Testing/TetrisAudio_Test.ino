//DFPlayer Mini Test for Tetris Project
//Joshua Curfman
//Learning how to use the DFRobot DFPlayer Mini with the Arduino Uno
//For further information, look to:
//https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial softSerial(10,11); //RX, TX
DFRobotDFPlayerMini tetrisAudio;
void printDetail(uint8_t type, int value);

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  Serial.println("");
  Serial.println("Initializing DFPlayer...");

  if(!tetrisAudio.begin(softSerial)) {
    Serial.println("Unable to begin:");
    while(true);
  }
  Serial.println("DFPlayer Mini initialized");

  tetrisAudio.volume(20); //Set volume value
  //tetrisAudio.play(1); //play first mp3
  tetrisAudio.loop(1); //loop first mp3 for background soundtrack
}

void loop() {
  static unsigned long timer=millis();
  if(tetrisAudio.available()) {
    printDetail(tetrisAudio.readType(), tetrisAudio.read());
  }
  if(millis()-timer>5000) {
    timer=millis();
    tetrisAudio.advertise(2); //sound effects
  }
}

void printDetail(uint8_t type, int value) {
  //Aids in troubleshooting. 
  switch(type) {
    case TimeOut:
      Serial.println("Time Out!");
      break;
    case WrongStack:
      Serial.println("Stack Wrong");
      break;
    case DFPlayerCardInserted:
      Serial.println("Card Inserted");
      break;
    case DFPlayerCardRemoved:
      Serial.println("Card Removed");
      break;
    case DFPlayerCardOnline:
      Serial.println("Card Online");
      break;
    case DFPlayerPlayFinished:
      Serial.print("Number: ");
      Serial.print(value);
      Serial.println("Play Finished");
      break;
    case DFPlayerError:
      Serial.print("DFPlayerError: ");
      switch(value) {
        case Busy:
          Serial.println("Card not found");
          break;
        case Sleeping:
          Serial.println("Sleeping");
          break;
        case SerialWrongStack:
          Serial.println("Get wrong stack");
          break;
        case CheckSumNotMatch:
          Serial.println("Check Sum Not Match");
          break;
        case FileIndexOut:
          Serial.println("File Index out of bound");
          break;
        case FileMismatch:
          Serial.println("Cannot find file");
          break;
        case Advertise:
          Serial.println("In Ad");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
