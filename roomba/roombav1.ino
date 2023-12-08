#include <Wire.h>
#define left1 9 
#define left2 6 
#define right1 5 
#define right2 3 
#define pingPin1 11
#define echoPin1 10
#define pingPin2 8
#define echoPin2 7

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

void setup() {
Serial.begin(9600); // Starting Serial Terminal
  pinMode(pingPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
}

void loop(){
    
  
    int fdist = getfrontdist();
    int ldist = getleftdist();
    Serial.print("Distance = ");
    Serial.println(" cm");
    delay(100);
    brake();
    
    if(fdist<20 && fdist!= 0){ //front sensor
      delay(1000);
      right90();
      brake();
      delay(1000);
    }
    else if(ldist<20 && ldist!= 0){
      delay(1000);
      right90();
      brake();
      delay(1000);
      fwd();
      delay(2000);
    }
    fwdc(200);
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