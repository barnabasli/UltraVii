// This code is for the non-retractable UltraVii 9.0

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

//gyroscope
#include<Wire.h>
const int MPU_addr = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal = 265;
int maxVal = 402;
double x;
double y;
double z;
int xAng = 0;
int yAng = 0;
int zAng = 0;
boolean liftedup = false;

//pad
int padled = 9;
int padbrightness = 0;

//display
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int timeremaining = 15;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
  //initialize pad
  pinMode(padled, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  padbrightness = 0;
  //initialize gyroscope
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
}

void loop() {
  analogWrite(padled, padbrightness);
  //increases pad brightness to max
  for (int i = 0; i < 255; i++)
  {
    padbrightness++;
    analogWrite(padled, padbrightness);
    delay(3);
  }
  timer();
  //decreases pad brightness to zero
  for (int i = 0; i < 255; i++)
  {
    padbrightness--;
    analogWrite(padled, padbrightness);
    delay(3);
  }
  exit(0);
}

void batteryindicator() {
  //draws battery indicator
  display.setTextColor(SSD1306_WHITE);
  for (int j = 108; j <= 123; j++) {
      display.drawPixel(j, 0, SSD1306_WHITE);
      display.drawPixel(j, 8, SSD1306_WHITE);
  }
  for (int j = 0; j <= 8; j++) {
      display.drawPixel(108, j, SSD1306_WHITE);
      display.drawPixel(123, j, SSD1306_WHITE);
  }
  for (int k = 124; k <= 125; k++) {
    for (int j = 2; j <= 6; j++) {
      display.drawPixel(k, j, SSD1306_WHITE);
    }
  }
  for (int j = 2; j <= 6; j++) {
    for (int k = 110; k <= 119; k++) {
      display.drawPixel(k, j, SSD1306_WHITE);
    }
  }
}

void timer() {
  for (int i = 15; i > 9; i--) {
    display.clearDisplay();
    batteryindicator();
    display.setTextSize(3);
    display.setCursor(30, 6);
    display.print(("0:"));
    display.setCursor(60, 6);
    display.print((i));
    display.display();
    timeremaining = i;
    for (int j = 0; j < 20; j++) {
      check();
      if (liftedup) {
        display.clearDisplay();
        batteryindicator();
        display.setCursor(30, 6);
        display.print(("0:"));
        display.setCursor(60, 6);
        display.print((i));
        display.display();
        liftedup = false;
      }
      delay(50);
    }
    display.clearDisplay();
  }
  for (int i = 9; i > 0; i--) {
    batteryindicator();
    display.setTextSize(3);
    display.setCursor(25, 6);
    display.print(("0:0"));
    display.setCursor(83, 6);
    display.print((i));
    display.display();
    timeremaining = i;
    for (int j = 0; j < 20; j++) {
      check();
      delay(50);
    }
    display.clearDisplay();
  }
  display.setTextSize(2);
  display.setCursor(16, 0);
  display.println(("Cleaning"));
  display.display();
  display.setCursor(14, 18);
  display.println(("Finished!"));
  display.display();
  display.clearDisplay();
}

void gyro() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX = Wire.read()<<8|Wire.read();
  AcY = Wire.read()<<8|Wire.read();
  AcZ = Wire.read()<<8|Wire.read();
  xAng = map(AcX,minVal,maxVal,-90,90);
  yAng = map(AcY,minVal,maxVal,-90,90);
  zAng = map(AcZ,minVal,maxVal,-90,90);
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI); //updates values of instance variables
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
}

void check() {
  gyro();
  if (x >= 30 && x <= 330) {
    for (int i = 255; i > 0; i--) {
      padbrightness--;
      analogWrite(padled, padbrightness);
      delay(1.7);
   }
  while (x >= 30 && x <= 330) {
    for (int i = 0; i < 10; i++) {
      delay(50);
      gyro();
      if (x < 30 || x > 330) {
        goto label1;
      }
    }
    display.clearDisplay();
    batteryindicator();
    display.display();
    for (int i = 0; i < 10; i++) {
      delay(50);
      gyro();
      if (x < 30 || x > 330) {
        goto label1;
      }
    }
    holdtime();
  }
  label1:
    holdtime();
    for (int i = 0; i < 255; i++) {
      padbrightness++;
      analogWrite(padled, padbrightness);
      delay(3.5);
    }
  liftedup = true;
  }
}

void holdtime() {
  display.clearDisplay();
  if (timeremaining > 9) {
    batteryindicator();
    display.setTextSize(3);
    display.setCursor(30, 6);
    display.print(("0:"));
    display.setCursor(60, 6);
    display.print((timeremaining));
    display.display();
  } else {
    batteryindicator();
    display.setTextSize(3);
    display.setCursor(25, 6);
    display.print(("0:0"));
    display.setCursor(83, 6);
    display.print((timeremaining));
    display.display();
  }
}
