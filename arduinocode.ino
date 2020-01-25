#include <SoftwareSerial.h> // gerekli olan kutuphane
#include <Servo.h> // servomotor icin gerekli olan kutuphane
Servo myservomotor; 

int ledPin = 12; //led çıkışı
int state = 0;
int flag = 0; 
 
void setup() {
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW);
myservomotor.attach(8);  // arduinonun 8. pinini aktif yaptik.
myservomotor.write(0);  // motora ilk 0.derecesinden baslamasi komutunu verdik

 Serial.begin(9600); // Default connection rate for my BT module
}
 
void loop() {

 if(Serial.available() > 0){
 state = Serial.read();
 flag=0;


 if (state == '3') 
 {
 

  for ( int derece = 90; derece > 1; derece-- ) 
  { 
   myservomotor.write(derece);
   delay(10);
   }
 }

 else if (state == '2') 
 {
 for(int derece = 0; derece < 90; derece++)
 {   // 180 derecelik aci yap
  myservomotor.write(derece);
  delay(10);
  }
 }

 else if (state == '6') 
  {
    digitalWrite(ledPin, HIGH);
  }

  else if (state == '7') 
  {
    digitalWrite(ledPin, LOW);
  }

 
 
 }
 
}
