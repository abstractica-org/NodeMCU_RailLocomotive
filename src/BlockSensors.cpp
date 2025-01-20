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
volatile uint8_t BlockSensors::_curDirection;
volatile bool BlockSensors::_onBlock;
volatile bool BlockSensors::_leftBlock;
volatile bool BlockSensors::_rightBlock;
volatile uint8_t BlockSensors::_blockCount;
uint8_t BlockSensors::_lastBlockCount;

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
    _curDirection = 0;
    _onBlock = false;
    _blockCount = 0;
    _lastBlockCount = 0;

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
    if(_blockCount != _lastBlockCount)
    {
        _lastBlockCount = _blockCount;
        uint8_t blockValue = 0;
        if(_leftBlock)
        {
            _leftBlock = false;
            blockValue += 1;
        } 
        if(_rightBlock)
        {
            _rightBlock = false;
            blockValue += 2;
        }
        _pListener->onBlock(blockValue, curTime);
    }
}

void BlockSensors::setDirection(uint8_t dir)
{
    if(dir == _curDirection) return;
    if(_onBlock)
    {
        _onBlock = false;
        _leftBlock = false;
        _rightBlock = false;
    }
    else
    {
        uint8_t pin = _curDirection == 0 ? _backSensorPin : _frontSensorPin;
        if(digitalRead(pin))
        {
            _onBlock = true;
            _leftBlock = false;
            _rightBlock = false;
        }
    }
}

//block sensors interrupt callbacks
void IRAM_ATTR BlockSensors::_onFrontSensor()
{
    if (_curDirection == 0 && !_onBlock)
    {
        _onBlock = true;
         _leftBlock = false;
        _rightBlock = false;
    }
    else if (_curDirection == 1 && _onBlock)
    {
        _onBlock = false;
        ++_blockCount;
    }
}

void IRAM_ATTR BlockSensors::_onBackSensor()
{
    if (_curDirection == 1 && !_onBlock)
    {
        _onBlock = true;
        _leftBlock = false;
        _rightBlock = false;
    }
    else if (_curDirection == 0 && _onBlock)
    {
        _onBlock = false;
        ++_blockCount;
    }
}

void IRAM_ATTR BlockSensors::_onLeftSensor()
{
    _leftBlock = true;
}

void IRAM_ATTR BlockSensors::_onRightSensor()
{
    _rightBlock = true;
}
