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
int getfrontdist(void);
int getleftdist(void);

void setup() {
Serial.begin(9600); // Starting Serial Terminal

  pinMode(pingPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop(){
    while ((getfrontdist() > 15) && (getleftdist() > 15)){
    Serial.println("MOVE front");
    Serial.println(getfrontdist());
    Serial.println(getleftdist());
    brake();
    fwdc(127);
    //delay(6000);
    //brake();
    //right90();
    //brake();
    }
   if ((getfrontdist() < 16)){
    Serial.println("front blocked");
    Serial.println(getfrontdist());
    Serial.println(getleftdist());
    brake();
    delay(400);
    right90();
    brake();
    delay(400);
    }
    if ((getleftdist() < 16)){
    Serial.println("left close");
    Serial.println(getfrontdist());
    Serial.println(getleftdist());
    brake();
    delay(400);
    right90();
    brake();
    delay(400);
    fwd();
    delay(1000);
    brake();
    left90();
    brake();
    }
}


int getfrontdist(){
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

int getleftdist(){
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
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
}

void bkwd(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
}

void left90(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
delay(800);
}

void right90(){
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
    delay(800);
}

void back180(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
    delay(1200);
}

void fwdc(int i){
    digitalWrite(left1, LOW);
    analogWrite(left2, i);
    digitalWrite(right1, LOW);
    analogWrite(right2, i);
}

void bkwdc(int i){
    digitalWrite(left1, HIGH);
    analogWrite(left2, i);
    digitalWrite(right1, HIGH);
    analogWrite(right2, i); 
    delay(20);
}

void brake(){
    digitalWrite(left1, HIGH);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, HIGH);
}