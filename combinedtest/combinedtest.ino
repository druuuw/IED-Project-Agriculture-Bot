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
#define DHT11_PIN A1
#define hitA 4 //define digital I/O pin D4 for Hit A
#define hitB 2 //define digital I/O pin D2 for Hit B
#define buzz 12
dht DHT;
int targetA = 0; //number of hits received from Target board A
int targetB = 0; //number of hits received from Target board B
int Astate = 0;
int Bstate = 0;
void fwd(void);
void bkwd(void);
void fwdc(int);
void bkwdc(int);
void brake(void);
void left90(void);
void right90(void);
void back180(void);
unsigned int getfrontdist(void);
unsigned int getleftdist(void);
void readdht(void);


void setup() {
  Serial.begin(9600); 
  Serial.println("hello");

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

  readdht();

  while(targetA == 0){ //wait for start/stop signal
  readdht();
  }; 
  targetA = 0; //reset start/stop signal back to 0 for next input
}

void loop() {

  int fdist = getfrontdist(); //movement
  int ldist = getleftdist();
  brake();
  readdht();
  while(targetA == 0){ //main loop until start/stop becomes 1
    if(fdist<20 && fdist!= 0){ //check front sensor
      for (int i=0; i < 4; i++)
        {
        digitalWrite(buzzer, HIGH);// produce a sound
        delay(5); 
        digitalWrite(buzzer, LOW);//No sound
        delay(5);
        }
      Serial.print("fDistance = ");
      Serial.print(fdist);
      Serial.println(" cm");
      delay(50);
      brake();
      delay(1000);
      right90();
      brake();
      delay(1000);
  }
    else if(ldist<20 && ldist!= 0){ //check left sensor
     for (int i=0; i < 3; i++)
        {
        digitalWrite(buzzer, HIGH);// produce a sound
        delay(5); 
        digitalWrite(buzzer, LOW);//No sound
        delay(5);
        }
      Serial.print("lDistance = ");
      Serial.print(ldist);
      Serial.println(" cm");
      brake();
      delay(1000);
      right90();
      brake();
      delay(1000);
      fwd();
      delay(1000);
      left90();
      delay(1000);
    }
    else {
      fwdc(150);
      readdht();
    }
    }
  brake(); //brake if target recieved signal to stop
  targetA = 0; //reset start/stop signal back to 0 for next input
  while(targetA == 0){ //loop until target gets another signal to resume
    readdht();
  }
  targetA = 0; //reset start/stop signal back to 0 for next input
}

void readdht(){
  lcd.clear();
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  Serial.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  Serial.print(DHT.humidity);
  lcd.print("%");
  delay(1000);
}

ISR (PCINT2_vect)
{
if (!digitalRead(hitA))
  { 
  delay(1000);
  targetA = 1;
  Serial.println("hitA")
  }  
    
  else if (!digitalRead(hitB))
  {
    delay(1000);
    targetB = 1;
    Serial.println("hitB")
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

void fwd(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
}

void bkwd(){
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
    delay(400);
}

void right90(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
    delay(400);
}

void back180(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
    delay(1200);
}

void fwdc(int i){
    digitalWrite(left1, HIGH);
    analogWrite(left2, i);
    digitalWrite(right1, HIGH);
    analogWrite(right2, i); 
}

void bkwdc(int i){
    digitalWrite(left1, LOW);
    analogWrite(left2, i);
    digitalWrite(right1, LOW);
    analogWrite(right2, i);
}

void brake(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
  
}
