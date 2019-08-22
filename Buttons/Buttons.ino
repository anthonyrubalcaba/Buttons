#include<Wire.h>
#include <FastLED.h>

// Button Cycle Mode #####################################
// Button cycles LED between Off > Red > Green > Blue
int pinButtonCycle = 0; 
#define pinLedCycle      2   // which pin your pixels are connected to
#define countLedsCycle    1   // how many LEDs you have
CRGB ledsCycle[countLedsCycle];
int currentColorCycle = 0;
const TProgmemPalette16 colorsCycle PROGMEM =
{
  CRGB::Black,
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue
};


// Button Gradual Mode #####################################
// Button powers up strip while pressed. Strip powers down while depressed. 
int pinButtonGrad = 1;
#define pinLedGrad 5
#define countLedsGrad 8
CRGB ledsGrad[countLedsGrad];
int currentColorGrad = 0;
const TProgmemPalette16 colorsGrad PROGMEM =
{
  CRGB::Black,
  CRGB::Blue,
  CRGB::Green,
  CRGB::Red
};
uint32_t tGrad;
uint32_t tPrevGrad;
int currentLevelGrad = 0;

// Pot Select Mode #####################################
// Potentiometer selects the color
int pinPot = 9;
#define pinLedPot 7
#define countLedsPot 7
CRGB ledsPot[countLedsPot];
int currentColorPot = 0;
float potVal = 0;
int potHue = 0;

// Stick Select Mode ###################################
int pinStickSelect = 11;
#define pinLedStick 12
#define countLedsStick 16
CRGB ledsStick[countLedsStick];
float stickValX = 0;
float stickValY = 0;


void setup() {
  delay( 2000 ); // power-up safety delay
  Serial.begin(9600);
  Serial.println("Setup");

  // Button Cycle #####################################
  pinMode(pinButtonCycle, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, pinLedCycle, RGB>(ledsCycle, countLedsCycle).setCorrection( TypicalLEDStrip );

  // Button Grad #####################################
  pinMode(pinButtonGrad, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, pinLedGrad, RGB>(ledsGrad, countLedsGrad).setCorrection( TypicalLEDStrip );

  // Pot Select ######################################
  FastLED.addLeds<WS2812B, pinLedPot, RGB>(ledsPot, countLedsPot).setCorrection( TypicalLEDStrip );

  // Stick Select ###################################
  FastLED.addLeds<WS2812B, pinLedStick, RGB>(ledsStick, countLedsStick).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness(255);
}


void loop() {

  // Button Cycle #####################################
  if (digitalRead(pinButtonCycle) == LOW) {
    currentColorCycle++;
    ledsCycle[0] = colorsCycle[currentColorCycle];
    FastLED.show();
    FastLED.delay(500);
    if(currentColorCycle > 3) {
      currentColorCycle = 0;  
    }
  }

  // Button Grad #####################################
  tGrad = millis();
  if((tGrad - tPrevGrad) > 100) {
    if(digitalRead(pinButtonGrad) == LOW) {
      // Increase
      if(currentColorGrad < 8) currentColorGrad++;
    } else {
      // Decrease to 0
      if(currentColorGrad > -1) currentColorGrad--;
    }
    for(int x = 0; x < countLedsGrad; x++) {
      if(x <= currentColorGrad) {
        ledsGrad[x] = colorsGrad[1];
      } else {
        ledsGrad[x] = colorsGrad[0];
      }
    }
    tPrevGrad = tGrad;
    FastLED.show();
  }

  // Pot Select #####################################
  if(true){
    potVal = analogRead(pinPot);
    potVal = constrain(potVal, 0, 255);
    potHue = (int)potVal;
    if(potHue < 10) potHue = 0;
    //Serial.println(potHue);
  
    if(potHue == 0) {
      for(int x=0; x<countLedsPot; x++) {
        ledsPot[x] = CRGB::Black;
      }
    }else {
      for(int x=0; x<countLedsPot; x++) {
        ledsPot[x].setHSV(potHue, 255, 255);
      }
    }
  }
  
  // Stick Select ##################################
  stickValX = analogRead(A1);
  stickValY = analogRead(A2);
  float angle = atan2(stickValY, stickValX);
  int degree = round(angle * 512/PI);
  
  Serial.println(degree);
  if(degree == 128) {
    for(int x = 0; x < countLedsStick; x++) {
      ledsStick[x] = CRGB::Black;
    }
  } else {
    for(int x = 0; x < countLedsStick; x++) {
      ledsStick[x].setHSV(degree, 255, 255);
    }
  }
    




  
  
}// loop
