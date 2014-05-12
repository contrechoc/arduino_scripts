
const int ledPin1 =  13;      // the number of the LED pin
const int ledPin2 =  12;      // the number of the LED pin

const int ledPin3 =  11;      // the number of the LED pin
const int ledPin4 =  10;      // the number of the LED pin

// Variables will change:
int ledState1 = LOW;             // ledState used to set the LED
long previousMillis1 = 0;        // will store last time LED was updated

int ledState2 = LOW;             // ledState used to set the LED
long previousMillis2 = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval1 = 3*1000;           // interval at which to blink (milliseconds)
long interval2 = 4*1000;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);  
  pinMode(ledPin2, OUTPUT);  
 pinMode(ledPin3, OUTPUT);  
  pinMode(ledPin4, OUTPUT);    

}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis1 > interval1) {
    // save the last time you blinked the LED 
    previousMillis1 = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState1 == LOW){
      ledState1 = HIGH;
      digitalWrite(ledPin1, HIGH);
      delay(100);
      digitalWrite(ledPin1, LOW);
    }
    else{
      ledState1 = LOW;
      digitalWrite(ledPin2, HIGH);
      delay(100);
      digitalWrite(ledPin2, LOW);
    }
  }

  if(currentMillis - previousMillis2 > interval2) {
    // save the last time you blinked the LED 
    previousMillis2 = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState2 == LOW){
      ledState2 = HIGH;
      digitalWrite(ledPin3, HIGH);
      delay(100);
      digitalWrite(ledPin3, LOW);
    }
    else{
      ledState2 = LOW;
      digitalWrite(ledPin4, HIGH);
      delay(100);
      digitalWrite(ledPin4, LOW);
    }
  }
}


