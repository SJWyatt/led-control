#include "Interface.h"

Interface::Interface(Keypad* lcd) {
    lcd_ = lcd;
}

Interface::Interface(Keypad* lcd, Leds* lights) {
    lcd_ = lcd;
    lights_ = lights;
    lights_->set_state(Gravity);
}

void Interface::draw() {
    lights_->refresh();
    switch (state_) {
        case Menu:
            menu();
            arrows();
            options();
            break;
        case Settings:
            menu();
            arrows();
            settings();
            break;
        case Brightness:
            brightness();
            break;
        case Speed:
            speed();
            break;
        case Primary:
            primary();
            break;
        case Secondary:
            secondary();
            break;
        default:
            break;
    }
}

void Interface::menu() {
    if (state_ != lastStateWritten_) {
        // 
        lcd_->clear();
        lcd_->setCursor(0,0);
        
        switch(state_) {
            case Menu:
                lcd_->write("Menu:");
                break;
            case Settings:
                lcd_->write("Settings:");
                break;
            case Brightness:
                lcd_->print(settings_[0]);
                break;
            case Speed:
                lcd_->print(settings_[1]);
                break;
            case Primary:
                lcd_->print(settings_[2]);
                break;
            case Secondary:
                lcd_->print(settings_[3]);
                break;
            default:
                break;
        }
        
        lastStateWritten_ = state_;
    }
}

void Interface::arrows() {
    if ((last_changed_ + ARR_DELAY) < millis()) {
        if(flash_) {
            lcd_->setCursor(0,1);
            lcd_->write(127);

            lcd_->setCursor(15,1);
            lcd_->write(126);
        } else {
            lcd_->setCursor(0,1);
            lcd_->write(' ');

            lcd_->setCursor(15,1);
            lcd_->write(' ');
        }
        flash_ = !flash_;
        last_changed_ = millis();
    }
}

void Interface::options() {
    if (prevOption_ != curOption_) {
        // clear this section of the lcd
        lcd_->setCursor(2, 1);
        for(size_t i = 0; i < 14; i++) {
            lcd_->write(' ');
        }

        lcd_->setCursor(1 + (14 - options_[curOption_].length()) / 2, 1);
        lcd_->print(options_[curOption_]);

        prevOption_ = curOption_;
    }
}

uint8_t Interface::next_option() {
    curOption_++;

    if (curOption_ >= numOptions_) {
        curOption_ = 0;
    }

    options();

    return curOption_;
}

uint8_t Interface::prev_option() {
    if (curOption_ > 0) {
        curOption_--;
    } else {
        curOption_ = numOptions_ - 1;
    }

    options();

    return curOption_;
}

uint8_t Interface::set_option(uint8_t option) {
    if (option > 0 && option < (numOptions_-1)) {
        curOption_ = option;
    }

    options();

    return curOption_;
}

void Interface::settings() {
    if (prevSetting_ != curSetting_) {
        check_setting(); //change setting if invalid

        // clear this section of the lcd
        lcd_->setCursor(2, 1);
        for(size_t i = 0; i < 14; i++) {
            lcd_->write(' ');
        }

        lcd_->setCursor(1 + (14 - settings_[curSetting_].length()) / 2, 1);
        lcd_->print(settings_[curSetting_]);

        prevSetting_ = curSetting_;
    }
}

uint8_t Interface::next_setting() {
    curSetting_++;

    if (curSetting_ >= numSettings_) {
        curSetting_ = 0;
    }

    check_setting(); //change setting if invalid
    settings();

    return curSetting_;
}

uint8_t Interface::prev_setting() {
    if (curSetting_ > 0) {
        curSetting_--;
    } else {
        curSetting_ = numSettings_ - 1;
    }

    check_setting(); // change setting if invalid.
    settings();

    return curSetting_;
}

uint8_t Interface::set_setting(uint8_t setting) {
    if (setting > 0 && setting < (numSettings_-1)) {
        curSetting_ = setting;
    }

    settings();

    return curSetting_;
}

void Interface::check_setting() {
    switch(stateSettings_[curOption_]) {
        case All: // Do nothing, all settings are displayed
            break;
        case Brightness: //Only show brightness setting
            curSetting_ = 0; // TODO: change from static number.
            break;
        case Primary:
            curSetting_ = 2;
            break;
        case None: // Nothing should be displayed.
            // TODO: check that this is an unreachable state
            break;
        default:
            break;
    }
}

void Interface::brightness() {
    if (selectedBrightness_ != lights_->get_brightness()) {
        // check_setting(); //change setting if invalid

        // clear this section of the lcd
        lcd_->setCursor(2, 1);
        for(size_t i = 0; i < 14; i++) {
            lcd_->write(' ');
        }

        // lcd_->setCursor(1 + (14 - settings_[curSetting_].length()) / 2, 1);
        // lcd_->print(settings_[curSetting_]);

        // Check brightness value
        // lights_->set_brightness(selectedBrightness_);
    }
}

void Interface::speed() {

}

void Interface::primary() {

}

void Interface::secondary() {

}

void Interface::select() {
    switch (state_) {
        case Menu:
            if(stateSettings_[curOption_] != None) {
                state_ = Settings;

                prevSetting_ = curSetting_ - 1; // to force refresh
            }
            lights_->set_state(ledOptions_[curOption_]);
            break;
        case Settings:
            // rtnState_ = state_;
            state_ = setState_[curSetting_];
            break;
        case Brightness:
            break;
        case Speed:
            break;
        case Primary:
            break;
        case Secondary:
            break;
        default:
            break;
    }
}

void Interface::down() {
    switch (state_) {
        case Menu:
            break;
        case Settings:
            break;
        case Brightness:
            break;
        case Speed:
            break;
        case Primary:
            break;
        case Secondary:
            break;
        default:
            break;
    }
}

void Interface::up() {
    switch (state_) {
        case Menu:
            break;
        case Settings:
            state_ = Menu;
            prevOption_ = curOption_ - 1; // to force refresh
            break;
        case Brightness:
            state_ = Settings;
            break;
        case Speed:
            state_ = Settings;
            break;
        case Primary:
            state_ = Settings;
            break;
        case Secondary:
            state_ = Settings;
            break;
        default:
            break;
    }
}

void Interface::next() {
    switch (state_) {
        case Menu:
            next_option();
            break;
        case Settings:
            next_setting();
            break;
        case Brightness:
            break;
        case Speed:
            break;
        case Primary:
            break;
        case Secondary:
            break;
        default:
            break;
    }
}

void Interface::prev() {
    switch (state_) {
        case Menu:
            prev_option();
            break;
        case Settings:
            prev_setting();
            break;
        case Brightness:
            break;
        case Speed:
            break;
        case Primary:
            break;
        case Secondary:
            break;
        default:
            break;
    }
}
