/*
  Author: Tobias Grundtvig
*/
    
#include <Arduino.h>
#include "BlockSensors.h"

uint8_t BlockSensors::_frontSensorPin;
uint8_t BlockSensors::_backSensorPin;
uint8_t BlockSensors::_leftSensorPin;
uint8_t BlockSensors::_rightSensorPin;
BlockListener* BlockSensors::_pListener;

volatile bool BlockSensors::_frontActivated;
volatile bool BlockSensors::_backActivated;
volatile bool BlockSensors::_leftActivated;
volatile bool BlockSensors::_rightActivated;

bool BlockSensors::_forward;

//Keeping track of blocks
bool BlockSensors::_onBlock;
unsigned long BlockSensors::_enterBlockTime;

void BlockSensors::begin
(
    uint8_t frontSensorPin,
    uint8_t backSensorPin,
    uint8_t leftSensorPin,
    uint8_t rightSensorPin,
    BlockListener* pListener
)                          
{
    _frontSensorPin = frontSensorPin;
    _backSensorPin = backSensorPin;
    _leftSensorPin = leftSensorPin;
    _rightSensorPin = rightSensorPin;
    _pListener = pListener;
    _forward = true;
    _onBlock = false;

    pinMode(_frontSensorPin, INPUT_PULLUP);
    pinMode(_backSensorPin, INPUT_PULLUP);
    pinMode(_leftSensorPin, INPUT_PULLUP);
    pinMode(_rightSensorPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_frontSensorPin), BlockSensors::_onFrontSensor, FALLING);
    attachInterrupt(digitalPinToInterrupt(_backSensorPin), BlockSensors::_onBackSensor, FALLING);
    attachInterrupt(digitalPinToInterrupt(_leftSensorPin), BlockSensors::_onLeftSensor, FALLING);
    attachInterrupt(digitalPinToInterrupt(_rightSensorPin), BlockSensors::_onRightSensor, FALLING);
}

void BlockSensors::update(unsigned long curTime)
{
    if(_forward)
    {
        if(_frontActivated)
        {
            _frontActivated = false;
            if(!_onBlock)
            {
                _enterBlockTime = curTime;
                _onBlock = true;
                _leftActivated = false;
                _rightActivated = false;
                _pListener->enterBlock(_forward);
            }
        }
        if(_backActivated)
        {
            _backActivated = false;
            if(_onBlock)
            {
                unsigned long blockTime = curTime - _enterBlockTime;
                _onBlock = false;
                uint8_t blockValue = 0;
                if(_leftActivated)
                {
                    blockValue += 1;
                }
                if(_rightActivated)
                {
                    blockValue += 2;
                }
                _leftActivated = false;
                _rightActivated = false;
                _pListener->exitBlock(_forward, blockValue, blockTime);
            }
        }
    }
    else
    {
        if(_backActivated)
        {
            _backActivated = false;
            if(!_onBlock)
            {
                _enterBlockTime = curTime;
                _onBlock = true;
                _leftActivated = false;
                _rightActivated = false;
                _pListener->enterBlock(_forward);
            }
        }
        if(_frontActivated)
        {
            _frontActivated = false;
            if(_onBlock)
            {
                unsigned long blockTime = curTime - _enterBlockTime;
                _onBlock = false;
                uint8_t blockValue = 0;
                if(_leftActivated)
                {
                    blockValue += 1;
                }
                if(_rightActivated)
                {
                    blockValue += 2;
                }
                _leftActivated = false;
                _rightActivated = false;
                _pListener->exitBlock(_forward, blockValue, blockTime);
            }
        }
    }
}

void BlockSensors::setDirection(bool forward)
{
    _forward = forward;
}

//block sensors interrupt callbacks
void IRAM_ATTR BlockSensors::_onFrontSensor()
{
    _frontActivated = true;
}

void IRAM_ATTR BlockSensors::_onBackSensor()
{
    _backActivated = true;
}

void IRAM_ATTR BlockSensors::_onLeftSensor()
{
   _leftActivated = true;
}

void IRAM_ATTR BlockSensors::_onRightSensor()
{
    _rightActivated = true;
}
