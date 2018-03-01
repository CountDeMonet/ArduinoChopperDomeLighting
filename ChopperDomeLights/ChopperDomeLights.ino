#include <Adafruit_NeoPixel.h>

#define NEO_EYES 2
Adafruit_NeoPixel eyeStick = Adafruit_NeoPixel(3, NEO_EYES, NEO_GRB + NEO_KHZ800);

// 13 with an offset of 3
#define NEO_POWER 3 // for cyclotron
Adafruit_NeoPixel powerStick = Adafruit_NeoPixel(16, NEO_POWER, NEO_GRB + NEO_KHZ800);

void setup() {
  // configure neopixels
  eyeStick.begin();
  eyeStick.setBrightness(80);
  eyeStick.show(); // Initialize all pixels to 'off'

  powerStick.begin();
  powerStick.setBrightness(80);
  powerStick.show(); // Initialize all pixels to 'off'
}

const int bright_interval = 100;
unsigned long prevBrightMillis = 0;
int bright_led = 3;
bool reverse = true;

const int swap_interval = 60000;
unsigned long prevSwapMillis = 0;
int swap_level = 1;
bool firstRun = true;

void loop() {
  int currentMillis = millis();

  if ( firstRun )
  {
    eyeStick.setPixelColor(1, eyeStick.Color(0, 63, 128));
    eyeStick.setPixelColor(2, eyeStick.Color(0, 63, 128));
    eyeStick.show();
    firstRun = false;
  }

  if (currentMillis - prevSwapMillis > swap_interval)
  {
    prevSwapMillis = currentMillis;

    switch ( swap_level )
    {
      case 1: // set all leds to white
        eyeStick.setPixelColor(0, eyeStick.Color(255, 255, 255));
        swap_level = 2;
        break;

      case 2: // set all leds to red
        eyeStick.setPixelColor(0, eyeStick.Color(255, 0, 0));
        swap_level = 3;
        break;

      case 3: // set all leds to blue
        eyeStick.setPixelColor(0, eyeStick.Color(0, 0, 255));
        swap_level = 4;
        break;

      case 4: // off
        eyeStick.setPixelColor(0, 0);
        swap_level = 1;
        break;
    }
    eyeStick.show();
  }

  for (int i = 3; i < 16; i++)
  {
    powerStick.setPixelColor(i, powerStick.Color(15, 0, 0));
  }

  if (currentMillis - prevBrightMillis > bright_interval)
  {
    prevBrightMillis = currentMillis;
    if ( reverse == false )
    {
      bright_led++;
    }
    else
    {
      bright_led--;
    }
  }
  powerStick.setPixelColor(bright_led, powerStick.Color(255, 0, 0));
  powerStick.show();

  if ( bright_led == 15 )
  {
    reverse = true;
  }
  if ( bright_led == 3 )
  {
    reverse = false;
  }

  delay(10);
}
