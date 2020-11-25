/**************
 * 
 * Name: led_control.ino
 * Description: main arduino function to control LED strip.
 * 
 *************/
#include <Arduino.h>
#include "Keypad.h"
#include "Interface.h"
#include "Leds.h"
#include "Message.h"
#include "Radio.h"

// setup the liquid crystal display
//  LiquidCrystal(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,  Analog Pin, ourBoard = 0);
Keypad lcd(       8,          9,              4,          5,          6,          7,           18,         false);

Leds lights;
Radio radio(A1, A2, &lights);
void radio_ISR(); // to catch message recieved interrupts

Interface interface(&lcd, &lights, &radio);

void setup() {
  //setup Serial display (for debugging)
  Serial.begin(115200);

  // Seed some chaos...
  randomSeed(analogRead(5));

  // Setup LED's
  lights.setup();

  //Setup LCD Display
  lcd.begin(16,2);

  // Start the radio module.
  radio.begin();

  // Use an interrupt for handling messages.
  attachInterrupt(0, radio_ISR, LOW);
}

void loop() {
    radio.listen();
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

void radio_ISR() {
    radio.messageReceivedISR();
}