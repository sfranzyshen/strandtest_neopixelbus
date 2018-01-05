#include <NeoPixelBus.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

const uint16_t PixelCount = 96; 
//const uint8_t PixelPin = D1;

// three element pixels, in different order and speeds
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod> strip(PixelCount, PixelPin);

// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
//NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PixelCount);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
//
// NeoEsp8266Uart800KbpsMethod uses GPI02 instead

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(115200);
  while (!Serial); // wait for serial attach

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();

  Serial.println();
  Serial.println("Running...");
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(RgbColor(255, 0, 0), 50); // Red
  colorWipe(RgbColor(0, 255, 0), 50); // Green
  colorWipe(RgbColor(0, 0, 255), 50); // Blue
//colorWipe(RgbwColor(0, 0, 0, 255), 50); // White RGBW

  // Send a theater pixel chase in...
  theaterChase(RgbColor(127, 127, 127), 50); // White
  theaterChase(RgbColor(127, 0, 0), 50); // Red
  theaterChase(RgbColor(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(RgbColor c, uint8_t wait) {
  for(uint16_t i=0; i<strip.PixelCount(); i++) {
    strip.SetPixelColor(i, c);
    strip.Show();
    delay(wait);
  }
}

/*
// Fill the dots one after the other with a color
void colorWipe(RgbwColor c, uint8_t wait) {
  for(uint16_t i=0; i<strip.PixelCount(); i++) {
    strip.SetPixelColor(i, c);
    strip.Show();
    delay(wait);
  }
}
*/

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.PixelCount(); i++) {
      strip.SetPixelColor(i, Wheel((i+j) & 255));
    }
    strip.Show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.PixelCount(); i++) {
      strip.SetPixelColor(i, Wheel(((i * 256 / strip.PixelCount()) + j) & 255));
    }
    strip.Show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(RgbColor c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.PixelCount(); i=i+3) {
        strip.SetPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.Show();

      delay(wait);

      for (uint16_t i=0; i < strip.PixelCount(); i=i+3) {
        strip.SetPixelColor(i+q, RgbColor(0,0,0));        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.PixelCount(); i=i+3) {
        strip.SetPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.Show();

      delay(wait);

      for (uint16_t i=0; i < strip.PixelCount(); i=i+3) {
        strip.SetPixelColor(i+q, RgbColor(0,0,0));        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
RgbColor Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}
