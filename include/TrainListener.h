/*
  Author: Tobias Grundtvig
*/

#ifndef TrainListener_h
#define TrainListener_h

#include <Arduino.h>


class TrainListener
{
public:
    virtual void onReadBlock(uint8_t blockValue, uint16_t distToGoal) = 0;
    virtual void onTrainStopped() = 0;
};

#endif