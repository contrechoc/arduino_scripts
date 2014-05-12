#include <avr/io.h>
#include <avr/interrupt.h>


const byte LED = 6;

	uint8_t pwm = 0x00;
bool up = true;

void setup() {
  pinMode (9, OUTPUT);
  pinMode (8, OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(9, digitalRead(9) ^ 1);
  delay(100);
  digitalWrite(9, digitalRead(9) ^ 1);
  delay(100);

  // initialize Timer1
  cli();             // disable global interrupts
  TCCR2A = 0;        // set entire TCCR1A register to 0
  TCCR2B = 0;

  // enable Timer1 overflow interrupt:
  TIMSK2 = (1 << TOIE2);
  // Set CS10 bit so timer runs at clock speed:
  TCCR2B |=  (1 << CS22)|(1 << CS20)|(1 << CS21);
  // enable global interrupts:
  sei();
}

void loop ()
{
  // Main loop
}


ISR(TIMER2_OVF_vect)
{
 
  pwm += up ? 1 : -1;
  if (pwm == 0xff)
    up = false;
  else if (pwm == 0x00)
    up = true;
    
    analogWrite(9,pwm);

}




