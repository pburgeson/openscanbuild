#include <AccelStepper.h>

AccelStepper stepper(1, 6, 7);
const int buttonPin = 3;
int buttonState = 0;
void setup() {
   // set up motor
   stepper.setEnablePin(5);
   stepper.setMaxSpeed(1600);
   stepper.setAcceleration(1600);
   stepper.setPinsInverted(true, false, true);
   stepper.moveTo(200000);

   pinMode(buttonPin, INPUT);
}

void loop() {
  // disable outputs is button pressed
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    stepper.disableOutputs();
  }
  else {
    stepper.enableOutputs();
  }
  
  // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());
    stepper.run();
}
