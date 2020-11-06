#include "Leds.h"


Leds::Leds() {
    state_ = Off;
    randomSeed(analogRead(0));
}

Leds::Leds(uint16_t brightness, uint16_t length, uint16_t delay) {
    this->set_brightness(brightness);
    this->set_length(length);
    this->set_delay(delay);

    Leds();
}

void Leds::setup() {
    // Setup LED's
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds_, NUM_LEDS);
    FastLED.setBrightness(brightness_);

    // Start with LED's off
    this->black();
}

void Leds::set_state(LedState state) {
    // allow for the led's to be updated dynamically (in refresh operation)
    state_ = state;

    // draw initial configuration of colors
    this->redraw();
}

LedState Leds::get_state() {
    return state_;
}

void Leds::refresh() {
    if (update_) {
        this->redraw();
    }
    if ((this->last_updated + delay_) <= millis()) {
        switch (state_) {
            case Snake:
                this->snake();
                break;
            case Wave:
                this->wave();
                break;
            case Bounce:
                this->bounce();
                break;
            case Fade:
                this->fade();
                break;
            case Flash:
                this->flash();
                break;
            case PingPong:
                this->ping_pong();
                break;
            case Rainbow:
                this->rainbow();
                break;
            case Random:
                this->random_colors();
                break;
            case Off:
            case White:
            case Color:
            default:
                // do nothing
                break;
        }

        this->last_updated = millis();
    }
}

void Leds::redraw() {
    // do operation that needs to be done once.
    switch(state_) {
        case Off:
            // turn all the led's off
            this->black();
            break;
        case White:
            this->white();
            break;
        case Color:
            this->color(primary_);
            break;
        case Snake:
            this->init_snake();
            break;
        case Wave:
            this->init_wave();
            break;
        case Bounce:
            this->bounce();
            break;
        case Fade:
            this->fade();
            break;
        case Flash:
            this->flash(true);
        case PingPong:
            this->init_pingpong();
            break;
        case Rainbow:
            this->rainbow();
            break;
        case Random:
            this->random_colors();
            break;
        default:
            // do nothing
            break;
    }
    update_ = false;
}

void Leds::turn_on() {
    state_ = White;
    this->redraw();
}

void Leds::turn_off() {
    state_ = Off;
    this->redraw();
}

void Leds::static_color(CRGB color) {
    this->set_primary(color);
    state_ = Color;
    this->redraw();
}

void Leds::set_primary(CRGB primary) {
    primary_ = primary;
    update_ = true;
}

void Leds::set_secondary(CRGB secondary) {
    secondary_ = secondary;
    update_ = true;
}

void Leds::white() {
    this->range(CRGB::White, 0, NUM_LEDS);
}

void Leds::black() {
    this->range(CRGB::Black, 0, NUM_LEDS);
}

void Leds::color(CRGB col) {
    this->range(col, 0, NUM_LEDS);
}

void Leds::range(CRGB col, uint16_t low, uint16_t high) {
    if(high >= NUM_LEDS) { // error checking
        high = NUM_LEDS;
    }
    if(low < 0) { // error checking
        low = 0;
    }
    for(size_t i = low; i < high; i++) {
        this->set_led(col, i);
    }
    FastLED.show();
}

void Leds::set_led(CRGB col, uint16_t idx) {
    if(idx >= NUM_LEDS || idx < 0) { // error checking
        return;
    } else {
        leds_[idx] = col;
    }
}

void Leds::set_brightness(uint16_t brightness) {
    if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    } else if (brightness < 0) {
        brightness = 0;
    }
    brightness_ = brightness;

    // tell the refresh operation to redraw the whole led strip
    update_ = true;
}

void Leds::set_length(uint16_t length) {
    if (length > NUM_LEDS) {
        // at most fill the whole strip with led's
        length = NUM_LEDS;
    } else if (length < 1) {
        // at least have a single led in the length
        length = 1;
    }
    length_ = length;

    // tell the refresh operation to redraw the whole led strip
    update_ = true;
}

void Leds::set_delay(uint16_t delay) {
    if (delay > MAX_DELAY) {
        // have a slowest speed. (1/4 second per )
        delay = MAX_DELAY;
    } else if (delay < 0) {
        // delay of 0 is the fastest we can go.
        delay = 0;
    }
    delay_ = delay;
}

void Leds::init_snake() {
    this->range(secondary_, 0, index_);
    this->range(primary_, index_, NUM_LEDS);
}

void Leds::snake() {
    this->set_led(secondary_, index_ - 1);
    this->set_led(primary_, index_);
    FastLED.show();
    
    if(direction_ && index_ < NUM_LEDS) {
        index_++;
    } else if(direction_ && index_ >= NUM_LEDS) {
        direction_ = false;
        index_--;
    } else if(!direction_ && index_ > 0) {
        index_--;
    } else {
        direction_ = true;
        index_++;
    }
}

void Leds::init_wave() {
    for (size_t i = 0; i < NUM_LEDS; i++) {
        if ((i + index_) % (length_ * 2) < length_) {
            this->set_led(primary_, i);
        } else {
            this->set_led(secondary_, i);
        }
    }
    FastLED.show();
}

void Leds::wave() {
    for(size_t i = 0; i <= ceil(NUM_LEDS/(float)(length_*2)); i++) {
        int16_t val = (i * length_ * 2) - index_;

        this->set_led(primary_, val);
        this->set_led(secondary_, val+length_);
    }
    FastLED.show();

    if(index_ < 0 || index_ >= (length_ * 2)) {
        index_ = 1;
    } else {
        index_++;
    }
}

void Leds::fade() {
    uint16_t pos = 40 * 10;
    float fade_step = (float)(200) / ((float)(pos) / (float)(4));

    Serial.print("p: ");
    Serial.print(pos);
    Serial.print(" f_s: ");
    Serial.print(fade_step);
    Serial.print(" v: ");


    CRGB col0;
    // initial fade
    if(index_ < pos/4) {
        Serial.print(50 + fade_step * (float)((pos/4) - index_));
        col0 = primary_;
        col0 = col0.subtractFromRGB(50 + fade_step * (float)((pos/4) - index_));
    } else if(index_ < (pos * 2)/4) {
        Serial.print(50 + fade_step * (float)(index_ - (pos/4)));
        col0 = primary_;
        col0 = col0.subtractFromRGB(50 + fade_step * (float)(index_ - (pos/4)));
    } else if(index_ < (pos * 3)/4) {
        Serial.print(50 + fade_step * (float)((pos * 3)/4 - index_));
        col0 = secondary_;
        col0 = col0.subtractFromRGB(50 + fade_step * (float)((pos * 3)/4 - index_));
    } else {
        Serial.print(50 + fade_step * (float)(index_ - (pos * 3)/4));
        col0 = secondary_;
        col0 = col0.subtractFromRGB(50 + fade_step * (float)(index_ - (pos * 3)/4));
    }

    // Set the colors
    this->range(col0, 0, NUM_LEDS);

    if( index_ < 0 || index_ >= pos) {
        index_ = 0;
    } else {
        index_++;
    }

    Serial.print(" I:");
    Serial.println(index_);
}

void Leds::flash(bool init) {
    // Set the colors
    if (index_ == 0) {
        this->range(primary_, 0, NUM_LEDS);
    } else if (index_ == 1) {
        this->black();
    } else if (index_ == 2) {
        this->range(secondary_, 0, NUM_LEDS);
    } else {
        this->black();
    }

    if(!init) {
        if(index_ < 3) {
            index_++;
        } else {
            index_ = 0;
        }
    }
}

void Leds::init_pingpong() {
    this->black();
    this->range(primary_, index_, index_ + length_);
    FastLED.show();
}

void Leds::ping_pong() {
    if (direction_) {
        this->set_led(CRGB::Black, index_ - 1);
        this->set_led(primary_, index_ + length_);

        // increment (or change direction)
        if (index_ + length_ < NUM_LEDS) {
            index_++;
        } else {
            direction_ = false;
            index_--;
        }
    } else {
        this->set_led(primary_, index_);
        this->set_led(CRGB::Black, index_ + length_);

        if(index_ > 0) {
            index_--;
        } else {
            direction_ = true;
            index_++;
        }
    }
    FastLED.show();
}

void Leds::bounce(bool alternate) {
    for(size_t i = 0; i <= ceil(NUM_LEDS/(float)(length_)); i++) {
        int16_t val = (i * (length_) - index_) - 1; //original direction
        int16_t rev = i * (length_) - (length_ - index_); // the reverse direction

        if(i % 2 == 0) {
            // all the even LED's
            if(direction_) {
                this->set_led(CRGB::Black, val + 1); // turn off led
                this->set_led(primary_, val);
            } else {
                this->set_led(CRGB::Black, val - 1); // turn off led

                if(alternate)
                    this->set_led(secondary_, val);
                else
                    this->set_led(primary_, val);
            }
        } else {
            // all the odd LED's
            if(direction_) {
                this->set_led(CRGB::Black, rev - 1); // turn off led
                this->set_led(secondary_, rev);
            } else {
                this->set_led(CRGB::Black, rev + 1); // turn off led
                
                if(alternate)
                    this->set_led(primary_, rev);
                else
                    this->set_led(secondary_, rev);
            }
            
        }
    }
    FastLED.show();

    // increment/decrement (or change direction)
    if (direction_) {
        if(index_ < (length_-1)) {
            index_++;
        } else {
            direction_ = false;
            index_--;
        }
    } else {
        if(index_ > 0) {
            index_--;
        } else {
            direction_ = true;
            index_++;
        }
    }
}

void Leds::rainbow() {
    uint16_t size = 1;
    for (size_t i = 0; i < NUM_LEDS; i++) {
        leds_[i] = CHSV(i - (index_ * (2 * size)), 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
    }
    FastLED.show();

    if(index_ < (255/size)) {
        index_++;
    } else {
        index_ = 0;
    }
}

void Leds::random_colors() {
    //                  Red            green          yellow           pink               blue               white orange
    // CRGB colors = {CRGB::Red, CRGB(0,200,0), CRGB(255,255,0), CRGB(255,192,203), CRGB(0,100,255), CRGB::White};
    CRGB colors[10] = {CRGB::Red, CRGB::DarkGreen, CRGB::White, CRGB::Cyan, CRGB::LightGreen, CRGB::Orange, CRGB::Yellow, CRGB::Pink, CRGB::PowderBlue, CRGB::MidnightBlue};

    for(size_t i = 0; i < NUM_LEDS; i++) {
        leds_[i] = colors[random(0,10)];
    }
    FastLED.show();
}