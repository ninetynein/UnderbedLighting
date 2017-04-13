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

const int toggleInPin = 5;     // toggle to set mode

const int aheadInPin = 9;     // the number of the pushbutton pin
const int backInPin = 11;     // the number of the pushbutton pin


int sensorValueRed = 0;        // value read from the pot
int sensorValueGreen = 0;        // value read from the pot
int sensorValueBlue = 0;        // value read from the pot

int outputValueRed = 0;        // value output to the PWM (analog out)
int outputValueGreen = 0;        // value output to the PWM (analog out)
int outputValueBlue = 0;        // value output to the PWM (analog out)

int aheadButtonState = 0;
int backButtonState = 0;
int toggleButtonState = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  //Change the overall brightness due to power requirements
  strip.setBrightness(255/4);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  
  // initialize the pushbutton pins as an input:
  pinMode(aheadInPin, INPUT);
  pinMode(backInPin, INPUT);  
  pinMode(toggleInPin, INPUT);
  
}

void ModeRGB() {
  
  // map it to the range of the analog out:
  outputValueRed = map(sensorValueRed, 0, 1023, 0, 254);
  outputValueGreen = map(sensorValueGreen, 0, 1023, 0, 254);
  outputValueBlue = map(sensorValueBlue, 0, 1023, 0, 254);
  
  colorWipe(strip.Color(outputValueRed, outputValueGreen, outputValueBlue), 10);
   
  // print the results to the serial monitor:
  Serial.print("Red = " );
  Serial.print(outputValueRed);
  Serial.print("\t Green = " );
  Serial.print(outputValueGreen);
  Serial.print("\t Blue = " );
  Serial.print(outputValueBlue);
/*  
  Serial.print("\t Toggle = " );
  Serial.print(toggleButtonState);
  Serial.print("\t ahead = " );
  Serial.print(aheadButtonState);
  Serial.print("\t back = " );
  Serial.print(backButtonState);
  */
  Serial.println(); 
}

void ModePatterns() {
  // ToDo: Do something 
  
  
  colorWipe(strip.Color(255, 0, 0), 10);
  
}

void loop() {
  // read the analog in value:
  sensorValueRed = analogRead(analogInPinRed);
  sensorValueGreen = analogRead(analogInPinGreen);
  sensorValueBlue = analogRead(analogInPinBlue);
  
  // read the state of the pushbutton value:
  aheadButtonState = digitalRead(aheadInPin);
  backButtonState = digitalRead(backInPin);

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
   //delay(wait);
  }
  strip.show();
}
