/* 
 * Dual stepper motor control.
 *
 * Takes a direction and drive input for each of two motors.
 * If drive is high, the motor is driven according to
 * direction: +ve steps if true, -ve steps if false.
 * The speed (in rpm) of each motor can also be set by passing
 * a pair of comma-separated integers values over the USB 
 * serial link, using 0 to indicate no change e.g.:
 *    60,30   to set the 60rpm on A and 30rpm on B;
 *    0, 30   to set 30rpm on B, leaving A as it is.
 *
 * Mick Phillips, 2014.
 */

#include <Stepper.h>

// Serial timeout.
#define SERIAL_TIMEOUT 100

// *** Parameters for each motor ***
// Number of steps per revolution.
#define STEPS_A 128
#define STEPS_B 128
// The maximum motor speed (rpm).
#define MAX_SPEED_A 200
#define MAX_SPEED_B 200
// The control pins.
#define DRIVE_A_PIN 2
#define DIRECTION_A_PIN 3
#define DRIVE_B_PIN 8
#define DIRECTION_B_PIN 9


// Stepper instances for motors A and B.
// Steppter(STEPS, pin1, pin2, pin3, pin4)
Stepper stepper_a(STEPS_A, 4, 6, 5, 7);
Stepper stepper_b(STEPS_B, 10, 12, 11, 13);

// Direction inputs for each motor.
boolean direction_a = true;
boolean direction_b = true;

// Speed inputs for each motor (rpm)
int speed_a = 60;
int speed_b = 60;

// Arduino setup.
void setup()
{
  // Set control lines as inputs.
  pinMode(DRIVE_A_PIN, INPUT);
  pinMode(DIRECTION_A_PIN, INPUT);
  pinMode(DRIVE_B_PIN, INPUT);
  pinMode(DIRECTION_B_PIN, INPUT);
  
  // Set a speed for the motors (rpm).
  stepper_a.setSpeed(speed_a);
  stepper_b.setSpeed(speed_b);

  // Initialise the serial comms.
  Serial.begin(9600);
  Serial.setTimeout(SERIAL_TIMEOUT);
}


// Main loop.
void loop()
{
  if (Serial.available() >0) {
    speed_a = Serial.parseInt();
    speed_b = Serial.parseInt();
    // Ignore anything other than a pair of valid ints.
    if (speed_a > 0 && speed_b > 0) {
      speed_a = constrain(speed_a, 0, MAX_SPEED_A);
      speed_b = constrain(speed_b, 0, MAX_SPEED_B);
      
      // A speed of 0 means don't change the speed.
      if (speed_a != 0) {
        stepper_a.setSpeed(speed_a);
      }
      if (speed_b != 0) {
        stepper_b.setSpeed(speed_b);
      }
    }
  }

  // Check the state of the direction pins.
  direction_a = digitalRead(DIRECTION_A_PIN);
  direction_b = digitalRead(DIRECTION_B_PIN);


  // Step each motor if its drive line is high.
  // Note that we can only move one motor at a time using
  // the Stepper library, because Stepper.step blocks.
  if (digitalRead(DRIVE_A_PIN)) {
    stepper_a.step(10 * (direction_a ? 1 : -1));
  }
  if (digitalRead(DRIVE_B_PIN)) {
    stepper_b.step(10 * (direction_b ? 1 : -1));
  }  
}
