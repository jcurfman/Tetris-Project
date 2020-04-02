//Tetris code- test
//Written by Joshua Curfman as a personal project

int boardArray[200]; //Standard Tetris field is a 10x20 column
int ActivePiece[4]; //Stores pixel locations of active (dropping) Tetromino
int RandBag[7]; //Random Sequence Generated Drawbag

int bagLeft=0;
int blockChoice=0; //Easier to access from disparate functions if global
int col[4]; //Easier to access from disparate functions if global
int row[4]; //^ditto

#define resetPin 12 //Jumper from pin 12 to RESET pin
#define leftButton 2 //left button input
#define rightButton 3 //right button input
#define rotateButton 4 //rotation button input

const int debounceDelay=200;
//Changing variables for each button for non-interrupt debouncing
volatile unsigned long lastLeft_micros;
volatile unsigned long lastRight_micros;
volatile unsigned long lastRotate_micros;

void setup() {
  //Begin serial output. Prints an empty field and generates a block to start.
  Serial.begin(9600);
  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, LOW);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(rotateButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftButton), debounceLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(rightButton), debounceRight, RISING);
  //attachInterrupt(digitalPinToInterrupt(rotateButton), debounceRotate, RISING);
  //Uno only has two external interrupts
  randomSeed(analogRead(A0)); //Unconnected to anything- aids initial randomization
  Serial.println("");
  Serial.println("Begin Tetris Test");
  SerialPrintGame();
  NewBlock();
}

void loop() {
  //Base ongoing logic of the game
  static unsigned long timer=millis();
  if(millis()-timer>800) {
    //This section run once per frame
    //800 ms based loosely off speed of level 2 on NES Tetris
    SerialPrintGame();
    ActiveBlockDown();
    Serial.println("");
    timer=millis();
  }
}

void SerialPrintGame() {
  Serial.println("");
  //Prints game field to serial output, for troubleshooting purposes
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

//Need to implement interrupt for debouncing
//Uno only has two external interrupts
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

void debounceRotate() {
  if((long)(micros()-lastRotate_micros)>=debounceDelay*1000) {
    if(digitalRead(rotateButton)==LOW) {
      controlMove(4);
      lastRotate_micros=micros();
    }
  }
}

void controlMove(int choice) {
  bool canMove=true;
  if(choice==1) {
    //Move left
    Serial.println("Left");
    for(int i=0; i<4; i++) {
      if(ActivePiece[i]%10==0) {
        //Wall collision
        Serial.println("Wall");
        canMove=false;
      }
      else if(boardArray[ActivePiece[i]-1]!=0) {
        //Block collision- CURRENTLY DISABLED
        detectFalse(2);
        Serial.println("Occupied");
        //canMove=false;
      }
    }
    if(canMove==true) {
      Serial.println("Go");
      for(int i=0; i<4; i++) {
        boardArray[ActivePiece[i]]=0;
        Serial.print(ActivePiece[i]);
        --ActivePiece[i];
        boardArray[ActivePiece[i]]=blockChoice;
        Serial.print(" to ");
        Serial.println(ActivePiece[i]);
      }
    }
  }
  if(choice==2) {
    //Move right
    Serial.println("Right");
    for(int i=0; i<4; i++) {
      if(ActivePiece[i]%10==9) {
        //Wall collision
        Serial.println("Wall");
        canMove=false;
      }
      else if(boardArray[ActivePiece[i]-1]!=0) {
        //Block collision- CURRENTLY DISABLED
        Serial.println("Occupied");
        //canMove=false;
      }
    }
    if(canMove=true) {
      Serial.println("Go");
      for(int i=0; i<4; i++) {
        boardArray[ActivePiece[i]]=0;
        Serial.print(ActivePiece[i]);
        ++ActivePiece[i];
        boardArray[ActivePiece[i]]=blockChoice;
        Serial.print(" to ");
        Serial.println(ActivePiece[i]);
      }
    }
  }
  if(choice==4) {
    //Rotate
    Serial.println("Rotate");
  }
  SerialPrintGame();
}

void ActiveBlockDown() {
  //Runs a collision check, then moves the active piece down if allowed
  bool stopBlock=CollisionCheck();
  if(stopBlock==false) {
    for(int i=0; i<4; i++) {
      int val=boardArray[ActivePiece[i]];
      boardArray[ActivePiece[i]]=0;
      ActivePiece[i]+=10;
      boardArray[ActivePiece[i]]=val;
    }
  }
  //Generates a new block if no further motion is allowed
  else if(stopBlock==true) {
    for(int i=0; i<4; i++) {
      ActivePiece[i]=0;
    }
    NewBlock();
  }
}

void detectFalse(int choice) {
  //Checks and clears false collisions from active block
  //Depends on first active index values being lowermost or leftmost
  for(int i=0; i<4; i++) {
    //Find columns and rows
    row[i]=ActivePiece[i]/10;
    col[i]=ActivePiece[i]-(row[i]*10);
  }
  //Three sides-bottom, left, right
  if(choice==1) {
    //Detect downwards
    for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(blockChoice==1) {
          //Troublesome edge case fix for I block
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
  }
  if(choice==2) {
    //Detect leftwards
    for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(blockChoice==1) {
          //Still troublesome
          if(row[1]==row[2]) {
            //Assume horizontal
            for(int k=1; k<4; k++) {
              col[k]=99;
            }
          }
        }
        else if(row[i]==row[j] && col[i]==col[j]+1) {
          row[j]=99;
          col[j]=99;
        }
      }
    }
  }
}

bool CollisionCheck() {
  //Routine to check for collision between active block and other blocks
  bool stopBlock=false;
  detectFalse(1); //Checks for false collisions
  /*int col[4];
  int row[4];
  for(int i=0; i<4; i++) {
    //Find columns and rows
    row[i]=ActivePiece[i]/10;
    col[i]=ActivePiece[i]-(row[i]*10);
  }
  //Detect active block false collisions
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(blockChoice==1) { 
        //Troublesome edge case for I block
        if(row[1]==row[2]) {
          //Assume flat
        }
        else if(col[1]==col[2]) {
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
  }*/
  //Detects collisions, ignoring false collisions from above
  for(int i=0; i<4; i++) {
    if(col[i]<99) {
      if(boardArray[ActivePiece[i]+10]!=0) {
        stopBlock=true;
        //Serial.println("Block Collision");
      }
    }
  }
  //Checks for the bottom collision
  for(int k=0; k<4; k++) {
    if(ActivePiece[k]>189) {
      stopBlock=true;
    }
  }
  return stopBlock;
}

void NewBlock() {
  //Picks a block based on a psuedo-random sequence generator
  if(bagLeft==0) {
    SequenceGenerator();
    bagLeft=7;
  }
  blockChoice=RandBag[bagLeft-1];
  bagLeft--; 
  if(blockChoice==1) {
    //Generates I block
    ActivePiece[0]=35;
    ActivePiece[1]=25;
    ActivePiece[2]=15;
    ActivePiece[3]=5;
    blockCheck(blockChoice);
  }
  else if(blockChoice==2) {
    ActivePiece[0]=15;
    ActivePiece[1]=14;
    ActivePiece[2]=5;
    ActivePiece[3]=4;
    blockCheck(blockChoice);
  }
  else if(blockChoice==3) {
    ActivePiece[0]=13;
    ActivePiece[1]=14;
    ActivePiece[2]=15;
    ActivePiece[3]=4;
    blockCheck(blockChoice);
  }
  else if(blockChoice==4) {
    ActivePiece[0]=13;
    ActivePiece[1]=14;
    ActivePiece[2]=4;
    ActivePiece[3]=5;
    blockCheck(blockChoice);
  }
  else if(blockChoice==5) {
    ActivePiece[0]=16;
    ActivePiece[1]=15;
    ActivePiece[2]=5;
    ActivePiece[3]=4;
    blockCheck(blockChoice);
  }
  else if(blockChoice==6) {
    ActivePiece[0]=13;
    ActivePiece[1]=14;
    ActivePiece[2]=15;
    ActivePiece[3]=3;
    blockCheck(blockChoice);
  }
  else if(blockChoice==7) {
    ActivePiece[0]=14;
    ActivePiece[1]=15;
    ActivePiece[2]=16;
    ActivePiece[3]=6;
    blockCheck(blockChoice);
  }
}

void blockCheck(int blockChoice) {
  for(int i=0; i<4; i++) {
    if(boardArray[ActivePiece[i]]!=0) {
      Serial.println("Game Over");
      delay(100);
      pinMode(resetPin, OUTPUT); //Reset system
      while(1);
    }
    else {
      boardArray[ActivePiece[i]]=blockChoice;
    }
  }
}

void SequenceGenerator() { 
  //Shuffles a sequence into psuedo-random order
  randomSeed(random(0,9999)); //Further randomizes from previous sequences
  int numbersLeft=7;
  int newRandBag[numbersLeft];
  for(int i=0; i<numbersLeft; i++) {
    newRandBag[i]=i+1;
  }
  //Shuffle, picking from remaining numbers
  while (numbersLeft > 0) {
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
