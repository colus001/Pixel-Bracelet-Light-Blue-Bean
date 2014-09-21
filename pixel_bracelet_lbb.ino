#include <Adafruit_NeoPixel.h>

#define PIN 4
#define PIXELS 9

#define COLOR_WIPE 1
#define RAINBOW 2
#define ROTATE_SINGLE_PIXEL 3
#define ROTATE_SINGLE_PIXEL_BOUNCE 4
#define THEATER_CHASE 5
#define THEATER_CHASE_RAINBOW 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int type;

void setup() 
{ 
  strip.begin();
  strip.show();
} 

void loop() 
{   
  uint32_t number = Bean.readScratchNumber(1);
  //  uint32_t number = 1 255 255 255;
  //  uint32_t number = 1 85 85 85;

  //  type = number/1000000000;
  //  number -= type*1000000000;
  //
  //  int red = number/1000000;
  //  int green = (number - red*1000000)/1000;
  //  int blue = number - red*1000000 - green*1000;

  type = number/1000000;
  number -= type*1000000;

  int red = number/10000;
  int green = (number - red*10000)/100;
  int blue = number - red*10000 - green*100;

  red *= 3;
  green *= 3;
  blue *= 3;
  
  uint16_t r, g, b;
  AccelerationReading accel = {0, 0, 0};

  switch (type) {
  case 1: 
    colorWipe(strip.Color(red, green, blue), 50);
    type = 0;
    break;
  case 2:
    rainbow(20);
    break;
  case 3:
    rotateSinglePixel(strip.Color(red, green, blue), 50, true);
    break;
  case 4:
    rotateSinglePixel(strip.Color(red, green, blue), 50, false);
    break;
  case 5:
    theaterChase(strip.Color(red, green, blue), 50);
    break;
  case 6:
    theaterChaseRainbow(50);
    break;
  case 7:
    accel = Bean.getAcceleration();
    r = (abs(accel.xAxis)) / 4;
    g = (abs(accel.yAxis)) / 4;
    b = (abs(accel.zAxis)) / 4;

    colorWipe(strip.Color(r, g, b), 50);
    break;
  default:
    break;
  }

  Bean.sleep(100);
}

void rotateSinglePixel(uint32_t c, uint8_t wait, bool isSingle) {
  int totalPixels = strip.numPixels();

  for (int i = 0; i < totalPixels; i++) {
    strip.setPixelColor(i, c);    //turn every third pixel on

    for ( int j = 0; j < totalPixels; j++ ) {
      if ( j != i ) {
        strip.setPixelColor(j, 0);
      }
    }

    strip.show();
    delay(wait);
  }

  if ( isSingle ) {
    return;
  } 

  for (int i = totalPixels-1; i >= 0; i--) {
    strip.setPixelColor(i, c);    //turn every third pixel on

    for ( int j = totalPixels-1; j >= 0; j-- ) {
      if ( j != i ) {
        strip.setPixelColor(j, 0);
      }
    }

    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
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

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


