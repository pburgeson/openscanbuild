#include <AccelStepper.h>

AccelStepper stepper(1, 6, 7);

void setup() {
  
  // put your setup code here, to run once:
   stepper.setEnablePin(5);
   stepper.setMaxSpeed(500);
   stepper.setSpeed(25);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.runSpeed();
}
