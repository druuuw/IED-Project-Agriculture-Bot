#define buzzer A0

void setup(){
pinMode(buzzer, OUTPUT);
}

void loop(){
for (int i=0; I < 80; I++)
{
digitalWrite(buzzer, HIGH);// produce a sound
delay(1); //delay 1ms
digitalWrite(buzzer, LOW);//No sound
delay(1);// delay 1ms
}
delay(50);

for (int j=0; j<100; j++)
{
digitalWrite(buzzer, HIGH);// produce another sound
delay(2);//delay 2ms
digitalWrite(buzzer, LOW);// no sound
delay(2);//delay 2ms
delay(100);
}
}

