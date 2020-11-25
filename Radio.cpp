#ifndef RADIO_CPP
#define RADIO_CPP

#include "Radio.h"
#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
#include "Message.h"

// Radio::Radio() {
//     radio_ = new RF24(A1, A2);
// }

Radio::Radio(uint8_t CE, uint8_t CSE) {
    radio_ = new RF24(CE, CSE);
}

Radio::Radio(uint8_t CE, uint8_t CSE, Leds* lights) {
    radio_ = new RF24(CE, CSE);
    lights_ = lights;
}

void Radio::begin() {
    // Set the addresses
    for(uint8_t i = 0; i < 6; i++) {
        address_[0][i] = i;
        address_[1][i] = i+6;
    }

    connMsg_.setPayload(0x51A4, Message::Initialize);
    ackMsg_.setPayload(0xAC40, Message::Initialize);
    okMsg_.setPayload(0xC055, Message::Initialize);

    // setup nRF24 chip
    radio_->begin();

    // Set the signal level to low (For high attach a decoupling capacitor)
    radio_->setPALevel(RF24_PA_LOW);
    
    // Use the Ack Payload feature
    radio_->enableAckPayload();
    radio_->enableDynamicPayloads();
}

void Radio::connect() {
    // Communicate on the correct channels
    radio_->stopListening();
    if(mode_ == 0) { // Master
        radio_->openWritingPipe(address_[0]);
        radio_->openReadingPipe(1, address_[1]);
    } else { // Slave
        radio_->openWritingPipe(address_[1]);
        radio_->openReadingPipe(1, address_[0]);
    }

    // Start listening for data
    radio_->startListening();
}

void Radio::listen() {
    switch(state_) {
        case Disconnected:
            #ifdef DEBUG_RADIO
                Serial.println("Broadcasting...");
            #endif

            // Start broadcasting connection information.
            radio_->stopListening();
            radio_->openWritingPipe(address_[0]); // Send as the master.
            radio_->openReadingPipe(1, address_[1]); // Listen to the slave
            delay(1);

            // Write a connection request
            radio_->startWrite(&connMsg_, sizeof(connMsg_), 0);
            #ifdef DEBUG_RADIO
                sendTime_ = micros();
            #endif

            state_ = Initializing;
            timeEnteredState_ = millis();
            break;
        case Initializing:
            // Wait for ack.
            if(msgAvail_ && lastMessage_ == ackMsg_) {
                #ifdef DEBUG_RADIO
                    Serial.println("ACK recieved!");
                #endif

                msgAvail_ = false;

                radio_->stopListening();
                delay(1);

                radio_->startWrite(&okMsg_, sizeof(okMsg_), 0);
                #ifdef DEBUG_RADIO
                    sendTime_ = micros();
                #endif

                // We have successfully connected.
                mode_ = 0;
                state_ = Connected;
                timeEnteredState_ = millis();

                // connect();

                #ifdef DEBUG_RADIO
                    // Serial.println("Conn (M)");
                    Serial.println('M');
                #endif
                break;
            }
            
            // Leave state after timeout
            if(millis() - timeEnteredState_ > ACK_TIMEOUT) {
                radio_->stopListening();
                radio_->openWritingPipe(address_[1]); // Send as slave.
                radio_->openReadingPipe(1, address_[0]); // Listen to master
                delay(1);

                
                state_ = Listen;
                listenTimeout_ = random(1000, 4000); // set random amout of time to listen for.
                timeEnteredState_ = millis();
                radio_->startListening();

                #ifdef DEBUG_RADIO
                    Serial.println("ACK timeout!");
                #endif
            }

            break;
        case Listen:
            // Listen for connection start packet.
            if(msgAvail_ && lastMessage_ == connMsg_) {
                #ifdef DEBUG_RADIO
                    Serial.println("Broadcast recieved!");
                #endif
                msgAvail_ = false;

                radio_->stopListening();
                delay(1);

                radio_->startWrite(&ackMsg_, sizeof(ackMsg_), 0);
                #ifdef DEBUG_RADIO
                    sendTime_ = micros();
                #endif

                // Wait for reply
                state_ = Connect;
                timeEnteredState_ = millis();
            }

            // Start broadcasting again after a delay
            if(millis() - timeEnteredState_ > listenTimeout_) {
                state_ = Disconnected;
                timeEnteredState_ = millis();
            }
            
            break;
        case Connect:
            // Wait for ok.
            if(msgAvail_ && lastMessage_ == okMsg_) {
                msgAvail_ = false;
                
                // We have successfully connected.
                mode_ = 1;
                state_ = Connected;
                timeEnteredState_ = millis();

                #ifdef DEBUG_RADIO
                    Serial.println('S');
                #endif
                break;
            }
            
            // After timeout, continue listening
            if(millis() - timeEnteredState_ > OK_TIMEOUT) {
                state_ = Listen;
                timeEnteredState_ = millis();
            }
            break;
        case Connected:
            if(msgAvail_) {
                if(lastMessage_.getType() == Message::Ping) {
                    // Send reply
                    state_ = Pinged;
                    timeEnteredState_ = millis();
                } else if(lastMessage_.getType() == Message::Pong) {
                    // Mark that a reply has been recieved.
                    state_ = ReplyReceived;
                    timeEnteredState_ = millis();
                } else if(lastMessage_ == connMsg_) {
                    // Someone initiated a connection, try to connect with them.
                    state_ = Listen;
                    timeEnteredState_ = millis();
                } else {
                    switch(lastMessage_.getType()) {
                        case Message::Empty:
                            break;
                        case Message::State:
                            lights_->set_state((LedState)lastMessage_.getPayload());
                            break;
                        case Message::Index:
                            lights_->set_index((uint16_t)lastMessage_.getPayload());
                            break;
                        case Message::Delay:
                            lights_->set_delay((uint16_t)lastMessage_.getPayload());
                            break;
                        case Message::Length:
                            lights_->set_length((uint16_t)lastMessage_.getPayload());
                            break;
                        case Message::Brightness:
                            lights_->set_brightness((uint16_t)lastMessage_.getPayload());
                            break;
                        case Message::Direction:
                            break;
                        case Message::PrimaryColor:
                            break;
                        case Message::SecondaryColor:
                            break;
                        case Message::AlternateColor:
                            break;
                        default:
                            #ifdef DEBUG_RADIO
                                // Just show the message
                                Serial.println(lastMessage_.getPayload());
                            #endif
                            break;
                    }
                    msgAvail_ = false;
                }
            }

            // As master, ping every 2 seconds (to check connection)
            if(mode_ == 0 && millis() - timeEnteredState_ > PING_TIME) {
                state_ = CheckConnection;
                timeEnteredState_ = millis();
            }

            // After PING_MISSES in a row, assume we are disconnected.
            if(failedPings_ > PING_MISSES) {
                #ifdef DEBUG_RADIO
                    Serial.println("Too many failed pings, assume disconnected.");
                #endif

                // Reset failed pings
                failedPings_ = 0;

                // Initiate connection again.
                state_ = Disconnected;
                timeEnteredState_ = millis();
            }
            break;
        case CheckConnection:
            #ifdef DEBUG_RADIO
                Serial.println("Pinging...");
            #endif
            // Send a ping.
            radio_->stopListening();
            delay(1);
            
            lastMessage_.setPayload(millis(), Message::Ping);

            radio_->startWrite(&lastMessage_, sizeof(lastMessage_), 0);
            #ifdef DEBUG_RADIO
                sendTime_ = micros();
            #endif

            failedPings_++;
            state_ = Connected;
            timeEnteredState_ = millis();
            break;
        case ReplyReceived:
            if(msgAvail_ && lastMessage_.getType() == Message::Pong) {
                unsigned long round_trip = millis() - lastMessage_.getPayload();
                msgAvail_ = false;

                #ifdef DEBUG_RADIO
                    Serial.print("Message::Pong recieved. (Round trip: ");
                    Serial.print(round_trip);
                    Serial.println("ms)");
                #endif
            }

            failedPings_ = 0; // Reset number of failed ping attempts.

            state_ = Connected;
            timeEnteredState_ = millis();
            break;
        case Pinged:
            // Reply if pinged
            if(msgAvail_ && lastMessage_.getType() == Message::Ping) {
                #ifdef DEBUG_RADIO
                    Serial.println("Pinged!");
                #endif

                // Reply with pong.
                radio_->stopListening();
                delay(1);

                lastMessage_.setType(Message::Pong);

                radio_->startWrite(&lastMessage_, sizeof(lastMessage_), 0);
                #ifdef DEBUG_RADIO
                    sendTime_ = micros();
                #endif

                msgAvail_ = false;
            }

            state_ = Connected;
            timeEnteredState_ = millis();
            break;
        default:
            break;
    }
}

bool Radio::connected() {
    if(state_ == Connected || state_ == CheckConnection || state_ == Pinged || state_ == ReplyReceived) {
        return true;
    }
    return false;
}
uint8_t Radio::get_mode() {
    return mode_;
}

bool Radio::send_msg(uint32_t msg, Message::DataType type) {
    //Check if connected.
    if(connected()) {
        radio_->stopListening();
        delay(1);

        // Create message.
        lastMessage_.setPayload(msg, type);
        radio_->startWrite(&lastMessage_, sizeof(lastMessage_), 0);

        return true;
    } else {
        // Error sending!
        return false;
    }
}

// bool Radio::queue_msg(uint32_t msg, DataType type) {
//     
// }

void Radio::messageReceivedISR() {
    bool tx, fail, rx;
    radio_->whatHappened(tx, fail, rx);
  
    // Check recieved data
    if(rx) {
        if(radio_->getDynamicPayloadSize() < 1) {
            // This is a flushing operation, ignore.
            return;
        }
        
        // Read data (in message format)
        Message recieved_msg;
        radio_->read(&recieved_msg, sizeof(recieved_msg));

        // Message handling logic
        if (msgAvail_) {
            if(recieved_msg.getType() == Message::Ping || recieved_msg.getType() == Message::Pong) {
                // Ignore, it's a failed ping/pong
                return;
            } else {
                #ifdef DEBUG_RADIO
                    Serial.println("Error, Overwriting previous message!");
                #endif

                lastMessage_ = recieved_msg;
            }
        } else {
            lastMessage_ = recieved_msg;
            msgAvail_ = true;
        }
    }

    // Handle completed transmission
    if(tx) {
        // Start listening for data
        radio_->startListening();
    }

    if(fail) {
        #ifdef DEBUG_RADIO
            Serial.print("Send time: ");
            Serial.print(micros() - sendTime_);
            Serial.println("us - Failed");
        #endif

        // Start listening for data
        radio_->startListening();
    }
}

#endif