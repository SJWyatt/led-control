/**************
 * 
 * Name: led_control.ino
 * Description: main arduino function to control LED strip.
 * 
 *************/
#include "Keypad.h"
#include "Interface.h"
#include "Leds.h"

// setup the liquid crystal display
//  LiquidCrystal(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,  Analog Pin, ourBoard = 0);
Keypad lcd(       8,          9,              4,          5,          6,          7,           18,         false);

Leds lights;
Interface interface(&lcd, &lights);

void setup() {
  // Setup LED's
  lights.setup();

  //setup Serial display (for debugging)
  Serial.begin(9600);

  //Setup LCD Display
  lcd.begin(16,2);
}


void loop() {
    interface.draw();

    switch(lcd.hasBeenPushed()) {
        case SELECT:
            interface.select();
            break;
        case UP:
            interface.up();
            break;
        case DOWN:
            interface.down();
            break;
        case LEFT:
            interface.prev();
            break;
        case RIGHT:
            interface.next();
            break;
        default:
            break;
    }
}
