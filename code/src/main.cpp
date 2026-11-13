#include <Arduino.h>
#include <EEPROM.h>
#include <tinyNeoPixel.h>

const int D1 = PIN_PB0; // Left eye
const int D2 = PIN_PB1; // Right eye
const int D3 = PIN_PA2; // 0402
const int D4 = PIN_PA6; // 0603
const int D5 = PIN_PA7; // 0805
const int D6 = PIN_PA5; // 1206
const int LEDS[] = {D1,D2,D3,D4,D5,D6};

const int NEOPIXEL = PIN_PA4; // Neopixels
const int PIXEL_COUNT = 3;
tinyNeoPixel strip = tinyNeoPixel(PIXEL_COUNT, NEOPIXEL, NEO_GRB);

const int BUTTON = PIN_PB3; // Button
bool pressed = true;

const int BUZZ = PIN_PA3; // Buzzer

//rainbow animation
int rainbox_index = 0;
// LED animation 
bool flipflop = true;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup() {

  for(int i = 0; i < 6; i++){
    pinMode(LEDS[i], OUTPUT);
    digitalWrite(LEDS[i], LOW);
  }
  pinMode(NEOPIXEL, OUTPUT);
  strip.setBrightness(50);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZ, OUTPUT);

}

void loop() {

  //Handle Button
  if (digitalRead(BUTTON) == LOW && !pressed){
    pressed = true;
    tone(BUZZ, 1000);
    delay(1000);     // Short delay to debounce button.
  }
  if (digitalRead(BUTTON) == HIGH){
    pressed = false;
    noTone(BUZZ);
    delay(20);
  }

  //Flash LEDs
  for(int i = 0; i < 6; i++){
    digitalWrite(LEDS[i], flipflop);
    delay(100);
  }
  flipflop = !flipflop;

  //Rainbow cycle the neopixels
  for (int i = strip.numPixels() - 1; i >= 0; i--) {
    strip.setPixelColor(i, Wheel(((i * 64) + rainbox_index) & 255));
    rainbox_index = (rainbox_index+1)%256;
  }
  strip.show();

}

