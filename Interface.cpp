#include "Interface.h"

Interface::Interface(Keypad* lcd) {
    lcd_ = lcd;
}

void Interface::draw() {
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
        default:
            break;
    }
}

DisplayMode Interface::menu() {
    if (state_ != lastState_) {
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
            default:
                break;
        }
        
        lastState_ = state_;
    }

    return this->state_;
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

    if (curOption_ >= 7) {
        curOption_ = 0;
    }

    options();

    return curOption_;
}

uint8_t Interface::prev_option() {
    if (curOption_ > 0) {
        curOption_--;
    } else {
        curOption_ = 6;
    }

    options();

    return curOption_;
}

uint8_t Interface::set_option(uint8_t option) {
    if (option > 0 && option < 7) {
        curOption_ = option;
    }

    options();

    return curOption_;
}

void Interface::settings() {
    if (prevSetting_ != curSetting_) {
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

    if (curSetting_ >= 4) {
        curSetting_ = 0;
    }

    settings();

    return curSetting_;
}

uint8_t Interface::prev_setting() {
    if (curSetting_ > 0) {
        curSetting_--;
    } else {
        curSetting_ = 4;
    }

    settings();

    return curSetting_;
}

uint8_t Interface::set_setting(uint8_t setting) {
    if (setting > 0 && setting < 7) {
        curSetting_ = setting;
    }

    settings();

    return curSetting_;
}

void Interface::select() {
    switch (state_) {
        case Menu:
            if(curOption_ > 0 && curOption_ < 4) {
                state_ = Settings;
            }
            break;
        case Settings:
            
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
        default:
            break;
    }
}