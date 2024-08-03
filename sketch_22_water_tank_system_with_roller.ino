#include <Servo.h>

// Pin Definitions
const int trigPin = 9;
const int echoPin = 10;
const int leftSwitchPin = 8;
const int rightSwitchPin = 7;
const int waterValvePin = 6;
const int waterPumpPin = 5;
const int rollerMotorPin = 11;

// Variables
long duration;
int distance;

// Initialize Servo
Servo rollerMotor;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Setup ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setup roller sensor switch pins
  pinMode(leftSwitchPin, INPUT);
  pinMode(rightSwitchPin, INPUT);

  // Setup water valve and pump pins
  pinMode(waterValvePin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);

  // Attach Servo motor
  rollerMotor.attach(rollerMotorPin);

  // Ensure initial state
  digitalWrite(waterPumpPin, LOW); // Pump OFF
  digitalWrite(waterValvePin, LOW); // Water out OFF
  rollerMotor.write(90); // Roller motor OFF (Neutral Position)
}


void loop() {
  distance = getDistance();

  bool leftSwitchState = digitalRead(leftSwitchPin);
  bool rightSwitchState = digitalRead(rightSwitchPin);

  // When water level is high
  if (distance >= 80 && leftSwitchState == HIGH) {
    delay(5000); // Wait 5 seconds
    digitalWrite(waterPumpPin, LOW); // Turn off the water pump
    digitalWrite(waterValvePin, LOW); // Ensure water out is OFF
    delay(5000); // Wait 5 seconds
    rollerMotor.write(180); // Roller motor FORWARD 
    // happen automatically : leftSwitchState == LOW;
  }

  // When roller sensor switch is ON to the RIGHT
  if (rightSwitchState == HIGH) {
    rollerMotor.write(90); // Stop the roller motor
    delay(5000);
    digitalWrite(waterPumpPin, LOW); // Ensure water pump is off
    digitalWrite(waterValvePin, HIGH); // Open the water valve 
  }

  // When water level is low
  if (distance <= 20 && rightSwitchState == HIGH) {
    delay(5000); // Wait 5 seconds
    digitalWrite(waterValvePin, LOW); // Close the water valve
    delay(5000); // Wait 5 seconds
    rollerMotor.write(0); // Roller motor BACKWARD 
    // happen automatically : rightSwitchState == LOW;
  }

  // When roller sensor switch is ON to the LEFT
  if (leftSwitchState == HIGH) {
    rollerMotor.write(90); // Stop the roller motor
    delay(5000); // Wait 5 seconds
    digitalWrite(waterPumpPin, HIGH); // Turn on the water pump
  }
}

// Function to get the distance from the ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  distance = map(distance, 0, 800, 0, 100);
  return distance;
}
