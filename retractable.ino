//Does NOT include photoresistors or a tilt sensor but rather utilizes a gyroscope
//LED RING vars
#include <FastLED.h>
#define NUM_LEDS 12
#define DATA_PIN 3
#define BRIGHTNESS 100
CRGB leds[NUM_LEDS]; //array of leds, don't touch
int ledbrightnesstemp;
boolean gyros = false;
boolean lol;
int blinkredcount;

//Gyroscope
#include<Wire.h>
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265;
int maxVal=402;
double x;
double y;
double z;
int xAng=0;
int yAng=0;
int zAng=0;

//Pad
int padled = 9;
int padbrightness = 0;
int padfade = 5; //not used

void setup() { 
    // init leds
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    // init gyro
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
    Serial.begin(115200);
    //init pad
    pinMode(padled, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    padbrightness = 0;
    gyros = false;
}

void loop() { 
  analogWrite(padled, padbrightness);
  analogWrite(LED_BUILTIN, padbrightness);
  poweron();
  int what = 255; // temp variable to hold padbrightness
  // cleaningcycle
  for (int k=11; k>=0; k--)
  {
    for (int a=255; a>=0; a--)
    {
      leds[k] = CHSV(160, 255, a);
      FastLED.show();
      delay(4);
      gyro();
    if (x>=30 && x<=180 || x>180 && x<=330) {
     for (int c=255; c>0; c--)
     {
        while (c>100)
        {
          padbrightness--;
          analogWrite(padled, padbrightness);
          c--;
        }
        padbrightness--;
        analogWrite(padled, padbrightness);
        Serial.println(padbrightness);
        delay(3.5);
     }
     ledblack();
     blinkred();
    while(x>=30 && x<=180 || x>180 && x<=330) //checks x rot
    {
      //reruns gyro sequence endlessly while condition is true
      gyros = true;
      blinkred();
    }
    //after xrot <30, increase pad brightness back to original
    ledblack();
    gyros = false;
    for (int c=0; c<255; c+=5)
     {
      for (int i=0; i<=k; i++)
      {
        leds[i] = CHSV(160,255,c);
        FastLED.show();
      }
      padbrightness+=5;
      analogWrite(padled, padbrightness);
      delay(2);
      Serial.println(padbrightness);      
     }
    a=255;
    }
    }
  }
  // turn off pad leds
  for (int i=255; i>0; i--)
  {
    padbrightness--;
    analogWrite(padled, padbrightness);
    delay(4);
    Serial.println(padbrightness);
  }
  exit(0);
}
void blinkred() {
  ledblack();
  for (int i=0; i<255; i+=5)
  {
    gyro();
    for (int j=0; j<12; j++)
    {
      leds[j] = CHSV(352,255,i);
    }
    FastLED.show();
    delay(6);
  }
}
void ledblack(){
  for (int j=0; j<12; j++)
  {
    leds[j] = CHSV(0,0,0);
    FastLED.show();
  }
}
void poweron() {
  for (int j=0; j<11; j++)
  {
    leds[j] = CRGB::Black;
    FastLED.show();
  }
  for (int i=0; i<11; i++)
  {
    for (int k=0; k<23; k++)
    {
      padbrightness++;
      analogWrite(padled, padbrightness);
      delay(3);
    }
    analogWrite(padled, padbrightness);
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(20);
  }
  padbrightness++;
  padbrightness++;
  Serial.println(padbrightness);
}
void gyro(){
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,true);
      AcX=Wire.read()<<8|Wire.read();
      AcY=Wire.read()<<8|Wire.read();
      AcZ=Wire.read()<<8|Wire.read();
      xAng = map(AcX,minVal,maxVal,-90,90);
      yAng = map(AcY,minVal,maxVal,-90,90);
      zAng = map(AcZ,minVal,maxVal,-90,90);
      x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); //updates values of instance variables
      y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
      z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
}
