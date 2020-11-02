#include "Keypad.h"

Keypad::Keypad(int a,int b,int c, int d, int e, int f, int ang, bool ourBoard) : LiquidCrystal(a,b,c,d,e,f) {
  if(ang > 17 && ang <= 21) {
    buttonPin = (ang-18);
  }

  this->ourBoard = ourBoard;
}

//*
Button Keypad::hasBeenPushed() {
  unsigned short buttonValue = analogRead(buttonPin); //read a value
  unsigned short buttonValueTwo = analogRead(buttonPin);
  for(int i = 0; i < 3; i++) {
    buttonValueTwo += analogRead(buttonPin);
  }
  buttonValueTwo /= 4; //take an average of the last four readings.
  
  if((buttonValueTwo+10) >= (buttonValue) && buttonValueTwo <= (buttonValue+10)) { //check whether the value read is within +-10 variance.
    Button returnState = NONE;
    //if it has been debounceDelay since the last time we read a value
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if(this->ourBoard) { // change debounce for different analogue resistance
        // if the button state has changed (within reason):
        if ((buttonValue+30) <= (buttonState) || buttonValue >= (buttonState+30)) {
          // Record the new button state.
          buttonState = buttonValue;
          
          // Set the value to return
          if (buttonState < 20) {
            returnState = RIGHT;
          }
          else if (buttonState > 200 && buttonState < 230) {
            returnState = UP;
          }
          else if (buttonState > 395 && buttonState < 420) {
            returnState = DOWN;
          }
          else if (buttonState > 615 && buttonState < 635) {
            returnState = LEFT;
          }
          else if (buttonState > 815 && buttonState < 835) {
            returnState = SELECT;
          }
          //save the time we last returned a button press
          lastDebounceTime = millis();
        }
      }
      else { //The board that is in the lab
        // if the button state has changed (within reason):
        if ((buttonValue+30) <= (buttonState) || buttonValue >= (buttonState+30)) {
          // Record the new button state.
          buttonState = buttonValue;
          
          // Set the value to return
          if (buttonState < 20) {
            returnState = RIGHT;
          }
          else if (buttonState > 90 && buttonState < 110) {
            returnState = UP;
          }
          else if (buttonState > 245 && buttonState < 265 ) {
            returnState = DOWN;
          }
          else if (buttonState > 400 && buttonState < 420) {
            returnState = LEFT;
          }
          else if (buttonState > 630 && buttonState < 650) {
            returnState = SELECT;
          }
          //save the time we last returned a button press
          lastDebounceTime = millis();
        }
      }
      // save the reading.  Next time through the loop, it'll be the lastButtonState:
      lastKnownButtonState = buttonValue;
    }
    return returnState;
  }
  else {
    return NONE;
  }
}