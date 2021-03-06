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
// const int PIN_ANALOG_RED = A0;    // Analog input pin that the potentiometer is attached to
const int analogInPinRed = A0;    // Analog input pin that the potentiometer is attached to
const int analogInPinGreen = A1;  // Analog input pin that the potentiometer is attached to
const int analogInPinBlue = A2;   // Analog input pin that the potentiometer is attached to

// Toggle switch
const int toggleInPin = 6;     // toggle to set mode

// Ahead and back pattern buttons
const int aheadInPin = 11;     // pushbutton pin to cycle forward through patterns
const int backInPin = 9;     // pushbutton pin to cycle backwards through patterns


int sensorValueRed = 0;        // value read from the pot
int sensorValueGreen = 0;        // value read from the pot
int sensorValueBlue = 0;        // value read from the pot

int outputValueRed = 0;        // value output to the PWM (analog out)
int outputValueGreen = 0;        // value output to the PWM (analog out)
int outputValueBlue = 0;        // value output to the PWM (analog out)

int toggleButtonState = HIGH;
int buttonStateForward = HIGH;
int buttonStateBack = HIGH;
  
int currentPattern = 0;          // value of the pattern when in pattern mode
const int BUTTON_STATE_FORWARD_UP = HIGH;
const int BUTTON_STATE_FORWARD_DOWN = LOW;

const int BUTTON_STATE_BACKWARD_UP = LOW;
const int BUTTON_STATE_BACKWARD_DOWN = HIGH;

const int TOGLE_STATE_PATTERN = HIGH;
const int TOGLE_STATE_RGB = LOW;


const int PATTERN_COLOR_WIPE_RED = 0 ; 
const int PATTERN_COLOR_WIPE_GREEN = 1 ; 
const int PATTERN_COLOR_WIPE_BLUE = 2 ; 
const int PATTERN_RAINBOW = 3 ; 
const int PATTERN_RAINBOW_CYCLE = 4 ;
const int PATTERN_THEATER_CHASE_RED = 5 ;
const int PATTERN_THEATER_CHASE_GREEN = 6 ;
const int PATTERN_THEATER_CHASE_BLUE = 7 ;
const int PATTERN_THEATER_CHASE_RAINBOW = 8 ;
const int MAX_PATTERN = PATTERN_THEATER_CHASE_RAINBOW ; 


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
  toggleButtonState = digitalRead(toggleInPin);
  buttonStateForward = digitalRead(aheadInPin);
  buttonStateBack = digitalRead(backInPin);
  debugPrint(); 
  
  // Check the toggle switch and set the mode

  if( toggleButtonState == TOGLE_STATE_PATTERN ) { 
    ModePatterns(); 
  } else {
     ModeRGB(); 
  }
}

void debugPrint() 
{  
  // print out the case for debugging
  Serial.print("Toggle = " );
  Serial.print(toggleButtonState == TOGLE_STATE_PATTERN ? "Pattern" : "RGB");
  Serial.print("  ahead = " );
  Serial.print(buttonStateForward == BUTTON_STATE_FORWARD_UP ? "Up" : "Down" );
  Serial.print("  back = " );
  Serial.print(buttonStateBack == BUTTON_STATE_BACKWARD_UP ? "Up" : "Down");
  Serial.print("  currentPattern = " );
  Serial.print(currentPattern);
  

  
  Serial.print("  Red Pot = " );
  Serial.print(sensorValueRed);
  Serial.print("  Green Pot = " );
  Serial.print(sensorValueGreen);
  Serial.print("  Blue Pot = " );
  Serial.print(sensorValueBlue);
  
  Serial.println();
  
  
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


void ModePatterns() 
{
  // Check to see if the forward or back button has changed, and if we are chaning the pattern.
  
  static int lastBackButtonState = BUTTON_STATE_BACKWARD_UP ; 
  if( lastBackButtonState != buttonStateBack ) {
    lastBackButtonState = buttonStateBack ; 
    if( buttonStateBack == BUTTON_STATE_BACKWARD_DOWN )  {
      currentPattern-- ; 
    }
  }
  static int lastForwardButtonState = BUTTON_STATE_FORWARD_UP ; 
  if( lastForwardButtonState != buttonStateForward ) {
    lastForwardButtonState = buttonStateForward ; 
    if( buttonStateForward == BUTTON_STATE_FORWARD_DOWN )  {
      currentPattern++ ; 
    }
  }
    
  // Check the ranges of the current patter to make sure that it doesn't go out of bounds
  if( currentPattern > MAX_PATTERN ) { 
    currentPattern = MAX_PATTERN ; 
  } else if(currentPattern < 0 ) { 
    currentPattern = 0 ; 
  }
  
  
  // Depending on the current pattern show a pattern 
  
  switch(currentPattern)  
  {
    case PATTERN_COLOR_WIPE_RED:
    {
      patternColorWipeRed(); 
      break ; 
    }
    case PATTERN_COLOR_WIPE_GREEN:
    {
      patternColorWipeGreen(); 
      break ; 
    }
    case PATTERN_COLOR_WIPE_BLUE:
    {
      patternColorWipeBlue(); 
      break ; 
    }
    case PATTERN_RAINBOW: 
    {
      patternRainbow(20); 
      break;
    }
    case PATTERN_RAINBOW_CYCLE: 
    {
      patternRainbowCycle(20); 
      break;
    }
    case PATTERN_THEATER_CHASE_RED: 
    {
      patternTheaterChase(strip.Color(127, 0, 0), 50); // Red
      break;
    }
    case PATTERN_THEATER_CHASE_GREEN:
    {
      patternTheaterChase(strip.Color(0, 127, 0), 50); // Green
      break;
    }

    case PATTERN_THEATER_CHASE_BLUE: 
    {
      patternTheaterChase(strip.Color(0, 0, 127), 50); // Blue; 
      break;
    }

    case PATTERN_THEATER_CHASE_RAINBOW: 
    {
      patternTheaterChaseRainbow(20); 
      break;
    }
  }
  
  return ;   
}
// Pattern 1 red increase
void patternColorWipeRed() {
  static unsigned char hue = 0 ; 
  colorWipe(strip.Color(hue, 0, 0), 5);  
  hue += 20 ; 
}

// Pattern 2 Green increase
void patternColorWipeGreen() {
  static unsigned char hue = 0 ; 
  colorWipe(strip.Color(0,hue, 0), 5);  
  hue += 20 ; 
}

// Pattern 3 Blue increase
void patternColorWipeBlue() {
  static unsigned char hue = 0 ; 
  colorWipe(strip.Color(0, 0, hue), 5);  
  hue += 20 ; 
}

// Patterns 1-3: color wipes Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    
   // enable if you want it to light up one LED at a time
   //delay(wait);
  }
  
  strip.show();
}

// Pattern 4 rainbow
void patternRainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
//
//
    strip.show();

  }
}

// Pattern 5 Slightly different, this makes the rainbow equally distributed throughout
void patternRainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  }
}

//Patterns 6-8 Theatre-style crawling lights.
void patternTheaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Pattern 9 Theatre-style crawling lights with rainbow effect
void patternTheaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
