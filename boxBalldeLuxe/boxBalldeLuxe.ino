#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };


int pos = 0;    // variable to store the servo position 


void setup(){

  //LED afdeling
  pinMode( 3, OUTPUT);//GND
  digitalWrite(3, LOW);
  pinMode( 4, OUTPUT);//GND
  digitalWrite(4, LOW);
  pinMode( 5, OUTPUT);//GND
  digitalWrite(5, LOW);
  pinMode( 6, OUTPUT);//GND
  digitalWrite(6, LOW);
  pinMode( 7, OUTPUT);//GND
  digitalWrite(7, LOW);

  //knop
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

  //sound
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
}

void loop(){


  digitalWrite( 3, LOW);
  digitalWrite( 4, LOW);
  digitalWrite( 5, LOW);
  digitalWrite( 6, LOW);
  digitalWrite( 7, LOW);


  if ( digitalRead( 13, HIGH)){
    teller= teller + 1;
    if ( teller == 0) allesUit();
    if ( teller > 0) digitalWrite( 3, HIGH);
    if ( teller > 1) digitalWrite( 4, HIGH);
    if ( teller > 2) digitalWrite( 5, HIGH);
    if ( teller > 3) digitalWrite( 6, HIGH);
    if ( teller > 4) {
      digitalWrite( 7, HIGH);
      soundOn();
    }
  }

  if ( teller > 5 ) teller = 0;

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


