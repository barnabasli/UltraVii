// This is the code for the non-retractable UltraVii 11.0. This version incorporates photoresistors to perform the same function as the gyroscope in the retractable prototype.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

//Pad
int padled = 9;
int padbrightness = 0;
int inPin = 2; //tilt switch

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
  Serial.begin(9600);

  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
  //init pad
    pinMode(padled, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    padbrightness = 0;
    //pinMode(inPin, INPUT);
    //digitalWrite(2, HIGH);
}

void loop() {
  analogWrite(padled, padbrightness);
  for (int i=0; i<255; i++)
  {
    padbrightness++;
    analogWrite(padled, padbrightness);
    delay(3);
  }
  timer();
  for (int i=0; i<255; i++)
  {
    padbrightness--;
    analogWrite(padled, padbrightness);
    delay(3);
  }
  exit(0);
}
void light()
{
  int level = analogRead(A0);
  int leveltwo = analogRead(A2);
  Serial.println(level);
  Serial.println(leveltwo);
  Serial.println("break");
  if (level > 40 || leveltwo > 40)
  {
    Serial.println("light");
    for (int i=0; i<255; i++)
    {
      padbrightness--;
      analogWrite(padled, padbrightness);
      delay(1);
    }
  level = analogRead(A0);
  leveltwo = analogRead(A2);
  while (level > 40 || leveltwo > 40)
  {
    delay(150);
    level = analogRead(A0);
    leveltwo = analogRead(A2);
    Serial.println(level);
    Serial.println(leveltwo);
  }
  Serial.println("dark");
  for (int i=0; i<255; i++)
  {
    padbrightness++;
    analogWrite(padled, padbrightness);
    delay(3);
  }
  }
  level = 0;
  leveltwo = 0;
}
void tilt()
{
  int digitalVal = digitalRead(2);
  if (LOW == digitalVal)
  {
    Serial.println("low");
    for (int i=0; i<255; i++)
    {
      padbrightness--;
      analogWrite(padled, padbrightness);
      delay(1);
    }
  while (LOW == digitalVal)
  {
    delay(200);
    digitalVal = digitalRead(2);
  }
  Serial.println("high");
  for (int i=0; i<255; i++)
  {
    padbrightness++;
    analogWrite(padled, padbrightness);
    delay(3);
  }
  }
}
void timer(void) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  for (int i=15; i>9; i--)
  {
    
  for (int j=108; j<=123; j++)
{
  display.drawPixel(j, 0, SSD1306_WHITE);
}
for (int j=108; j<=123; j++)
{
  display.drawPixel(j, 8, SSD1306_WHITE);
}
for (int j=0; j<=8; j++)
{
  display.drawPixel(108, j, SSD1306_WHITE);
}
for (int j=0; j<=8; j++)
{
  display.drawPixel(123, j, SSD1306_WHITE);
}
for (int k=124; k<=125; k++)
{
  for (int j=2; j<=6; j++)
  {
    display.drawPixel(k, j, SSD1306_WHITE);
  }
}
for (int j=2; j<=6; j++)
{
  for (int k=110; k<=119; k++)
  {
    display.drawPixel(k, j, SSD1306_WHITE);
  }
}
  display.setTextSize(3);
  display.setCursor(30, 6);
  display.print(("0:"));
  display.setCursor(60, 6);
  display.print((i));
  display.display();
  for (int i=0; i<20; i++)
  {
     delay(50);
     light();
  }
  display.clearDisplay();
  }
  for (int i=9; i>0; i--)
  {
  display.setTextSize(1);
  for (int j=108; j<=123; j++)
{
  display.drawPixel(j, 0, SSD1306_WHITE);
}
for (int j=108; j<=123; j++)
{
  display.drawPixel(j, 8, SSD1306_WHITE);
}
for (int j=0; j<=8; j++)
{
  display.drawPixel(108, j, SSD1306_WHITE);
}
for (int j=0; j<=8; j++)
{
  display.drawPixel(123, j, SSD1306_WHITE);
}
for (int k=124; k<=125; k++)
{
  for (int j=2; j<=6; j++)
  {
    display.drawPixel(k, j, SSD1306_WHITE);
  }
}
for (int j=2; j<=6; j++)
{
  for (int k=110; k<=119; k++)
  {
    display.drawPixel(k, j, SSD1306_WHITE);
  }
}
  display.setTextSize(3);
  display.setCursor(25, 6);
  display.print(("0:0"));
  display.setCursor(83, 6);
  display.print((i));
  display.display();
  for (int i=0; i<20; i++)
  {
     delay(50);
     light();
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
