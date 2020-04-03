//Tetris Field LED wrap Test
//WS2812b led string

#include <FastLED.h>

#define ledPin 7
#define width 4
#define height 4

const int numLed=width*height;
int boardArray[width*height]; //Standard Tetris field is a 10x20 column
int counter=0;
int fauxCounter=0;
int x=0;

CRGB leds[numLed];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLed);
  Serial.println("Begin LED Test");
}

void loop() {
  static unsigned long timer=millis();
  if(millis()-timer>800) {
    updateBoard();
    serialPrintGame();
    timer=millis();
    leds[x]=CRGB(0,0,0);
    x=zigzagUpdate();
    leds[x]=CRGB(128,0,0);
    FastLED.show();
  }
}

void serialPrintGame() {
  //Prints game field to serial output, for troubleshooting and development
  for(int i=0; i<(sizeof(boardArray)/sizeof(boardArray[0])); i++) {
    Serial.print("[");
    Serial.print(boardArray[i]);
    Serial.print("]");
    if(i%width==width-1) {
      Serial.println("");
    }
  }
  Serial.println("");
}

void updateBoard() {
  //Updates location of lit pixel on Serial output
  if(counter>0) {
    boardArray[counter-1]=0;
  }
  else if(counter==0) {
    boardArray[15]=0;
  }
  boardArray[counter]=1;
}

int zigzagUpdate() {
  //Adjusts counter value in order to properly display correct pixel
  Serial.print("Counter is ");
  Serial.println(counter);
  int row=counter/width;
  int column=counter-(row*width);
  if(row%2==1) {
    //If the row is odd, need to shift
    Serial.println("Odd- shift");
    if(column%width==0) {
      //sets fauxCounter value to one row ahead
      fauxCounter=counter+(width-1);
    }
    else {
      //iterates fauxCounter value down
      fauxCounter--;
    }
  }
  else {
    //Even row, direct counter value
    fauxCounter=counter;
  }
  counter++;
  if(counter>=(height*width)) {
    //reset counter when at end of array
    counter=0;
  }
  Serial.println(fauxCounter);
  return fauxCounter;
}
