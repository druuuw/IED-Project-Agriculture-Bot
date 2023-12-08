#define hitA 4 //define digital I/O pin D4 for Hit A
#define hitB 2 //define digital I/O pin D2 for Hit B
int countA = 0; //number of hits received from Target board A
int countB = 0; //number of hits received from Target board B

void setup() {
Serial.begin(9600);
pinMode(hitA, INPUT_PULLUP); //Use Hit_aPin as digital INPUT_PULLUP pin
pinMode(hitB, INPUT_PULLUP); //Use Hit_bPin as digital INPUT_PULLUP pin
PCICR |= B00000100; // Enable interrupts on PD port (D0 to D7)
PCMSK2 |= B00010100; // Trigger interrupts on pins D2 and D4
}

void loop() {
Serial.print("hitA ");
Serial.println(countA);
Serial.print("hitB ");
Serial.println (countB);
delay(1000);
 while((!digitalRead(hitB)) && (!digitalRead(hitA)))
  delay(500);
}

//Below is the interrupt function to detect Hit a(pin D4)and Hit b (pin D2)
ISR (PCINT2_vect)
{
if (!digitalRead(hitA))
  { 
  delay(1000);
  countA++;
  }  
    
  else if (!digitalRead(hitB))
  {
    delay(1000);
    countB++;
  }  
}