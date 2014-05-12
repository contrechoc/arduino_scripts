/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 */

int led1 = 11;          
int led2 = 10;  
int led3 = 9;          
int led4 = 6; 
int led5 = 5;          
int led6 = 3; 

int brightness = 0;    // how bright the LED is
int fadeAmount = 2;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup()  { 
  // declare pin 9 to be an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT); 
  pinMode(led6, OUTPUT);

} 

// the loop routine runs over and over again forever:
void loop()  { 
  // set the brightness of pin 9:
  analogWrite(led1, brightness); 
  analogWrite(led2, brightness); 
  analogWrite(led3, brightness); 
  analogWrite(led4, brightness); 
  analogWrite(led5, brightness);  
  analogWrite(led6, brightness);  

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness <= 0 || brightness > (255 - fadeAmount)) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(10);                            
}


