/*
 * By Mike Cook April 2009
 * Three RFID readers outputing 26 bit Wiegand code to pins:-
 * Reader A  Pins 4 & 5
 * Reader B  Pins 6 & 7
 * Reader C  Pins 8 & 9
 * Interrupt service routine gathers Wiegand pulses (zero or one) until 26 have been recieved
 * Then a sting is sent to processing
 */
#include "pins_arduino.h"
/*
 * an extension to the interrupt support for arduino.
 * add pin change interrupts to the external interrupts, giving a way
 * for users to have interrupts drive off of any pin.
 * Refer to avr-gcc header files, arduino source and atmega datasheet.
 */

/*
 * Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
 * The PCINT corresponding to the pin must be enabled and masked, and
 * an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
 * must use some logic to actually implement a per-pin interrupt service.
 */

/* Pin to interrupt map:
 * D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
 * D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
 * A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
 */

volatile uint8_t *port_to_pcmask[] = {
  &PCMSK0,
  &PCMSK1,
  &PCMSK2
};


typedef void (*voidFuncPtr)(void);

volatile static voidFuncPtr PCintFunc[24] = {
  NULL };

volatile static uint8_t PCintLast[3];

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 * First version only supports CHANGE mode.
 */
 void PCattachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  uint8_t slot;
  volatile uint8_t *pcmask;

  if (mode != CHANGE) {
    return;
  }
  // map pin to PCIR register
  if (port == NOT_A_PORT) {
    return;
  }
  else {
    port -= 2;
    pcmask = port_to_pcmask[port];
  }
  slot = port * 8 + (pin % 8);
  PCintFunc[slot] = userFunc;
  // set the mask
  *pcmask |= bit;
  // enable the interrupt
  PCICR |= 0x01 << port;
}

void PCdetachInterrupt(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *pcmask;

  // map pin to PCIR register
  if (port == NOT_A_PORT) {
    return;
  }
  else {
    port -= 2;
    pcmask = port_to_pcmask[port];
  }

  // disable the mask.
  *pcmask &= ~bit;
  // if that's the last one, disable the interrupt.
  if (*pcmask == 0) {
    PCICR &= ~(0x01 << port);
  }
}

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
static void PCint(uint8_t port) {
  uint8_t bit;
  uint8_t curr;
  uint8_t mask;
  uint8_t pin;

  // get the pin states for the indicated port.
  curr = *portInputRegister(port+2);
  mask = curr ^ PCintLast[port];
  PCintLast[port] = curr;
  // mask is pins that have changed. screen out non pcint pins.
  if ((mask &= *port_to_pcmask[port]) == 0) {
    return;
  }
  // mask is pcint pins that have changed.
  for (uint8_t i=0; i < 8; i++) {
    bit = 0x01 << i;
    if (bit & mask) {
	pin = port * 8 + i;
	if (PCintFunc[pin] != NULL) {
	  PCintFunc[pin]();
	}
    }
  }
}

SIGNAL(PCINT0_vect) {
  PCint(0);
}
SIGNAL(PCINT1_vect) {
  PCint(1);
}
SIGNAL(PCINT2_vect) {
  PCint(2);
}

// End of interrupts code and start of the reader code

volatile long reader1 = 0,reader2 = 0, reader3 = 0;
volatile int reader1Count = 0, reader2Count = 0,  reader3Count = 0;

void reader1One(void) {
  if(digitalRead(4) == LOW){
  reader1Count++;
  reader1 = reader1 << 1;
  reader1 |= 1;
  }
}

void reader1Zero(void) {
  if(digitalRead(5) == LOW){
  reader1Count++;
  reader1 = reader1 << 1;  
  }
}

void reader2One(void) {
  if(digitalRead(6) == LOW){
  reader2Count++;
  reader2 = reader2 << 1;
  reader2 |= 1;
  }
}

void reader2Zero(void) {
  if(digitalRead(7) == LOW){
  reader2Count++;
  reader2 = reader2 << 1;  
  }
}

void reader3One(void) {
  if(digitalRead(8) == LOW){
  reader3Count++;
  reader3 = reader3 << 1;
  reader3 |= 1;
  }
}

void reader3Zero(void) {
  if(digitalRead(9) == LOW){
  reader3Count++;
  reader3 = reader3 << 1;  
  }
}

void setup()
{
  Serial.begin(9600);
  // Attach pin change interrupt service routines from the Wiegand RFID readers
  PCattachInterrupt(4, reader1One, CHANGE);
  PCattachInterrupt(5, reader1Zero, CHANGE);
  PCattachInterrupt(6, reader2One, CHANGE);
  PCattachInterrupt(7, reader2Zero, CHANGE);
  PCattachInterrupt(8, reader3One, CHANGE);
  PCattachInterrupt(9, reader3Zero, CHANGE);
  delay(10);
  // the interrupt in the Atmel processor mises out the first negitave pulse as the inputs are already high,
  // so this gives a pulse to each reader input line to get the interrupts working properly.
  // Then clear out the reader variables.
  // The readers are open collector sitting normally at a one so this is OK
  for(int i = 4; i<10; i++){
  pinMode(i, OUTPUT);
   digitalWrite(i, HIGH); // enable internal pull up causing a one
  digitalWrite(i, LOW); // disable internal pull up causing zero and thus an interrupt
  pinMode(i, INPUT);
  digitalWrite(i, HIGH); // enable internal pull up
  }
  delay(10);
  // put the reader input variables to zero
  reader1 = reader2 = reader3 = 0;
  reader1Count = reader2Count =  reader3Count = 0;
  digitalWrite(13, HIGH);  // show Arduino has finished initilisation
}

void loop() {
  if(reader1Count >= 26){
//  Serial.print(" Reader 1 ");Serial.println(reader1,HEX);
 // Serial.print("A");
  Serial.println(reader1 & 0xfffffff);
  reader1 = 0;
  reader1Count = 0;
     }
    
  if(reader2Count >= 26){
  Serial.println("B");Serial.println(reader2 & 0xfffffff);
  reader2 = 0;
  reader2Count = 0;
     }
    
 if(reader3Count >= 26){
  Serial.println("C");Serial.println(reader3 & 0xfffffff);
  reader3 = 0;
  reader3Count = 0;
     }
    
}
 

