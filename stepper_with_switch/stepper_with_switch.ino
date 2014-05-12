 
#include <Stepper.h>
 
int pos = 0;    // variable to store the servo position 

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 7,6,5,4 );
//Stepper stepper(STEPS, 7,6,8,4 );
// the previous reading from the analog input
int previous = 0;
volatile int switch1 = 0;
volatile int switch2 = 0;

int timer1_counter;

void setup()
{
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(50);
  pinMode ( 14,INPUT_PULLUP);
  pinMode ( 15,INPUT_PULLUP);

  pinMode ( 8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode ( 9, OUTPUT);
  digitalWrite(9, LOW);

  digitalWrite(8, HIGH);   
  delay(200);
  digitalWrite(8, LOW);   
  delay(200);
  digitalWrite(9, HIGH);   
  delay(200);
  digitalWrite(9, LOW);   
  delay(200);
  digitalWrite(8, HIGH);   
  delay(200);
  digitalWrite(8, LOW);   
  delay(200);
  digitalWrite(9, HIGH);   
  delay(200);
  digitalWrite(9, LOW);   
  delay(200);
 
    // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286/2;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
   if ( switch2 == 1 ){
  if ( switch1 == 1 ){
  digitalWrite(8, digitalRead(8) ^ 1);
  }
  else{
   digitalWrite(9, digitalRead(9) ^ 1);
  }
   }
}

 

void loop()
{
  digitalWrite(8, LOW);
   digitalWrite(9, LOW);
 switch2 = 0;
 
    if ( (digitalRead(14) == HIGH )&& (switch1 ==0) ){
      switch2 = 1;
      switch1 =1;
      digitalWrite(8, HIGH);
      delay(200);
      digitalWrite(8, LOW);
     // stepper.setSpeed(5);
      stepper.step(-100*4.1);
      delay(50);
      
    }
    else if ( (digitalRead(15) == HIGH )&& (switch1 ==1) ){
        switch2 = 1;
      switch1 =0;
      digitalWrite(9, HIGH);
      delay(200);
      digitalWrite(9, LOW);
      //  stepper.setSpeed(1);
      stepper.step(4.1*100);
      delay(50);
     
    }
    delay(10);
 

  // if ( switch1 == 0 )
  //    stepper.step(200);


}

