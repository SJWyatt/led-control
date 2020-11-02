/**************
 * 
 * Name: Keypad.h
 * Description: A library to add button functionality to the LiquidCrystal library.
 * 
 *************/

#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <avr/io.h>

const unsigned short debounceDelay = 50;

typedef enum {LEFT, RIGHT, UP, DOWN, SELECT, NONE} Button;

class Keypad : public LiquidCrystal //use inheritance
{
  public:
    Keypad(int a,int b,int c, int d, int e, int f, int ang, bool ourBoard);
    Button hasBeenPushed();
    
  private:
    unsigned short buttonPin;
    bool ourBoard = false;
    unsigned long lastDebounceTime;
    short lastKnownButtonState = 1024;
    unsigned short buttonState = 1024;
};

#endif
