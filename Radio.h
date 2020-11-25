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
#include "Leds.h"

// #define DEBUG_RADIO

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
        // Radio();
        Radio(uint8_t CE, uint8_t CSE);
        Radio(uint8_t CE, uint8_t CSE, Leds* lights);

        void begin();
        void connect();

        void listen();

        bool connected();
        uint8_t get_mode();
        bool send_msg(uint32_t msg, Message::DataType type);
        // bool queue_msg(uint32_t payload, DataType type);

        void messageReceivedISR();

    private:
        RF24* radio_;
        Leds* lights_;

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

        #ifdef DEBUG_RADIO
            unsigned long sendTime_ = 0;
        #endif

        uint8_t mode_ = 1; // 0 - send, 1 - listen

        // message queue???
};

#endif