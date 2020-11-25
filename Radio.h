/**************
 * 
 * Name: Radio.h
 * Description: Class to handle radio communications with other light strips.
 * 
 *************/

#ifndef RADIO_H
#define RADIO_H

#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
#include "Message.h"

// #define DEBUG

#define ACK_TIMEOUT 300
#define OK_TIMEOUT 300
#define PING_TIME 2000
#define PING_MISSES 3

typedef enum {
    Disconnected,
    Initializing,
    Listen,
    Connect,
    Connected,
    CheckConnection,
    Pinged,
    ReplyReceived
} RadioState;

class Radio {
    public:
        Radio();
        Radio(uint8_t CE, uint8_t CSE);

        void begin();
        void connect();

        void listen();

        // bool send_config();

        void messageReceivedISR();

    private:
        RF24* radio_;

        uint8_t state_ = Disconnected; //using uint8_t instead of RadioState to save space.
        unsigned long timeEnteredState_ = millis();
        uint8_t failedPings_ = 0;

        uint8_t address_[2][6]; // = {"BRCST", "LISTN"};

        uint16_t listenTimeout_ = random(1000, 2000);
        Message connMsg_;
        Message ackMsg_;
        Message okMsg_;

        bool msgAvail_ = false;

        Message lastMessage_;
        // Message sendMsg_;

        #ifdef DEBUG
            unsigned long sendTime_ = 0;
        #endif

        bool mode_ = 1; // 0 - send, 1 - listen
};

#endif