//Tetris code- test
//Written by Joshua Curfman as a personal project

int boardArray[200]; //Standard Tetris field is a 10x20 column
int ActivePiece[4]; //Stores pixel locations of active (dropping) Tetromino
int RandBag[7]; //Random Sequence Generated Drawbag

int bagLeft=0;

#define resetPin 12 //Jumper from pin 12 to RESET pin
#define NumPieces 7

void setup() {
  //Begin serial output. Prints an empty field and generates a block to start.
  Serial.begin(9600);
  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, LOW);
  randomSeed(analogRead(A0)); //Unconnected to anything- aids initial randomization
  Serial.println("");
  Serial.println("Begin Tetris Test");
  SerialPrintGame();
  NewBlock();
}

void loop() {
  //Base ongoing logic of the game, run once per frame. 
  SerialPrintGame();
  ActiveBlockDown();
  Serial.println("");
  //delay is based loosely off speed of level 2 on NES Tetris.
  delay(800); //800 is playing default?
}

void SerialPrintGame() {
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

void MoveDown() {
  //Deeply flawed. Moves all floating pieces down
  for(int i=(sizeof(boardArray)/sizeof(boardArray[0]))-1; i>=0; i--) {
    if(boardArray[i]==0 && boardArray[i-10]!=0) {
      if(i<10) {
        break;
      }
      boardArray[i]=boardArray[i-10];
      boardArray[i-10]=0;
    }
    else if(boardArray[i]!=0) {
      NewBlock();
      Serial.println("New Block");
    }
  }
}

void ActiveBlockDown() {
  //Runs a collision check, then moves the active piece down if allowed
  bool stopBlock=CollisionCheck();
  if(stopBlock==false) {
    //Collision Check very broken
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
    Serial.println("New Block");
  }
}

bool CollisionCheck() {
  //Routine to check for collision between active block and other blocks
  bool stopBlock=false;
  int col[4];
  int row[4];
  for(int i=0; i<4; i++) {
    //Find columns and rows
    row[i]=ActivePiece[i]/10;
    col[i]=ActivePiece[i]-(row[i]*10);
    Serial.print(col[i]);
    Serial.print(", ");
    Serial.println(row[i]);
  }
  //Detect active block false collisions- TEST. NOT WORKING WITH I BLOCK
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(col[i]==col[j] && row[i]==row[j]+1) {
        Serial.println("Same block?");
        row[j]=99;
        col[j]=99;
      }
    }
  }
  //Detects collisions, ignoring false collisions from above
  for(int i=0; i<4; i++) {
    if(col[i]<99) {
      if(boardArray[ActivePiece[i]+10]!=0) {
        stopBlock=true;
        Serial.println("Block Collision");
      }
    }
    Serial.print(col[i]);
    Serial.print(", ");
    Serial.println(row[i]);
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
  int blockChoice=RandBag[bagLeft-1];
  bagLeft--;
  //int blockChoice=random(1,7); //Replace with RandBag Generator function
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
