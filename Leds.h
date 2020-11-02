/**************
 * 
 * Name: Leds.h
 * Description: A library to control the led strip and a few functions to display various patterns
 * 
 *************/

#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H

#define LED_PIN     3
#define NUM_LEDS    300
#define MAX_BRIGHTNESS 250
#define MAX_DELAY 250

typedef enum {Off,
              White,
              Color,
              Snake,
              Wave,
              Bounce,
              Fade,
              Flash,
              PingPong
              } LedState; // function to display on led's

class Leds {
    public:
        // Initialization functions
        Leds();
        Leds(uint16_t brightness, uint16_t length, uint16_t delay);
        void setup();

        void set_state(LedState state);
        LedState get_state();

        // functions for one color
        void turn_on();
        void turn_off();
        void static_color(CRGB color);

        // Getters and setters
        void set_brightness(uint16_t brightness);
        void set_length(uint16_t length);
        void set_delay(uint16_t delay);
        void set_primary(CRGB primary);
        void set_secondary(CRGB secondary);
        // void change_direction();

        // update functions (too keep refreshing the led's)
        void refresh();
        void redraw();

    protected:
        CRGB leds[NUM_LEDS];

        // static colors
        void white(void);
        void black(void);
        void color(CRGB col);
        void range(CRGB col, uint16_t low, uint16_t high);
        void set_led(CRGB col, uint16_t idx);
        
        // functions for animated colors
        void init_snake();
        void snake(); // snake the color along the led strip
        
        void init_wave();
        void wave(); // create a 'wave' of color along the led's'

        void fade(); // fade from one color to another.

        void flash(bool init=false); // flash two different colors

        void init_pingpong();
        void ping_pong();

        void bounce(bool alternate=false);

        LedState state_ = Off;
        uint64_t last_updated = 0;
    private:
        CRGB primary_ = CRGB(255, 0, 0);
        CRGB secondary_ = CRGB(0, 0, 255);

        uint8_t brightness_ = 25; // 10% brightness
        uint16_t length_ = 10; // 10 leds
        uint8_t delay_ = 0; // fastest speed

        bool direction_ = true;

        uint16_t index_ = 0; // start at teh very beginning.
        bool update_ = false; // whether to update 'all' the led's.
};


#endif
