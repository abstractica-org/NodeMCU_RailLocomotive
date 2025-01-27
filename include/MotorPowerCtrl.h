/*
  Author: Tobias Grundtvig
*/

#ifndef MotorPowerCtrl_h
#define MotorPowerCtrl_h

#include <Arduino.h>

class MotorPowerCtrl
{
public:
    MotorPowerCtrl(uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB);
    void update(unsigned long curTime);
    void setTargetPower(uint8_t targetPower, unsigned int msPrPowerChange);
    void stop();
    void setDirection(bool forward); // true = Forward, false = Backward
    bool getDirection();
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