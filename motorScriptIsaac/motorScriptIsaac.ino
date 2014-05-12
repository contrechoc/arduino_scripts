


//motor 1
const int motor1PinA = 13;    // H-bridge leg 1 (pin 2, 1A)
const int motor1PinB = 12;    // H-bridge leg 2 (pin 7, 2A)

//motor 2
const int motor2PinA = 11;    // H-bridge leg 1 (pin 2, 1A)
const int motor2PinB = 10;    // H-bridge leg 2 (pin 7, 2A)



const int ledPin = 13;      // LED

void setup() {

  // set all the other pins you're using as outputs:
  pinMode(motor1PinA, OUTPUT);
  pinMode(motor1PinB, OUTPUT);

  pinMode(motor2PinA, OUTPUT);
  pinMode(motor2PinB, OUTPUT);

Serial.begin(9600);
}

void loop(){

  motorNumber(motor1PinA, motor1PinB, 1);
  // motorNumber(motor2PinA, motor2PinB, 0);
  delay(2000);

  motorNumber(motor1PinA, motor1PinB, 0);
  delay(2000);

 motorNumberStop(motor1PinA, motor1PinB);
 delay(2000);
 


}



void motorNumberStop(int pinNumberA,int pinNumberB){
  
    digitalWrite( pinNumberA, LOW );
    digitalWrite( pinNumberB, LOW );
   Serial.println("stop");
  
}

void motorNumber(int pinNumberA,int pinNumberB , int upDown){
  if (upDown == 0 )
  { 
    digitalWrite( pinNumberA, HIGH );
    digitalWrite( pinNumberB, LOW );
    Serial.println("left");
  }
  else
  {
    digitalWrite( pinNumberA, LOW );
    digitalWrite( pinNumberB, HIGH );
      Serial.println("right");

  }

}




