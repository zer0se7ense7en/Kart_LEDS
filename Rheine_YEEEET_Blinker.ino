


#include <FastLED.h>
#include "StateManager.h"
#include "Button.h"

#define NUM_LEDS 50
#define DATA_PIN_L 7
#define DATA_PIN_R 8

#define Switch1 2
#define Switch2 3
#define Switch3 4


/*
 * Configs
 */
 #define rainbowTime 10000
 #define rainbowXMultiplier 80


CRGB ledsL[NUM_LEDS];
CRGB ledsR[NUM_LEDS];

// Defines the events
 void onSwitchToggled(bool mode);

/*If you want to invert, just uncomment the command below*/
Button switchRight(onSwitchToggled,4/*,true*/);
Button switchMiddle(onSwitchToggled,3);
Button switchLeft(onSwitchToggled,2);

StateManager mng;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN_L, GRB>(ledsL, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_R, GRB>(ledsR, NUM_LEDS);
  
  /*pinMode(Switch1, INPUT);
  pinMode(Switch2, INPUT);
  pinMode(Switch3, INPUT);*/
   
  FastLED.setMaxPowerInVoltsAndMilliamps(5,2500);
  FastLED.setBrightness(255);

  Serial.begin(115200);
}

void loop() {
  //updates the booleans relating switch positions
  //SwitchPosUpdate();
  //switch the modes accordingly and execute them
  modeSwitch();

  //SwitchDebug();

  // Button updates
  switchRight.update();
  switchMiddle.update();
  switchLeft.update();
  SwitchDebug();
}

/*
 * Events
 */

 void onSwitchToggled(bool mode){
  cleanL(0);
  cleanR(1);
  mng.reset();
 }

/*
 * TODO:
 * - adjust LED_STRIP Length
 * 
 * 
 */



void modeSwitch() {
/*
 * Middle down:
 *  Left & down normal blinkers & warn light
 *  
 * Middle up:
 *  rainbow, xrainbox, whitelight when all 3 up, blaulicht
 *
 *
 */

  // Checks the middle-switch state
  if(switchMiddle.isPressed()){

    // Switches between the different modes

    // Warning-light
    if(switchRight.isPressed() && switchLeft.isPressed())
      Warnblinker();

    // Right blinker
    else if(switchRight.isPressed())
      BlinkerRight();

    // Left blinker
    else if(switchLeft.isPressed())
      BlinkerLeft();
    
  }else{
    
    // White-light
    if(switchRight.isPressed() && switchLeft.isPressed())
      WhiteLight();

    // Time-based rainbow
    else if(switchRight.isPressed())
      TimeRainbow();

    // X-Axis-Rainbow
    else if(switchLeft.isPressed())
      XRainbow();

    // Both switches are down
    else
      Blaulicht();
  }
  /*
  //Zivilmodus mit Blinkern
  if(Switch2Down) {
    if(Switch1Up && Switch3Down) {
      currentModeState = 1;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      BlinkerLeft();
    }
    else if(Switch1Down && Switch3Up) {
      currentModeState = 2;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      BlinkerRight();
    }
    else if(Switch1Up && Switch3Up) {
      currentModeState = 3;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      Warnblinker();
    }
  }
  //LSDModus mit purem Chaos und REGENBOEGEN!!!!!!!
  else if(Switch2Up) {
    if(Switch1Down && Switch3Down) {
      currentModeState = 4;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      Blaulicht();
    }
    else if(Switch1Up && Switch3Down) {
      currentModeState = 5;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      XRainbow();
    }
    else if(Switch1Down && Switch3Up) {
      currentModeState = 6;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      TimeRainbow();
    }
    else if(Switch1Up && Switch3Up) {
      currentModeState = 7;
      ResetIfModeStateSwitch(lastModeState,currentModeState);
      WhiteLight();
    }
  }
  lastModeState = currentModeState;*/
}

// ModeStateManager
void ResetIfModeStateSwitch(int last, int curr) {
  if(last != curr) {
    cleanL(1);
    cleanR(1);
    mng.reset();
    Serial.println("ModeState has been switched!!!!!!!!!!!!!!!!");
  }
}

void BlinkerLeft() {
  if(mng.isState(0,500)){
    colorL(32,1);
  }
  if(mng.isState(1,500,0)){
    cleanL(1);
  }
}

void BlinkerRight() {
  if(mng.isState(0,500)){
    colorR(32,1);
  }

  if(mng.isState(1,500,0)){
    cleanR(1);
  }
}

void Warnblinker() {
  if(mng.isState(0,500)){
    colorL(32,1);
    colorR(32,1);
  }

  if(mng.isState(1,500,0)){
    cleanL(1);
    cleanR(1);
  }
}

void TimeRainbow() {
  
  
  
  /*
  for(int h = 0; h < 253; h++) {
    if(mng.isState(h,50)) {
      colorL(h,0);
      colorR(h,1);
    }
  }
  if(mng.isState(254,50,0)) {
    colorL(0,0);
    colorR(0,1);
  }*/

    // Gets a timespace that is timebase (with the millis) based on the config value
    float timescale = millis()%rainbowTime;
    // Gets the percentage state where the rainbow currently is
    float perc = timescale/rainbowTime;

    colorL(perc * 253,1);
}

void XRainbow() {

    // Iterates over all pixels to update them x-value and time based
    for(byte i = 0; i < NUM_LEDS; i++) {
      // Gets a timespace that is timebase (with the millis) based on the config value and adds the x value to it
      float timescale = (millis() + i * rainbowXMultiplier)%rainbowTime;
      // Gets the percentage state where the rainbow currently is
      float perc = timescale/rainbowTime;

      // Sets the final pixel color
      ledsL[i].setHue(perc * 253);
    }
    FastLED.show();
}

void Blaulicht() {
  if(mng.isState(0,70)) {
    colorL(160,1);
  }

  if(mng.isState(1,35)) {
    cleanL(1);
  }

  if(mng.isState(2,100)) {
    colorL(160,1);
  }

  if(mng.isState(3,35)) {
    cleanL(1);
  }

  if(mng.isState(4,70)) {
    colorR(160,1);
  }

  if(mng.isState(5,35)) {
    cleanR(1);
  }

  if(mng.isState(6,100)) {
    colorR(160,1);
  }

  if(mng.isState(7,35,0)) {
    cleanR(1);
  }

}

void WhiteLight() {
 // if(mng.isState(0,1000)) {
    whiteL(1);
    whiteR(1);
 // }
}

/*void SwitchPosUpdate() {
  if(digitalRead(Switch1) == HIGH) {
    Switch1Up = true; Switch1Down = false;
    //Serial.println("Switch1Up");
  }
  if(digitalRead(Switch1) == LOW) {
    Switch1Up = false; Switch1Down = true;
    //Serial.println("Switch1Down");
  }

  if(digitalRead(Switch2) == HIGH) {
    Switch2Up = true; Switch2Down = false;
    //Serial.println("Switch2Up");
  }
  if(digitalRead(Switch2) == LOW) {
    Switch2Up = false; Switch2Down = true;
    //Serial.println("Switch2Down");
  }

  if(digitalRead(Switch3) == HIGH) {
    Switch3Up = true; Switch3Down = false;
    //Serial.println("Switch3Up");
  }
  if(digitalRead(Switch3) == LOW) {
    Switch3Up = false; Switch3Down = true;
    //Serial.println("Switch3Down");
  }
}*/

void cleanL(bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsL[i].setRGB(0,0,0);
  }
  if(shouw) {
    FastLED.show();
  }
}

void cleanR(bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsR[i].setRGB(0,0,0);
  }
  if(shouw) {
    FastLED.show();
  }
}

void cleanall() {
  FastLED.clear();
  FastLED.show();
}

void colorL(byte hue, bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsL[i].setHue(hue);
  }
  if(shouw) {
    FastLED.show();
  }
}

void colorR(byte hue, bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsR[i].setHue(hue);
  }
  if(shouw) {
    FastLED.show();
  }
}

void colspL(byte hue, byte startled, byte endled, bool shouw) {
  for(byte i = startled-1; i <= endled-1; i++) {
    ledsL[i].setHue(hue);
  }
  if(shouw) {
    FastLED.show();
  }
}

void colspR(byte hue, byte startled, byte endled, bool shouw) {
  for(byte i = startled-1; i <= endled-1; i++) {
    ledsR[i].setHue(hue);
  }
  if(shouw) {
    FastLED.show();
  }
}

void whiteL(bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsL[i].setRGB(255,255,255);
  }
  if(shouw) {
    FastLED.show();
  }
}

void whiteR(bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsR[i].setRGB(255,255,255);
  }
  if(shouw) {
    FastLED.show();
  }
}

void whispL(byte startled, byte endled, bool shouw) {
  for(byte i = startled-1; i <= endled-1; i++) {
    ledsL[i].setRGB(255,255,255);
  }
  if(shouw) {
    FastLED.show();
  }
}

void whispR(byte startled, byte endled, bool shouw) {
  for(byte i = startled-1; i <= endled-1; i++) {
    ledsR[i].setRGB(255,255,255);
  }
  if(shouw) {
    FastLED.show();
  }
}

void rgbL(byte r, byte g, byte b, bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsL[i].setRGB(r,g,b);
  }
  if(shouw) {
    FastLED.show();
  }
}

void rgbR(byte r, byte g, byte b, bool shouw) {
  for(byte i = 0; i < NUM_LEDS; i++) {
    ledsR[i].setRGB(r,g,b);
  }
  if(shouw) {
    FastLED.show();
  }
}

void SwitchDebug() {
  Serial.print("Button1");Serial.println(digitalRead(Switch1));
  Serial.print("Button2");Serial.println(digitalRead(Switch2));
  Serial.print("Button3");Serial.println(digitalRead(Switch3));
  delay(100);
}
