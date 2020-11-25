/**************
 * 
 * Name: Message_cpp
 * Last Edited: 12/05/2018 - 13:18
 * Last Edited By: Steven
 * 
 *************/

#ifndef MESSAGE_CPP
#define MESSAGE_CPP

#include "Message.h"
#include <Arduino.h>


Message::Message() { // create an empty message
    payload_ = 0;
    type_ = Empty;
}

Message::Message(uint32_t payload, int8_t type) {
    setPayload(payload, (DataType) type);
}

Message::Message(uint32_t payload, DataType type) {
    setPayload(payload, type);
}

Message::Message(Message& Copy) {
    setPayload(Copy.getPayload(), Copy.getType());
}

void Message::setPayload(uint32_t payload, DataType type) {
    payload_ = payload;
    type_ = (int8_t) type;
}

uint32_t Message::getPayload() {
  return payload_;
}

void Message::setType(DataType type) {
    type_ = type;
}

void Message::setType(int8_t type) {
    type_ = type;
}

Message::DataType Message::getType() {
  return (DataType) type_;
}

bool Message::operator!=(Message& RHS) {
  if(payload_ != RHS.getPayload() || (DataType) type_ != RHS.getType()) {
    return true;
  }
  return false;
}

bool Message::operator==(Message& RHS) {
  if(payload_ == RHS.getPayload() && (DataType) type_ == RHS.getType()) {
    return true;
  }
  return false;
}

#endif