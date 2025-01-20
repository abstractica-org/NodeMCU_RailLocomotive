#include <Arduino.h>
#include "MotorCtrl.h"

MotorCtrl::MotorCtrl(uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB, unsigned long msPrPowerChange)
{
    _pwmPin = pwmPin;
    _dirPinA = dirPinA;
    _dirPinB = dirPinB;
    _msPrPowerChange = msPrPowerChange;
    _lastPowerChange = 0;
    _dir = 0;
    pinMode(_pwmPin, OUTPUT);
    pinMode(_dirPinA, OUTPUT);
    pinMode(_dirPinB, OUTPUT);
    analogWrite(_pwmPin, 0);
    digitalWrite(_dirPinA, LOW);
    digitalWrite(_dirPinB, HIGH);
}

void MotorCtrl::update(unsigned long curTime)
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

void MotorCtrl::setTargetPower(uint8_t targetPower)
{
    _targetPower = targetPower;
}

void MotorCtrl::stop()
{
    _targetPower = 0;
    _actualPower = 0;
    analogWrite(_pwmPin, 0);
}

void MotorCtrl::setDirection(uint8_t dir)   // 0 = Forward, 1 = Backward
{
    if(_dir == dir || _actualPower != 0) return;
    if(dir == 0)
    {
        Serial.println("Setting motor dir = 0");
        digitalWrite(_dirPinA, LOW);
        digitalWrite(_dirPinB, HIGH);
        _dir = 0;       
    }
    else if(dir == 1)
    {
        Serial.println("Setting motor dir = 1");
        digitalWrite(_dirPinA, HIGH);
        digitalWrite(_dirPinB, LOW);
        _dir = 1;
    }
}

uint8_t MotorCtrl::getDirection()
{
    return _dir;
}

uint8_t MotorCtrl::getActualPower()
{
    return _actualPower;
}

uint8_t MotorCtrl::getTargetPower()
{
    return _targetPower;
}