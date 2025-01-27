/*
  Author: Tobias Grundtvig
*/

#ifndef BlockListener_h
#define BlockListener_h

#include <Arduino.h>


class BlockListener
{
public:
    virtual void enterBlock(bool direction) = 0;
    virtual void exitBlock(bool direction, uint8_t blockValue, unsigned long blockTime) = 0;
};

#endif