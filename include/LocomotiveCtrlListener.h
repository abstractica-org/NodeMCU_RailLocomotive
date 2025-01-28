#ifndef LocomotiveCtrlListener_h
#define LocomotiveCtrlListener_h

#include <Arduino.h>

class LocomotiveCtrlListener
{
public:
    virtual void onStopped() = 0;
    virtual void onExitBlock(uint8_t blockValue, uint16_t distanceToGoal, unsigned int blockTime, uint8_t curPower) = 0;
};

#endif