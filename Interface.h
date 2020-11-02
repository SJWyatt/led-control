/**************
 * 
 * Name: Interface.h
 * Description: Class to make it easier to write to the LCD display and display a menu. 
 * 
 *************/


#ifndef INTERFACE_H
#define INTERFACE_H

#include "Keypad.h"
#include <string.h>

using namespace std;

#define ARR_DELAY 1000

typedef enum {Nothing,
              Menu,
              Settings
              } DisplayMode; // state_s

class Interface {
    public:
        Interface(Keypad* lcd);

        void draw();
        
        DisplayMode menu();
        void arrows();
        
        void options();
        uint8_t next_option();
        uint8_t prev_option();
        uint8_t set_option(uint8_t option);

        void settings();
        uint8_t next_setting();
        uint8_t prev_setting();
        uint8_t set_setting(uint8_t option);

        void select();
        void up();
        void down();
        void next();
        void prev();
        
    private:
        DisplayMode lastState_ = Nothing;
        DisplayMode state_ = Menu;
        Keypad* lcd_;

        String options_[7] = {"Wave", "Snake", "Fade", "Ping Pong", "Color", "White", "Off"};
        uint8_t curOption_ = 0;
        uint8_t prevOption_ = 7;

        String settings_[4] = {"Brightness", "Speed", "Color 1", "Color 2"};
        uint8_t curSetting_ = 0;
        uint8_t prevSetting_ = 4;

        uint32_t last_changed_ = 0;
        bool flash_ = true;
};

#endif
