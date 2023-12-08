#include <Wire.h>

#include <dht.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define left1 9 
#define left2 6 
#define right1 5 
#define right2 3 

#define pingPin1 11
#define echoPin1 10
#define pingPin2 8
#define echoPin2 7

#define DHT11_PIN A0
dht DHT;

#define hitA 4 //define digital I/O pin D4 for Hit A
#define hitB 2 //define digital I/O pin D2 for Hit B

#define buzz A1

int countA = 0; //number of hits received from Target board A
int countB = 0; //number of hits received from Target board B

bool Astate = false;
bool Bstate = false;

void tuturu(void);

void fwd(void);
void bkwd(void);
void fwdc(int);
void brake(void);
void left90(void);
void right90(void);
void back180(void);

unsigned int getfrontdist(void);
unsigned int getleftdist(void);

void readdht(void);

void readdht2(void);

void setup() {
  Serial.begin(9600); 
  Serial.println("hello");

  tuturu();

  lcd.init(); //lcd init
  lcd.begin(16, 2); // 16 characters, 2 lines
  lcd.backlight();  
  lcd.setCursor(0,0);
  lcd.print("morning ");
  delay(2000);

  pinMode(pingPin1, OUTPUT); //sensor init
  pinMode(echoPin1, INPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(left1, OUTPUT); //motor
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);

  pinMode(hitA, INPUT_PULLUP); //target init
  pinMode(hitB, INPUT_PULLUP); 
  PCICR |= B00000100; // Enable interrupts on PD port (D0 to D7)
  PCMSK2 |= B00010100; // Trigger interrupts on pins D2 and D4

  pinMode(buzz, OUTPUT);
}

void loop() {
  unsigned int fdist = getfrontdist(); //movement
  unsigned int ldist = getleftdist();
  if(Astate == true && Bstate == false){
    readdht();
    if(fdist<30 && fdist!= 0){ //check front sensor
      brake();
      Serial.println(fdist);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("smth in front");
      for (int i=0; i < 4; i++)
        {
        tone(buzz, 700, 100);
        delay(180);
        }
      bkwd();
      delay(100);
      brake();
      delay(300);
      right90();
      brake();
      delay(300);
  }
    else if(ldist<20 && ldist!= 0){ //check left sensor
      brake();
      for (int i=0; i < 3; i++)
        {
        tone(buzz, 1300, 100);
        delay(180);
        }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("smth on left");
      right90();
      brake();
      delay(300);
      fwd();
      delay(200);
      left90();
      brake();
      delay(1000);
    }
    else {
      fwdc(180);
    }
  }
  else if(Astate == false && Bstate == false){
    brake();
    readdht();
  }
  else if(Astate == false && Bstate == true){
    brake();
    readdht2();
  }
  else if(Astate == true && Bstate == true){
    brake();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reset target");
    lcd.setCursor(0,1);
    lcd.print("states to 0");
    Astate = false;
    Bstate = false;
  }
  else{
    brake();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("bro i got error help me");
    for (int i=0; i < 3; i++)
        {
        lcd.backlight();
        digitalWrite(buzz, HIGH);
        delay(200); 
        lcd.noBacklight();
        digitalWrite(buzz, LOW);
        delay(200); 
        }
  }
  
  }


void readdht(){
  lcd.clear();
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
}

void readdht2(){
  lcd.clear();
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Rec: 25-28");
  lcd.print((char)223);
  lcd.print("C");
  if(DHT.temperature < 25 || DHT.temperature > 28){
     for (int i = 0; i < 3; i++)
        {
          tone(buzz, 1300, 80);
          delay(100);
        }
     for (int i = 0; i < 3; i++)
        {
          tone(buzz, 600, 100);
          delay(120);
        }   
  }
  delay(700);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Rec: 50-70");
  lcd.print("%");
  if(DHT.humidity < 50 || DHT.humidity > 70){
     for (int i=0; i < 3; i++)
        {
          tone(buzz, 1300, 80);
          delay(100);
        }
     for (int i=0; i < 6; i++)
        {
          tone(buzz, 600, 100);
          delay(120);
        }   
  }  
  delay(700);
}

ISR (PCINT2_vect)
{
if (!digitalRead(hitA)){ 
  delay(1000);
  Astate = !Astate;
  Serial.println("Astate ");
  Serial.println(countA);
  tone(buzz, 700, 100);
  delay(100);
  }  
    
else if (!digitalRead(hitB)){
  delay(1000);
  Bstate = !Bstate;
  Serial.println("Bstate ");
  Serial.println(Bstate);
  tone(buzz, 700, 100);
  delay(150);
  tone(buzz, 700, 100);
  delay(100);
  }  
}

unsigned int getfrontdist(){
  int duration;
  int distance;
  digitalWrite(pingPin1, LOW);
  delayMicroseconds(2);   
  digitalWrite(pingPin1, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  digitalWrite(pingPin1, LOW);
  duration = pulseIn(echoPin1, HIGH); // receive reflected waves
  distance = duration / 58;   // convert to distance
  delay(10);
  return distance;
}

unsigned int getleftdist(){
  int duration;
  int distance;
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);   
  digitalWrite(pingPin2, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  digitalWrite(pingPin2, LOW);
  duration = pulseIn(echoPin2, HIGH); // receive reflected waves
  distance = duration / 58;   // convert to distance
  delay(10);
  return distance;
}

void bkwd(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
}

void fwd(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
}

void left90(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
    delay(130);
}

void right90(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
    delay(130);
}

void back180(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
    delay(260);
}

void fwdc(int i){
    digitalWrite(left1, HIGH);
    analogWrite(left2, i);
    digitalWrite(right1, HIGH);
    analogWrite(right2, i); 
}

void brake(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
}

void tuturu(){
  tone(buzz, 2000, 20);
  delay(20);//congrats on your tenz sub
	tone(buzz, 700, 80);
  delay(80);
  delay(200);
	tone(buzz, 2000, 20);
  delay(20);
	tone(buzz, 300, 100);
  delay(100);
	tone(buzz, 700, 550);
  delay(550);
  noTone;
}

