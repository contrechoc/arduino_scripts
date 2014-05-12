#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };


#include <Servo.h> 

Servo myservo1;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 
Servo myservo2; 


int pos = 0;    // variable to store the servo position 
int teller = 0;


void setup(){

  //LED afdeling
  pinMode( 7, OUTPUT);//7 LED1
  pinMode( 6, OUTPUT);//GND
  digitalWrite(6, LOW);
  pinMode( 5, OUTPUT);//7 LED2

  //knop
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

  //sound
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

//ldr
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);
 digitalWrite(14, LOW);
 digitalWrite(16, HIGH);

  myservo1.attach(9);
  myservo2.attach(10);
  
  Serial.begin(9600);
  
 
}



void loop(){

  if ( digitalRead( 13) == HIGH){
    
   for(int i = 0; i<10 ; i++)
    {
      tone(3, 1000 + 100*i, 300);
      delay(100);
    }
     
     lampjesAan();
     soundOn();
      armpjes();
   
  }
  else
  {
    allesUit();
  }

int sensorReading =  analogRead(A1) ;

 if ( teller < 5){
  // play the pitch:
  tone(3, sensorReading*3, 100);
  teller++;
 }
  
  if ( sensorReading < 150  ) 
  {
    
    teller = 0;
    //LED 10 seconden aan
    lampjesAan();
    delay(500);
  }
  else
  {
    lampjesUit();
    
  }
}
    

  void allesUit(){
    soundOff();
    armpjesOff();
    lampjesUit();
  }

void soundOn(){

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(3, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}
void soundOff(){
  //kan later nog meer....

}

void armpjesOff(){
  
}

void armpjes(){

  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo1.write(pos);              // tell servo to go to position in variable 'pos' 
    myservo2.write(pos);  
     tone(3, 1000 + pos*10, 300);
    
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo1.write(pos);              // tell servo to go to position in variable 'pos' 
    myservo2.write(pos);  
     tone(3, 1000 + pos*10, 300);
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 


}

void lampjesAan(){
  digitalWrite(7, HIGH);
  digitalWrite(5, HIGH);
 

}

void lampjesUit(){

  digitalWrite(7, LOW);
  digitalWrite(5, LOW);
  delay(100);


}



