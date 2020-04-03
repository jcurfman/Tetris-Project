#include <FastLED.h>

#define LED_PIN 7
#define numLED 9

CRGB leds[numLED];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, numLED);
  Serial.println("Begin LED Test");
}

void loop() {
  for(int i=0; i<numLED; i++) {
    leds[i]=CRGB(0,0,0);
    FastLED.show();
    delay(500);
  }
  leds[0]=CRGB(128,0,0);
  FastLED.show();
  delay(500);
  leds[1]=CRGB(0,128,0);
  FastLED.show();
  delay(500);
  leds[2]=CRGB(0,0,128);
  FastLED.show();
  delay(500);
  Serial.println("Initial");
  delay(2000);
  for(int i=0; i<3; i++) {
    leds[i]=CRGB(0,0,0);
    FastLED.show();
  }
  int r=10;
  int g=0;
  int b=0;
  while(r<128) {
    //red grow
    for(int i=0; i<numLED; i++) {
      leds[i]=CRGB(r,g,b);
      FastLED.show();
      delay(50);
    }
    r+=20;
    if(r>128) {
      r=128;
    }
  }
  while(r>=128 && b<128) {
    //blue grow, into purple
    for(int i=0; i<numLED; i++) {
      leds[i]=CRGB(r,g,b);
      FastLED.show();
      delay(10);
    }
    b+=10;
    if(b>128) {
      b=128;
    }
  }
  while(r>1) {
    //red shrink, into blue
    for(int i=0; i<numLED; i++) {
      leds[i]=CRGB(r,g,b);
      FastLED.show();
      delay(10);
    }
    r-=10;
    if(r<1) {
      r=0;
    }
  }
}
