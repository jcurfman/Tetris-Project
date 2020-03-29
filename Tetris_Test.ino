//Tetris code- test
//Written by Joshua Curfman as a personal project

int boardArray[200]; //Standard Tetris field is a 10x20 column
int ActivePiece[4]; //Stores pixel locations of active (dropping) Tetromino

#define NumPieces 7

void setup() {
  //Begin serial output. Prints an empty field and generates a block to start.
  Serial.begin(9600);
  Serial.println("Begin Tetris Test");
  SerialPrintGame();
  GenerateNewBlock();
}

void loop() {
  //Base ongoing logic of the game, run once per frame. 
  SerialPrintGame();
  ActiveBlockDown();
  Serial.println("");
  //delay is based loosely off speed of level 2 on NES Tetris.
  delay(800); 
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
  //Deeply flawed, but functional. Moves all floating pieces down
  for(int i=(sizeof(boardArray)/sizeof(boardArray[0]))-1; i>=0; i--) {
    if(boardArray[i]==0 && boardArray[i-10]!=0) {
      if(i<10) {
        break;
      }
      boardArray[i]=boardArray[i-10];
      boardArray[i-10]=0;
    }
  }
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
    GenerateNewBlock();
    Serial.println("New Block");
  }
}

bool CollisionCheck() {
  //Routine to check for collision between active block and other blocks
  bool stopBlock=false;
  //Currently broken somewhere- block collision is not yet working
  for(int i=0; i<4; i++) {
    if(boardArray[ActivePiece[i]+10]!=0) {
      stopBlock=true;
      for(int j=0; j<4; j++) {
        int compareVal=ActivePiece[i]+10;
        if(compareVal=ActivePiece[j]) {
          stopBlock=false;
        }
      } 
    }
  }
  //Also checks for the bottom- WORKING
  for(int k=0; k<4; k++) {
    if(ActivePiece[k]>189) {
      stopBlock=true;
    }
  }
  return stopBlock;
}

void GenerateNewBlock() {
  //Currently generates a single I block
  int blockChoice=1;
  if(blockChoice==1) {
    //Generates I block
    ActivePiece[0]=35;
    ActivePiece[1]=25;
    ActivePiece[2]=15;
    ActivePiece[3]=5;
    for(int i=0; i<4; i++) {
      if(boardArray[ActivePiece[i]]!=0) {
        Serial.println("Game Over");
      }
      else {
        boardArray[ActivePiece[i]]=blockChoice;
      }
    }
  }
}
