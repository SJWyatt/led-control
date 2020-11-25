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
    Speed,
    Length
} DisplayMode; // state_s

typedef enum {
    All,
    SetColor,
    SetBrightness,
    SetSpeed,
    SetLength,
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
        void increase_brightness();
        void decrease_brightness();
        void set_brightness(uint8_t brightness);

        void speed();
        void decrease_speed();
        void increase_speed();
        void set_speed(uint8_t speed);

        void length();
        void decrease_length();
        void increase_length();
        void set_length(uint8_t length);

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
        int8_t lastStateWritten_ = Nothing; // Using int8_t, instead of DisplayMode for size
        int8_t state_ = Menu; // Using int8_t, instead of DisplayMode for size
        Keypad* lcd_;

        // Settings for the led lights
        Leds* lights_;
        uint8_t shownBrightness_ = 0;
        uint8_t shownSpeed_ = MAX_DELAY;
        uint8_t shownLength_ = 0;
        // uint8_t selectedLength_;

        static const uint8_t numOptions_ = 12;
        char options_[numOptions_][4] =          {"Wv", "Snk", "Fde", "Png", "Bnc", "Fls", "Rbw", "Rnd", "Grv", "Clr",  "Wht",       "Off"};
        int8_t ledOptions_[numOptions_] =     { Wave,   Snake,   Fade,   PingPong,    Bounce,   Flash,   Rainbow,   Random,   Gravity,   Color,    White,         Off }; // Using int8_t, instead of LedState for size
        int8_t stateSettings_[numOptions_] = {All,   All,     All,    All,         All,      All,     All,       All,      All,       SetColor, SetBrightness, None}; // Using int8_t, instead of SettingType for size
        uint8_t curOption_ = 0;
        uint8_t prevOption_ = numOptions_ - 1;

        static const uint8_t numSettings_ = 5;
        char settings_[numSettings_][6] = {"Brtns", "Spd", "Len", "Clr1", "Clr2"};
        int8_t setState_[numSettings_] = {Brightness, Speed, Length, Primary, Secondary}; // Using int8_t, instead of DisplayMode for size
        uint8_t curSetting_ = 0;
        uint8_t prevSetting_ = numSettings_ - 1;

        uint32_t last_changed_ = 0;
        bool flash_ = true;
};

#endif
