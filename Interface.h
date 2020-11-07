/**************
 * 
 * Name: Interface.h
 * Description: Class to make it easier to write to the LCD display and display a menu. 
 * 
 *************/


#ifndef INTERFACE_H
#define INTERFACE_H

#include "Keypad.h"
#include "Leds.h"
#include <string.h>

using namespace std;

#define ARR_DELAY 1000

typedef enum {
    Nothing,
    Menu,
    Settings,
    Primary,
    Secondary,
    Brightness,
    Speed
} DisplayMode; // state_s

typedef enum {
    All,
    SetColor,
    SetBrightness,
    SetSpeed,
    None
} SettingType;

class Interface {
    public:
        Interface(Keypad* lcd);
        Interface(Keypad* lcd, Leds* lights);

        void draw();
        void menu();
        void arrows();
        
        // Show available options
        void options();
        uint8_t next_option();
        uint8_t prev_option();
        uint8_t set_option(uint8_t option);

        // Show available settings
        void settings();
        uint8_t next_setting();
        uint8_t prev_setting();
        uint8_t set_setting(uint8_t option);

        // Functions for changing settings
        void brightness();
        void speed();
        void primary();
        void secondary();

        // Respond to keypresses
        void select();
        void up();
        void down();
        void next();
        void prev();
        
    protected:
        void check_setting();

    private:
        DisplayMode lastStateWritten_ = Nothing;
        DisplayMode state_ = Menu;
        Keypad* lcd_;

        // Settings for the led lights
        Leds* lights_;
        uint8_t selectedBrightness_;
        uint8_t selectedSpeed_;
        // uint8_t selectedLength_;

        static const uint8_t numOptions_ = 12;
        String options_[numOptions_] =          {"Wave", "Snake", "Fade", "Ping Pong", "Bounce", "Flash", "Rainbow", "Random", "Gravity", "Color",  "White",       "Off"};
        LedState ledOptions_[numOptions_] =     { Wave,   Snake,   Fade,   PingPong,    Bounce,   Flash,   Rainbow,   Random,   Gravity,   Color,    White,         Off };
        SettingType stateSettings_[numOptions_] = {All,   All,     All,    All,         All,      All,     All,       All,      All,       SetColor, SetBrightness, None};
        uint8_t curOption_ = 0;
        uint8_t prevOption_ = numOptions_ - 1;

        static const uint8_t numSettings_ = 4;
        String settings_[numSettings_] = {"Brightness", "Speed", "Color 1", "Color 2"};
        DisplayMode setState_[numSettings_] = {Brightness, Speed, Primary, Secondary};
        uint8_t curSetting_ = 0;
        uint8_t prevSetting_ = numSettings_ - 1;

        uint32_t last_changed_ = 0;
        bool flash_ = true;
};

#endif
