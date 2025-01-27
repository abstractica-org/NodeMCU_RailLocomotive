#include <Arduino.h>
#include "MotorPowerCtrl.h"

MotorPowerCtrl::MotorPowerCtrl(uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB)
{
    _pwmPin = pwmPin;
    _dirPinA = dirPinA;
    _dirPinB = dirPinB;
    _msPrPowerChange = 10;
    _lastPowerChange = 0;
    _dir = 0;
    pinMode(_pwmPin, OUTPUT);
    pinMode(_dirPinA, OUTPUT);
    pinMode(_dirPinB, OUTPUT);
    analogWrite(_pwmPin, 0);
    digitalWrite(_dirPinA, LOW);
    digitalWrite(_dirPinB, HIGH);
}

void MotorPowerCtrl::update(unsigned long curTime)
{
    if(_targetPower != _actualPower && curTime >= _lastPowerChange + _msPrPowerChange)
    {
        if(_targetPower > _actualPower)
        {
            ++_actualPower;
        }
        else
        {
            --_actualPower;
        }
        analogWrite(_pwmPin, _actualPower);
        _lastPowerChange = curTime;
    }
}

void MotorPowerCtrl::setTargetPower(uint8_t targetPower, unsigned int msPrPowerChange)
{
    _targetPower = targetPower;
    _msPrPowerChange = msPrPowerChange;
}

void MotorPowerCtrl::stop()
{
    _targetPower = 0;
    _actualPower = 0;
    analogWrite(_pwmPin, 0);
}

void MotorPowerCtrl::setDirection(bool dir)   // true = Forward, false = Backward
{
    if(_dir == dir || _actualPower != 0) return;
    _dir = dir;
    digitalWrite(_dirPinA, dir);
    digitalWrite(_dirPinB, !dir);
}

bool MotorPowerCtrl::getDirection()
{
    return _dir;
}

uint8_t MotorPowerCtrl::getActualPower()
{
    return _actualPower;
}

uint8_t MotorPowerCtrl::getTargetPower()
{
    return _targetPower;
}