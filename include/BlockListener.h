/*
  Author: Tobias Grundtvig
*/

#ifndef BlockListener_h
#define BlockListener_h

#include <Arduino.h>


class BlockListener
{
public:
    virtual void onBlock(uint8_t blockValue, unsigned long curTime) = 0;
};

#endif