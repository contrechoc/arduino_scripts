#include <JeeLib.h>

void setup(){
  /*
    //initializing for wrist band --- and testing
   for (int i = 14 ; i < 20 ; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
   digitalWrite(14, LOW);
   
   delay(250);
   for (int i = 14 ; i < 20 ; i++){
    digitalWrite(i, LOW);
  }
  delay(250);
  for (int i = 14 ; i < 20 ; i++){
    allLOW2();
    digitalWrite(i, HIGH);
     delay(250);
  }
  Serial.begin(9600);
  */
}


void allLOW2(){ //wrist band
   for (int i = 14 ; i < 20 ; i++){
    digitalWrite(i, LOW);
  }
}

void pattern1(){
    for (int i = 15 ; i < 20 ; i++){
    allLOW2();
    digitalWrite(i, HIGH);
     delay(250);
  }
}

void pattern2(){
    for (int i = 19 ; i > 13 ; i--){
    allLOW2();
    digitalWrite(i, HIGH);
     delay(250);
  }
}

void pattern3(){
    allLOW2();
    digitalWrite(15, HIGH);
     digitalWrite(16, HIGH);
     delay(250);
       allLOW2();
    digitalWrite(16, HIGH);
     digitalWrite(17, HIGH);
     delay(250);
           allLOW2();
    digitalWrite(17, HIGH);
     digitalWrite(18, HIGH);
     delay(250);
                allLOW2();
    digitalWrite(18, HIGH);
     digitalWrite(19, HIGH);
     delay(250);

}

void pattern4(){
    allLOW2();
    digitalWrite(15, HIGH);
     digitalWrite(16, HIGH);
      digitalWrite(17, HIGH);
     delay(250);
       allLOW2();
    digitalWrite(17, HIGH);
     digitalWrite(18, HIGH);
      digitalWrite(19, HIGH);
     delay(250);
}

void loop(){
  Serial.println("loop");
  delay(500);
  /*
   for (int i = 0 ; i < 5 ; i++){
pattern1();
delay(500);
   }

 for (int i = 0 ; i < 5 ; i++){
pattern2();
delay(500);
 }

 for (int i = 0 ; i < 5 ; i++){
pattern3();
delay(500);
 }

 for (int i = 0 ; i < 5 ; i++){
pattern4();
delay(500);
 }

  */
}

