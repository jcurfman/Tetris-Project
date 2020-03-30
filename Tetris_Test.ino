//Tetris code- test
//Written by Joshua Curfman as a personal project

int boardArray[200]; //Standard Tetris field is a 10x20 column
int ActivePiece[4]; //Stores pixel locations of active (dropping) Tetromino

#define resetPin 12 //Jumper from pin 12 to RESET pin
#define NumPieces 7

void setup() {
  //Begin serial output. Prints an empty field and generates a block to start.
  Serial.begin(9600);
  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, LOW);
  Serial.println("");
  Serial.println("Begin Tetris Test");
  SerialPrintGame();
  NewBlock();
}

void loop() {
  //Base ongoing logic of the game, run once per frame. 
  SerialPrintGame();
  MoveDown();
  //ActiveBlockDown();
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
    NewBlock();
    Serial.println("New Block");
  }
}

bool CollisionCheck() {
  //Routine to check for collision between active block and other blocks
  bool stopBlock=false;
  for(int i=0; i<4; i++) {
    if(boardArray[ActivePiece[i]+10]!=0) {
      if(ActivePiece[i]+10!=ActivePiece[i-1]) {
        stopBlock=true;
        Serial.println("Block Collision");
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
  //Currently generates a single I block
  int blockChoice=random(1,5); //Replace with RandBag Generator function
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
}

void blockCheck(int blockChoice) {
  for(int i=0; i<4; i++) {
    if(boardArray[ActivePiece[i]]!=0) {
      Serial.println("Game Over");
      pinMode(resetPin, OUTPUT); //Reset system
      while(1);
    }
    else {
      boardArray[ActivePiece[i]]=blockChoice;
    }
  }
}
