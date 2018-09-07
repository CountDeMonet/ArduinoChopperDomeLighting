#include <Adafruit_NeoPixel.h>

#define NEO_EYES 2
Adafruit_NeoPixel eyeStick = Adafruit_NeoPixel(3, NEO_EYES, NEO_GRB + NEO_KHZ800);

// 13 with an offset of 3
#define NEO_POWER 3 // for cyclotron
Adafruit_NeoPixel powerStick = Adafruit_NeoPixel(16, NEO_POWER, NEO_GRB + NEO_KHZ800);

unsigned long bright_interval = 70;
unsigned long prevBrightMillis = 0;
int bright_led = 4;
int trailing_led = 4;
int trailing_led_2 = 4;
bool reverse = true;

unsigned long swap_interval;
unsigned long prevSwapMillis = 0;
int swap_level = 1;
bool firstRun = true;

void setup() {
  // configure neopixels
  eyeStick.begin();
  eyeStick.setBrightness(90);
  eyeStick.show(); // Initialize all pixels to 'off'

  powerStick.begin();
  powerStick.setBrightness(80);
  powerStick.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(0));
  swap_interval = random(180000, 300000);
}

void loop() {
  unsigned long currentMillis = millis();

  /* TURN ON THE EYES */
  if ( firstRun ) {
    eyeStick.setPixelColor(1, eyeStick.Color(0, 255, 255));
    eyeStick.setPixelColor(2, eyeStick.Color(0, 255, 255));
    eyeStick.show();
    firstRun = false;
  }

  /* CHANGE HOLO LIGHT EVERY X SECONDS */
  if ((unsigned long)(currentMillis - prevSwapMillis) >= swap_interval) {
    switch ( swap_level ) {
      case 1: // set led to white
        eyeStick.setPixelColor(0, eyeStick.Color(255, 255, 255));
        swap_interval = random(60000, 180000); // 1 - 3 minutes
        swap_level = 2;
        break;

      case 2: // off
        eyeStick.setPixelColor(0, 0);
        swap_interval = random(180000, 300000);  // 3 - 5 minutes
        swap_level = 1;
        break;
    }
    eyeStick.show();

    prevSwapMillis = millis();
  }
  /* END HOLO LIGHT */

  /* POWERCELL ANIMATION */
  for (int i = 4; i < 16; i++) {
    powerStick.setPixelColor(i, powerStick.Color(5, 0, 0));
  }

  if ((unsigned long)(currentMillis - prevBrightMillis) >= bright_interval) {
    if ( reverse == false ) {
      trailing_led_2 = trailing_led;
      trailing_led = bright_led;
      bright_led++;

      if ( bright_led == 15 ) {
        bright_interval = 500;
      }else {
        bright_interval = 75;
      }
    } else {
      trailing_led_2 = trailing_led;
      trailing_led = bright_led;
      bright_led--;
      if ( bright_led == 4 ) {
        bright_interval = 600;
      } else {
        bright_interval = 75;
      }
    }
    prevBrightMillis = millis();
  }

  powerStick.setPixelColor(bright_led, powerStick.Color(255, 0, 0));

  if ( bright_led == 15 ){
    reverse = true;
  } else if ( bright_led == 4 ){
    reverse = false;
  } else {
    powerStick.setPixelColor(trailing_led, powerStick.Color(128, 0, 0));
    powerStick.setPixelColor(trailing_led_2, powerStick.Color(64, 0, 0));
  }
  powerStick.show();

  /* END POWERCELL ANIMATION */

  delay(50);
}
