#include <Servo.h> // Servo control library
#include <CMRI.h> // CMRI-JMRI library

// Servo motor global varibales

Servo crossing_gate; // Servo object being controlled
CMRI control; // JMRI object to add Arduino as a node
int crossing = 10; // the servo object location on board
int open = 0; // crossing gate fully open
int close = 130; // crossing gate fully closed

// Flashing LED global variables

int ledPin1 = 5; // LED 1 location on board
int ledPin2 = 6; // LED 2 location on board

// JMRI control global variables
int j_output = 12; // JMRI led table output 001
int trigger = 8; // read JMRI command (open / close)

//// variables will change:
//int buttonPin = 11;
//int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600, SERIAL_8N2); // Interface with JMRI
  crossing_gate.attach(crossing); // servo object attached to Arduino PWM output
  pinMode(ledPin1, OUTPUT); // LED 1 set as output
  pinMode(ledPin2, OUTPUT); // LED 2 set as output
  pinMode(j_output, OUTPUT); // Output generated using JMRI
  pinMode(trigger, INPUT); // trigger action for crossing gates (open / close)
}

// Flasing LED script

void led_flash() {
   // LED 1 Flash with fade
  // fade in from min to max in increments of 15 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 15) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin1, fadeValue);
    // wait for 20 milliseconds to see the dimming effect
    delay(20);
  }

  // fade out from max to min in increments of 15 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 15) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin1, fadeValue);
    // wait for 20 milliseconds to see the dimming effect
    delay(20);
  }

  // LED 2 Flash with fade
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 15) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin2, fadeValue);
    // wait for 20 milliseconds to see the dimming effect
    delay(20);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 15) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin2, fadeValue);
    // wait for 20 milliseconds to see the dimming effect
    delay(20);
  }
}

void close_gate(){
  led_flash(); // flash led
  crossing_gate.write(close); // close gates
}

void open_gate() {
  crossing_gate.write(open); // open gates
}

void loop() { // main loop

control.process(); // build a data packet

digitalWrite(j_output, control.get_bit(0)); // Train detection output from JMRI 001
  
  if ((digitalRead(trigger) == 1)) { // gates triggered close or open
  close_gate(); // flash lights, sound bell and close gates
  } 
  else {
  open_gate(); // open gates
  }
}
