/*
This is a device that controls an LED strip.  The device has a toggle switch, three pots, and two buttons.
The toggle switch switches between modes.  
Mode 1 (toggle to the left) is RGB mode, where each pot controls the red green and blue value.  The buttons 
are ignored in Mode 1
Mode 2 (toggle to the right) is Pattern mode.  The black button cycles through the patterns in reverse
and the red button cycles through the patterns forward.  The pots are ignored in this mode.

The lights are a 5 meter strip of 60 LEDs/strip
The circuit:
 * Center pin of each potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
   Red potentiometer connected to analog pin 0.
   Green pot connected to analog pin 1
   Red pot connected to analog pin 2
 * LED strip connected to digital pin 4
 * Ahead button is connected to pin 11
 * Back button is connected to pin 9
 
Power supply is a 5 volt, 2.4 amp wall wort
 

 created April 2, 2017
 by Tyson Haverkort and Steven Smethurst

This code uses the NeoPixel Library and snipits from the Neopixel strand test cod (c) 2013 Shae Erisson
released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

 */


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 4

// How many NeoPixels are attached to the Arduino?  Pixels per metre (60) x number of metres (5)
#define NUMPIXELS 300

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


// These constants won't change.  They're used to give names
// to the pins used:
// RGB pots
const int analogInPinRed = A0;    // Analog input pin that the potentiometer is attached to
const int analogInPinGreen = A1;  // Analog input pin that the potentiometer is attached to
const int analogInPinBlue = A2;   // Analog input pin that the potentiometer is attached to

// Toggle switch
const int toggleInPin = 6;     // toggle to set mode

// Ahead and back pattern buttons
const int aheadInPin = 9;     // pushbutton pin to cycle forward through patterns
const int backInPin = 11;     // pushbutton pin to cycle backwards through patterns


int sensorValueRed = 0;        // value read from the pot
int sensorValueGreen = 0;        // value read from the pot
int sensorValueBlue = 0;        // value read from the pot

int outputValueRed = 0;        // value output to the PWM (analog out)
int outputValueGreen = 0;        // value output to the PWM (analog out)
int outputValueBlue = 0;        // value output to the PWM (analog out)

int aheadButtonState = LOW;
int backButtonState = HIGH;
int toggleButtonState = HIGH;
int currentPattern = 0;          // value of the pattern when in pattern mode



void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  //Change the overall brightness due to power limitations
  strip.setBrightness(255/5);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  
  // initialize the pushbutton and toggle pins as an input:
  pinMode(aheadInPin, INPUT);
  pinMode(backInPin, INPUT);  
  pinMode(toggleInPin, INPUT);
  
}

void loop() {
  // read the analog in value:
  sensorValueRed = analogRead(analogInPinRed);
  sensorValueGreen = analogRead(analogInPinGreen);
  sensorValueBlue = analogRead(analogInPinBlue);

  // Check the toggle switch and set the mode
  toggleButtonState = digitalRead(toggleInPin);
  if( toggleButtonState == 1 ) { 
    ModePatterns(); 
  } else {
     ModeRGB(); 
  }
}

void ModeRGB() {
  // Set color based on POT values
  
  // map it to the range of the analog out:
  outputValueRed = map(sensorValueRed, 0, 1023, 0, 254);
  outputValueGreen = map(sensorValueGreen, 0, 1023, 0, 254);
  outputValueBlue = map(sensorValueBlue, 0, 1023, 0, 254);
  
  // set the color
  colorWipe(strip.Color(outputValueRed, outputValueGreen, outputValueBlue), 10);
}


void ModePatterns() {
  // Run a series of patterns.  Cycle through when a button is pressed
    
  // read the state of the pushbutton value:
  int aheadButtonState = digitalRead(aheadInPin);
  int backButtonState = digitalRead(backInPin);
  int lastaheadButtonState = LOW;   // the previous reading from the input pin
  int lastbackButtonState = HIGH;   // the previous reading from the input pin

  // the following variables are unsigned long's because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  unsigned long lastaheadDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long lastbackDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 1000;    // the debounce time; increase if the output flickers
  
  // set the color to red to initialize
  colorWipe(strip.Color(254, 0, 0), 10);
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (aheadButtonState!= lastaheadButtonState){
    // reset the debouncing timer
    lastaheadDebounceTime = millis();
  }
  Serial.print("\t Ahead Debounce Time = " );
  Serial.print(lastaheadDebounceTime);
  Serial.println();
  
  if ((millis() - lastaheadDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (aheadButtonState != lastaheadButtonState) {
      lastaheadButtonState = aheadButtonState;
  
      if ((aheadButtonState == HIGH) && (currentPattern <= 3)){ //from 4...
        currentPattern = (currentPattern + 1);
      }
    }
    
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastaheadButtonState = aheadButtonState;
  
 // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (backButtonState!= lastbackButtonState){
    // reset the debouncing timer
    lastbackDebounceTime = millis();
  }
  Serial.print("\t Back Debounce Time = " );
  Serial.print(lastbackDebounceTime);
  Serial.println();
  
  if ((millis() - lastbackDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (backButtonState != lastbackButtonState) {
      lastbackButtonState = backButtonState;
      if ((backButtonState == LOW) && (currentPattern >= 2)){  //to 0
        currentPattern = (currentPattern - 1);
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastbackButtonState = backButtonState;
   
  
  switch (currentPattern) {
    case 1:    // Pattern 1
      Serial.println("Pattern 1");
      break;
    case 2:    // Pattern 2
      Serial.println("Pattern 2");
      break;
    case 3:    // Pattern 3
      Serial.println("Pattern 3");
      break;
    case 4:    // Pattern 4
      Serial.println("Pattern 4");
      break;
    }  

  // print out the case for debugging
  Serial.print("\t Toggle = " );
  Serial.print(toggleButtonState);
  Serial.print("\t ahead = " );
  Serial.print(aheadButtonState);
  Serial.print("\t Lastahead = " );
  Serial.print(lastaheadButtonState);
  Serial.print("\t back = " );
  Serial.print(backButtonState);
  Serial.print("\t Lastback = " );
  Serial.print(lastbackButtonState);
  Serial.print("\t Current Pattern = " );
  Serial.print(currentPattern);
  Serial.println();
 
}

// TODO: Add pattern functions
// Pattern 1: Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    
   // enable if you want it to light up one LED at a time
   //delay(wait);
  }
  strip.show();
}

