/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
 // NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 300

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//int delayval = 500; // delay for half a second

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPinRed = A0;    // Analog input pin that the potentiometer is attached to
const int analogInPinGreen = A1;  // Analog input pin that the potentiometer is attached to
const int analogInPinBlue = A2;   // Analog input pin that the potentiometer is attached to

const int toggleInPin = 6;     // toggle to set mode

const int aheadInPin = 9;     // the number of the pushbutton pin
const int backInPin = 11;     // the number of the pushbutton pin


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
  
  //Change the overall brightness due to power requirements
  strip.setBrightness(255/5);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  
  // initialize the pushbutton pins as an input:
  pinMode(aheadInPin, INPUT);
  pinMode(backInPin, INPUT);  
  pinMode(toggleInPin, INPUT);
  
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
  // ToDo: Do something 
    
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
    case 1:    // your hand is on the sensor
      Serial.println("dark");
      break;
    case 2:    // your hand is close to the sensor
      Serial.println("dim");
      break;
    case 3:    // your hand is a few inches from the sensor
      Serial.println("medium");
      break;
    case 4:    // your hand is nowhere near the sensor
      Serial.println("bright");
      break;
    }  

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

void loop() {
  // read the analog in value:
  sensorValueRed = analogRead(analogInPinRed);
  sensorValueGreen = analogRead(analogInPinGreen);
  sensorValueBlue = analogRead(analogInPinBlue);


  toggleButtonState = digitalRead(toggleInPin);
  if( toggleButtonState == 1 ) { 
    ModePatterns(); 
  } else {
     ModeRGB(); 
  }
   
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    
   // enable if you want it to light up one LED at a time
   //delay(wait);
  }
  strip.show();
}

