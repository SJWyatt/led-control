/*******************
 * 
 * Name: Message
 * Description: A message format for sending led control config.
 * 
 *******************/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>

class Message {
    public:
        typedef enum {
            Ping=-2,
            Pong=-1,
            Empty=0,
            Index,
            Delay,
            Length,
            Brightness,
            Direction,
            PrimaryColor,
            SecondaryColor,
            AlternateColor,
            // Gravity parameters?

            Initialize=42,
            Address=24
        } DataType;

        Message(); // create an empty message
        Message(uint32_t payload, int8_t type); //create message
        Message(uint32_t payload, DataType type); //create message
        Message(Message& Copy); // Copy constructor

        void setPayload(uint32_t payload, DataType type);
        void setType(int8_t type);
        void setType(DataType type);
        
        uint32_t getPayload();
        DataType getType();
        
        bool operator!=(Message& RHS);
        bool operator==(Message& RHS);

    private:
        uint32_t payload_; // message payload
        int8_t type_ = Empty; // message type (Using enum DataType, but casting as int8_t)
        // Checksum? Parity? ...
};

#endif
