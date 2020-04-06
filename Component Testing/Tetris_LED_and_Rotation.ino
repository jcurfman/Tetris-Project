//Tetris Test Code
//Rotation and WS2812b LED usage
//Written by Joshua Curfman as a personal project

class Block {
  int blockChoice; //The shape of the block, which acts as an identifier visually
  int ActiveIndices[4]; //The indices of the active block within the main array
  int rotation; //The rotation state of the block

  //Constructor
  public: Block() {
    rotation=1;
  }

  //Data entry functions
  void setBlockChoice(int blockType) {
    //Sets block type, a value between 1 and 7 denoting one of the 7 tetromino shapes
    blockChoice=blockType;
  }
  void addPosition(int index, int value) {
    //Adds the positional data for the active block, called by index of internal array
    //Should be stored as lowermost first, then leftmost first
    ActiveIndices[index]=value;
  }
  void rotate() {
    //rotation state is an integer between 1 and 4, with 1 being the default
    //moves clockwise
    if(rotation<4) {
      rotation++;
    }
    else if(rotation==4) {
      rotation=1;
    }
  }
  void resetRotate() {
    //to be called when creating new block
    rotation=1;
  }

  //Data return functions
  int getPosition(int index) {
    //returns coordinate of index
    return ActiveIndices[index];
  }
  int getBlockChoice() {
    //self explanatory
    return blockChoice;
  }
  int getRotation() {
    //returns rotation state
    return rotation;
  }
};
//BLOCK CLASS ENDS HERE-----------------------------------------------
#include <FastLED.h>

//field size definition
#define width 4
#define height 4

int boardArray[width*height]; //field for testing
int RandBag[7]; //Random generated sequence drawbag
int rgb[3]; //Color selection

int bagLeft=0;
Block activeBlock;

#define resetPin 12 //jumper from pin 12 to RESET pin
#define ledPin 7
#define rotateButton 2 //rotation button input
#define newBlockButton 3 //summon new block input

const int numLed=width*height;
int fauxCounter=0;

const int debounceDelay(50);
//Changing variables for each button for debouncing
volatile unsigned long lastRotate_micros;
volatile unsigned long lastNew_micros;

CRGB leds[numLed];

void setup() {
  //Startup sequence. Begins serial output, prints an empty field, generates first block
  Serial.begin(9600);
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLed);
  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, LOW);
  pinMode(rotateButton, INPUT_PULLUP);
  pinMode(newBlockButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rotateButton), debounceRotate, RISING);
  attachInterrupt(digitalPinToInterrupt(newBlockButton), debounceNew, RISING);
  //Uno has two external interrupts- all that's necessary here
  randomSeed(analogRead(A0)); //unconnected to anything, aids initial randomization

  Serial.println("");
  Serial.println("Begin Tetris Test- LED and Rotation");
  serialPrintGame();
  delay(2000);
}

void LEDloop() {
  //Print next tetromino after two seconds. LED Testing
  static unsigned long timer=millis();
  if(millis()-timer>2000) {
    newBlock();
    
    serialPrintGame();
    ledUpdate();
    timer=millis();
  }
}

void loop() {
  //Rotation Testing. Serial print only when necessary in debounce
  ledUpdate();
}

void ledUpdate() {
  //Uncomment the serial print sections for block by block readout if troubleshooting
  for(int i=0; i<(width*height); i++) {
    blockColor(boardArray[i]);
    int ledPosition=zigzagUpdate(i);
    if(boardArray[i]==0) {
      leds[ledPosition]=CRGB(rgb[0],rgb[1],rgb[2]);
      /*Serial.print("Empty @ ");
      Serial.print(i);
      Serial.print(", ");
      Serial.println(ledPosition);*/
    }
    else {
      leds[ledPosition]=CRGB(rgb[0],rgb[1],rgb[2]);
      /*Serial.print("Block @ ");
      Serial.print(i);
      Serial.print(", ");
      Serial.println(ledPosition);*/
    }
  }
  FastLED.show();
  delay(50);
}

void blockColor(int type) {
  //uses the block type in order to set the rgb values
  if(type==1) {
    rgb[0]=40;
    rgb[1]=100;
    rgb[2]=255;
  }
  else if(type==2) {
    rgb[0]=158;
    rgb[1]=158;
    rgb[2]=0;
  }
  else if(type==3) {
    rgb[0]=128;
    rgb[1]=0;
    rgb[2]=156;
  }
  else if(type==4) {
    rgb[0]=0;
    rgb[1]=255;
    rgb[2]=0;
  }
  else if(type==5) {
    rgb[0]=255;
    rgb[1]=0;
    rgb[2]=0;
  }
  else if(type==6) {
    rgb[0]=0;
    rgb[1]=0;
    rgb[2]=255;
  }
  else if(type==7) {
    rgb[0]=255;
    rgb[1]=140;
    rgb[2]=0;
  }
  else if(type==0) {
    rgb[0]=0;
    rgb[1]=0;
    rgb[2]=0;
  }
  
  //Comment out for full brightness
  for(int i=0; i<3; i++) {
    int temp=rgb[i];
    rgb[i]=temp/2;
  }
}

void serialPrintGame() {
  //Prints 4x4 field to serial output, for troubleshooting and development
  Serial.println("");
  for(int i=0; i<(sizeof(boardArray)/sizeof(boardArray[0])); i++) {
    Serial.print("[");
    Serial.print(boardArray[i]);
    Serial.print("]");
    if(i%width==(width-1)) {
      Serial.println("");
    }
  }
  Serial.println("");
  delay(50);
}

//Interrupt implementations
void debounceRotate() {
  if((long)(micros()-lastRotate_micros)>=debounceDelay*1000) {
    if(digitalRead(rotateButton)==HIGH) {
      Serial.println("Rotate");
      blockRotate();
      serialPrintGame();
    }
  }
}
void debounceNew() {
  if((long)(micros()-lastNew_micros)>=debounceDelay*1000) {
    if(digitalRead(newBlockButton)==HIGH) {
      Serial.println("New Button");
      newBlock();
      serialPrintGame();
    }
  }
}

void newBlock() {
  //Picks a block based on a psuedo-random sequence generator
  if(bagLeft==0) {
    sequenceGenerator();
    bagLeft=7;
  }
  int blockChoice=RandBag[bagLeft-1];
  bagLeft--;
  //Block activeBlock();
  activeBlock.setBlockChoice(blockChoice);

  //Assigns initial indices for block type
  if(blockChoice==1) {
    //Generates I block
    activeBlock.addPosition(0,4);
    activeBlock.addPosition(1,5);
    activeBlock.addPosition(2,6);
    activeBlock.addPosition(3,7);
    blockCheck();
  }
  else if(blockChoice==2) {
    //Generates O block
    activeBlock.addPosition(0,5);
    activeBlock.addPosition(1,6);
    activeBlock.addPosition(2,1);
    activeBlock.addPosition(3,2);
    blockCheck();
  }
  else if(blockChoice==3) {
    //Generates T block
    activeBlock.addPosition(0,4);
    activeBlock.addPosition(1,5);
    activeBlock.addPosition(2,6);
    activeBlock.addPosition(3,1);
    blockCheck();
  }
  else if(blockChoice==4) {
    //Generates S block
    activeBlock.addPosition(0,4);
    activeBlock.addPosition(1,5);
    activeBlock.addPosition(2,1);
    activeBlock.addPosition(3,2);
    blockCheck();
  }
  else if(blockChoice==5) {
    //Generates Z block
    activeBlock.addPosition(0,5);
    activeBlock.addPosition(1,6);
    activeBlock.addPosition(2,0);
    activeBlock.addPosition(3,1);
    blockCheck();
  }
  else if(blockChoice==6) {
    //Generates J block
    activeBlock.addPosition(0,4);
    activeBlock.addPosition(1,5);
    activeBlock.addPosition(2,6);
    activeBlock.addPosition(3,0);
    blockCheck();
  }
  else if(blockChoice==7) {
    //Generates L block
    activeBlock.addPosition(0,4);
    activeBlock.addPosition(1,5);
    activeBlock.addPosition(2,6);
    activeBlock.addPosition(3,2);
    blockCheck();
  }
  activeBlock.resetRotate();
}

void blockCheck() {
  for(int i=0; i<width*height; i++) {
    boardArray[i]=0;
  }
  for(int i=0; i<4; i++) {
    boardArray[activeBlock.getPosition(i)]=activeBlock.getBlockChoice();
  }
}

void sequenceGenerator() {
  //shuffles a sequence into psuedo-random order
  randomSeed(random(0,9999)); //Further randomization
  int numbersLeft=7;
  int newRandBag[numbersLeft];
  for(int i=0; i<numbersLeft; i++) {
    newRandBag[i]=i+1;
  }
  //Shuffle, picking from remaining numbers
  while (numbersLeft>0) {
    int r=random(0, numbersLeft);
    RandBag[numbersLeft-1]=newRandBag[r];
    newRandBag[r]=newRandBag[--numbersLeft];
  }
  //Force rearranges last digit
  int x=random(10,59)/10;
  int temp=RandBag[x];
  RandBag[x]=RandBag[6];
  RandBag[6]=temp;
}

int zigzagUpdate(int counter) {
  //Adjusts counter value in order to properly display correct pixel
  //Serial.print("Counter is ");
  //Serial.println(counter);
  int row=counter/width;
  int column=counter-(row*width);
  if(row%2==1) {
    //If the row is odd, need to shift
    //Serial.println("Odd- shift");
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
  //Serial.println(fauxCounter);
  return fauxCounter;
}

void blockRotate() {
  //Currently only designed for clockwise rotation
  int rotaState=activeBlock.getRotation();
  int type=activeBlock.getBlockChoice();
  int activeIndices[4];
  for(int i=0; i<4; i++) {
    activeIndices[i]=activeBlock.getPosition(i);
  }
  if(type==1) {
    if(rotaState==1) {
      activeIndices[0]+=((2*width)+2);
      activeIndices[1]+=(width+1);
      activeIndices[2]+=0;
      activeIndices[3]-=(width+1);
    }
    else if(rotaState==2) {
      activeIndices[0]-=(width+2);
      activeIndices[1]-=1;
      activeIndices[2]+=width;
      activeIndices[3]+=((2*width)+1);
    }
    else if(rotaState==3) {
      activeIndices[0]+=(width+1);
      activeIndices[1]+=0;
      activeIndices[2]-=(width+1);
      activeIndices[3]-=((2*width)+2);
    }
    else if(rotaState==4) {
      activeIndices[0]-=((2*width)+1);
      activeIndices[1]-=width;
      activeIndices[2]+=1;
      activeIndices[3]+=(width+2);
    }
  }
  for(int i=0; i<4; i++) {
    activeBlock.addPosition(i, activeIndices[i]);
  }
  blockCheck();
  activeBlock.rotate();
}
