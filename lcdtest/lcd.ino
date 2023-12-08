#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
// addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
// addr can be 0x3F or 0x27
void setup(){
lcd.init(); // initialise the LCD
lcd.setCursor(0,0); //position cursor at top row left hand corner
lcd.begin(16, 2); // 16 characters, 2 lines
lcd.backlight(); // turn backlight on
lcd.setCursor(0, 0); // position cursor at character 0, line 0
lcd.print("Game is ready");
delay(500);
lcd.setCursor(0, 1); // position cursor at character 0, line 1
lcd.print("for Hit A and B");
delay(100);
}
void loop(){
}
