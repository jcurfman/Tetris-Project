int RandBag[7];
int numbersLeft = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  SequenceGenerator();
  for(int i=0; i<7; i++) {
    Serial.print(RandBag[i]);
    Serial.print(" ");
  }
  Serial.println("New Sequence");
  delay(800);
}

void SequenceGenerator() {
  //BUG- currently places the "1" value at the end of the RandBag array always. 
  //initialize array to shuffle
  int newRandBag[7];
  numbersLeft=7;
  for(int i=0; i<numbersLeft; i++) {
    newRandBag[i]=i+1;
  }
  //Shuffle, picking from remaining numbers
  while (numbersLeft > 0) {
    int r=random(0, numbersLeft);
    RandBag[numbersLeft-1]=newRandBag[r];
    newRandBag[r]=newRandBag[--numbersLeft];
  }
  //Resets randomization seed for next sequence run
  if(numbersLeft==0) {
    //Serial.println("New Sequence");
    int newRand=random(0,9999);
    randomSeed(newRand);
  }
}
