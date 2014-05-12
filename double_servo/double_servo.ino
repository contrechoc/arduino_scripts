// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo1;  // create servo object to control a servo 
 Servo myservo2; 
 float inc = 0;
 
void setup() 
{ 
  myservo1.attach(6); 
 myservo2.attach(11); 
 
 //myservo3.attach(10); 
 //myservo4.attach(9); 
 pinMode(14, OUTPUT);
 pinMode(15, OUTPUT);
 pinMode(16, INPUT);
 pinMode(17, INPUT);
 digitalWrite(14, LOW);
  digitalWrite(15, HIGH);
} 
 
void loop() 
{ /*
  int val = analogRead(A2);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 255);     // scale it to use it with the servo (value between 0 and 180) 
  myservo1.write(val);                  // sets the servo position according to the scaled value 
  val = analogRead(A3);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 255);     // scale it to use it with the servo (value between 0 and 180) 
  myservo2.write(val);                  // sets the servo position according to the scaled value 
   */
myservo1.write(125 + 30.0*(sin(inc)));
myservo2.write(125 + 30.0*(sin(inc+PI/2.0)));

   inc+=0.005;
   if (inc > 2.0*3.14) inc = 0.0;
   delay(2);
} 
