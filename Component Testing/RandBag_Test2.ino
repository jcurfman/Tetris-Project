int RandBag[7];

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
  //initialize array to shuffle
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
  //Resets randomization seed for next sequence run
  if(numbersLeft==0) {
    int newRand=random(0,9999);
    randomSeed(newRand);
  }
}
