//Tetris Test code Mark 2
//Written by Joshua Curfman as a personal project

class Block {
  int blockChoice; //The shape of the block, which acts as an identifer visually
  int ActiveIndices[4]; //The indices of the active block within the Tetris field array
  int rotation; //The rotation of the block

  //Constructor
  public: Block() {
    //block type is a value between 1 and 7 denoting one of the 7 tetromino shapes
    //blockChoice=blockType;
    //Also sets default rotation
    rotation=1;
  }

  //Data entry functions
  void setBlockChoice(int blockType) {
    //Sets block type, a value between 1 and 7 denoting one of the 7 tetromino shapes
    blockChoice=blockType;
  }
  void addPosition(int index, int value) {
    //Adds the positional data for the active block
    ActiveIndices[index]=value;
  }
  void rotate() {
    //rotation state is an integer between 1 and 4, with 1 being the default
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
    //Returns coordinate of index
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
//BLOCK CLASS ENDS HERE------------------------------

#include <FastLED.h>

//Field Size Definition- standard tetris field is a 10x20 column
#define width 10
#define height 20

int boardArray[width*height]; //Array for playfield
int RandBag[7]; //Random generated sequence drawbag
int rgb[3];

int bagLeft=0;
Block activeBlock;

#define resetPin 12 //jumper from pin 12 to RESET pin
#define ledPin 7 //WS2812b strip data pin
#define leftButton 2 //left button input
#define rightButton 3 //right button input
#define rotateButton 4 //rotation button input

const int debounceDelay(200);
//Changing variables for each button for debouncing
volatile unsigned long lastLeft_micros;
volatile unsigned long lastRight_micros;
volatile unsigned long lastRotate_micros;

const int numLed=width*height;
int fauxCounter=0;
CRGB leds[numLed];

void setup() {
  //Startup sequence. Begins serial output, prints an empty field, and generates a first block
  Serial.begin(9600);
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numLed);
  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, LOW);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(rotateButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftButton), debounceLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(rightButton), debounceRight, RISING);
  //attachInterrupt(digitalPinToInterrupt(rotateButton), debounceRotate, RISING);
  //Uno only has two external interrupts- Feather M0 has 20, I believe? MEGA has 6.
  randomSeed(analogRead(A0)); //unconnected to anything- aids initial randomization

  Serial.println("");
  Serial.println("Begin Tetris Test (Mk2)");
  serialPrintGame();
  newBlock();
  delay(1000);
}

void loop() {
  static unsigned long timer=millis();
  //Serial.println(timer);
  if(millis()-timer>800) {
    activeBlockDown();
    serialPrintGame();
    timer=millis();
  }
}

void serialPrintGame() {
  //Prints game field to serial output, for troubleshooting and development
  for(int i=0; i<(sizeof(boardArray)/sizeof(boardArray[0])); i++) {
    Serial.print("[");
    Serial.print(boardArray[i]);
    Serial.print("]");
    if(i%10==9) {
      Serial.println("");
    }
  }
  Serial.println("");
}

void ledUpdate() {
  //Uncomment the serial print sections for block by block readout if troubleshooting
  for(int i=0; i<(width*height); i++) {
    blockColor(boardArray[i]);
    int ledPosition=zigzagUpdate(i);
    leds[ledPosition]=CRGB(rgb[0],rgb[1],rgb[2]);
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
  /*for(int i=0; i<3; i++) {
    int temp=rgb[i];
    rgb[i]=temp/2;
  }*/
}

//Interrupt implementations
void debounceLeft() {
  if((long)(micros()-lastLeft_micros)>=debounceDelay*1000) {
    if(digitalRead(leftButton)==LOW) {
      controlMove(1);
      lastLeft_micros=micros();
    }
  }
}
void debounceRight() {
  if((long)(micros()-lastRight_micros)>=debounceDelay*1000) {
    if(digitalRead(rightButton)==LOW) {
      controlMove(2);
      lastRight_micros=micros();
    }
  }
}

void controlMove(int choice) {
  bool stopBlock=collisionCheck();
  Serial.println("");
  if(choice==1) {
    //Move Left
    Serial.println("Left");
  }
  if(choice==2) {
    //Move Right
    Serial.println("Right");
  }
  if(choice==3) {
    //Rotate block
    Serial.println("Rotate");
  }
  //serialPrintGame();
}

void activeBlockDown() {
  //Runs a collision check, then moves the active piece down if allowed
  bool stopBlock=collisionCheck();
  if(stopBlock==false) {
    for(int i=0; i<4; i++) {
      int val=activeBlock.getPosition(i);
      boardArray[val]=0;
      val+=width;
      activeBlock.addPosition(i,val);
      boardArray[val]=activeBlock.getBlockChoice();
    }
  }
  else if(stopBlock==true) {
    for(int i=0; i<4; i++) {
      activeBlock.addPosition(i,0);
    }
    newBlock();
    Serial.println("New Block");
  }
}

bool collisionCheck() {
  //Routine to check for collision between active block and others
  //Old version- replacement in development
  bool stopBlock=false;
  int col[4];
  int row[4];
  for(int i=0; i<4; i++) {
    //Find rows and columns
    row[i]=activeBlock.getPosition(i)/width;
    col[i]=activeBlock.getPosition(i)-(row[i]*width);
  }
  //Detect false self collisions
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(activeBlock.getBlockChoice()==1) {
        //Troublesome edge case for I block
        if(col[1]==col[2]) {
          //Assume vertical
          for(int k=1; k<4; k++) {
            col[k]=99;
          }
        }
      }
      else if(col[i]==col[j] && row[i]==row[j]+1) {
        row[j]=99;
        col[j]=99;
      }
    }
  }
  //Detects collisions, throwing out the false collisions from above
  for(int i=0; i<4; i++) {
    if(col[i]<99) {
      if(boardArray[activeBlock.getPosition(i)+width]!=0) {
        stopBlock=true;
      }
    }
  }
  for(int i=0; i<4; i++) {
    if(activeBlock.getPosition(i)>189) {
      stopBlock=true;
    }
  }
  return stopBlock;
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
    activeBlock.addPosition(0,35);
    activeBlock.addPosition(1,25);
    activeBlock.addPosition(2,15);
    activeBlock.addPosition(3,5);
    blockCheck();
  }
  else if(blockChoice==2) {
    //Generates O block
    activeBlock.addPosition(0,14);
    activeBlock.addPosition(1,15);
    activeBlock.addPosition(2,4);
    activeBlock.addPosition(3,5);
    blockCheck();
  }
  else if(blockChoice==3) {
    //Generates T block
    activeBlock.addPosition(0,13);
    activeBlock.addPosition(1,14);
    activeBlock.addPosition(2,15);
    activeBlock.addPosition(3,4);
    blockCheck();
  }
  else if(blockChoice==4) {
    //Generates S block
    activeBlock.addPosition(0,13);
    activeBlock.addPosition(1,14);
    activeBlock.addPosition(2,4);
    activeBlock.addPosition(3,5);
    blockCheck();
  }
  else if(blockChoice==5) {
    //Generates Z block
    activeBlock.addPosition(0,15);
    activeBlock.addPosition(1,16);
    activeBlock.addPosition(2,4);
    activeBlock.addPosition(3,5);
    blockCheck();
  }
  else if(blockChoice==6) {
    //Generates J block
    activeBlock.addPosition(0,13);
    activeBlock.addPosition(1,14);
    activeBlock.addPosition(2,15);
    activeBlock.addPosition(3,3);
    blockCheck();
  }
  else if(blockChoice==7) {
    //Generates L block
    activeBlock.addPosition(0,14);
    activeBlock.addPosition(1,15);
    activeBlock.addPosition(2,16);
    activeBlock.addPosition(3,6);
    blockCheck();
  }
  activeBlock.resetRotate();
}

void blockCheck() {
  for(int i=0; i<4; i++) {
    if(boardArray[activeBlock.getPosition(i)]!=0) {
      Serial.println("Game Over");
      delay(100);
      pinMode(resetPin, OUTPUT); //resets whole system
      while(1);
    }
    else {
      boardArray[activeBlock.getPosition(i)]=activeBlock.getBlockChoice();
    }
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
