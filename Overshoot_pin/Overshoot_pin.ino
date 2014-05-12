// Overshoot.pde
// -*- mode: C++ -*-
//
// Check overshoot handling
// which sets a new target position and then waits until the stepper has 
// achieved it. This is used for testing the handling of overshoots
//
// Copyright (C) 2009 Mike McCauley
// $Id: Overshoot.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
 
AccelStepper stepper = AccelStepper(); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

char onOff = 0;

void setup()
{  
 
  stepper.setMaxSpeed(150);
  stepper.setAcceleration(100);
  pinMode(13, OUTPUT);
}

void loop()
{    
  stepper.moveTo(500);
  while (stepper.currentPosition() != 300) // Full speed up to 300
    stepper.run();
  stepper.runToNewPosition(0); // Cause an overshoot then back to 0
  
  digitalWrite( 13, onOff );
  if( onOff == 0)
  onOff = 1;
  else
  onOff = 0;
 // delay(100);
}
