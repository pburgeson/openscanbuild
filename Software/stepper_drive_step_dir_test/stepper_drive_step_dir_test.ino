/*
Initial testing of Trinamic stepper driver (TMC2208) silent step stick.  Goal of this 
test is to successfully drive a stepper motor with the most basic software possible.
*/
#define enable_pin 5
#define step_pin 6
#define dir_pin 7
#define step_period 100  // in microseconds

void setup() {
  // intitize pins
  pinMode(enable_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // enable motor
  digitalWrite(enable_pin, LOW);
  
  // set direction
  digitalWrite(dir_pin, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(step_pin, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(step_period); 
  digitalWrite(step_pin, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(step_period);
}
