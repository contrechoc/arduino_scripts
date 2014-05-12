 
 #include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper = AccelStepper(7,6,8,4); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
//AccelStepper stepper;

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 500 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

long t= millis();
long interval = 5000;

int scaleDistance = 1;
int timer1_counter;

volatile int switch1 = 0;

int stepperPos = 0;

void setup()
{
   stepper.setMaxSpeed(150);
  stepper.setAcceleration(100);
  
  pinMode ( 14, INPUT);
  digitalWrite(14, HIGH);
  
      pinMode ( 13, OUTPUT);
  digitalWrite(13, HIGH);
  
    pinMode ( 10, OUTPUT);
  digitalWrite(10, LOW);
  

  pinMode ( 15, INPUT);
  digitalWrite( 15, HIGH);

  pinMode ( 5, OUTPUT);
  digitalWrite(5, LOW);
  Serial.begin(9600);
  t = millis() + interval;
  
  Serial.begin(9600);

    // initialize timer1 
    
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286/4;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  
    stepper.moveTo(500);
  while (stepper.currentPosition() != 300) // Full speed up to 300
    stepper.run();
  stepper.runToNewPosition(0); // Cause an overshoot then back to 0

  
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
   
  if ( switch1 == 1 )
  digitalWrite(5, digitalRead(5) ^ 1);
 
   
}


void loop()
{
  //digitalWrite(5, LOW);
   switch1 == 0;
  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping();
  digitalWrite(5, digitalRead(5) ^ 1);
  uS/= US_ROUNDTRIP_CM;
  Serial.println(uS);
  
  if ( (uS > 5) && (uS < 230)){
   Serial.println(uS);

 
    if ( uS < 150*scaleDistance){
     // digitalWrite(5, HIGH);
       //t = millis() + interval;
        switch1 == 1;
       stepperPos += 500;
      stepper.moveTo(stepperPos);   
       while (stepper.currentPosition() != stepperPos - 100) // Full speed up to 300
    stepper.run();
       stepper.runToNewPosition(0);
       
         delay(30); 
    }
    else if ( uS < 200*scaleDistance ){
    
        stepperPos -= 300;
      stepper.moveTo(stepperPos);   
       while (stepper.currentPosition() != stepperPos - 100) // Full speed up to 300
    stepper.run();
    
         delay(30); 
    }
  }
  else
  {
     delay(30);   
  }
  
    if ( t< millis()){
     
         stepper.moveTo(100);
  while (stepper.currentPosition() != 90) // Full speed up to 300
    stepper.run();
  stepper.runToNewPosition(0); // Cause an overshoot then back to 0
  
        t = millis() + interval;
         Serial.println("up");
         delay(3000);
      }


}


