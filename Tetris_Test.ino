//Tetris code- test
//Written by Joshua Curfman as a personal project

int boardArray[200]; //Standard Tetris field is a 10x20 column

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
  MoveDown();
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
  //Deeply flawed, but functional. Currently moves all floating pieces down
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

void GenerateNewBlock() {
  //Currently generates a single I block
  int blockChoice=1;
  if(blockChoice==1) { //Gets into the if loop, but not the for loop?
    for(int i=5; i<=35; i+=10) {
      if(boardArray[i]!=0) {
        Serial.println("Game Over");
      }
      else {
        boardArray[i]=blockChoice;
      }
    }
  }
}
