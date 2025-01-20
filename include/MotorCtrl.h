/*
  Author: Tobias Grundtvig
*/

#ifndef MotorCtrl_h
#define MotorCtrl_h

#include <Arduino.h>

class MotorCtrl
{
public:
    MotorCtrl(uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB, unsigned long msPrPowerChange);
    void update(unsigned long curTime);
    void setTargetPower(uint8_t targetPower);
    void stop();
    void setDirection(uint8_t dir); // 0 = Forward, 1 = Backward
    uint8_t getDirection();
    uint8_t getActualPower();
    uint8_t getTargetPower();
private:
    uint8_t _pwmPin;
    uint8_t _dirPinA;
    uint8_t _dirPinB;
    unsigned long _msPrPowerChange;
    
    uint8_t _targetPower;
    uint8_t _actualPower;
    uint8_t _dir;
    unsigned long _lastPowerChange;
};


#endif