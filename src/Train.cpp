#include <Arduino.h>
#include "Train.h"

Train::Train
(
    uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB, unsigned long msPrPowerChange,
    uint8_t frontSensorPin, uint8_t backSensorPin, uint8_t leftSensorPin, uint8_t rightSensorPin
) : RemoteDevice(ESP.getChipId(), "Locomotive", 1), 
    _trainCtrl(pwmPin, dirPinA, dirPinB, msPrPowerChange, frontSensorPin, backSensorPin, leftSensorPin, rightSensorPin, this),
    _led(LED_BUILTIN, false, 100, 300, 1000)
{
    _isStopped = true;
    _isStoppedSent = true;
}

void Train::update(unsigned long curTime)
{
    _curTime = curTime;
    RemoteDevice::update(curTime);
    _trainCtrl.update(curTime);
    _led.update(curTime);
    if(_led.isBlinking() && curTime > _ledStarted + IDENTIFY_BLINK_DURATION)
    {
        _led.stopBlink();
    }
    if(_isStopped && !_isStoppedSent)
    {
        _isStoppedSent = (sendPacketToServer(CMD_STOPPED, 0, 0) != 0);
    }
}

void Train::onReadBlock(uint8_t blockValue, uint16_t distToGoal)
{
    sendPacketToServer(CMD_ON_BLOCK, blockValue, distToGoal, true, true);
}

void Train::onTrainStopped()
{
    _isStopped = true;
    _isStoppedSent = (sendPacketToServer(CMD_STOPPED, 0, 0) != 0);
}

uint16_t Train::onPacketReceived(uint16_t command, uint16_t arg1, uint16_t arg2, uint8_t* pData, uint16_t size)
{
    switch(command)
    {
        case CMD_IDENTIFY:
            Serial.print("Identify: ");
            Serial.println(arg1);
            _led.startBlink(arg1);
            _ledStarted = _curTime;
            return 0;
        case CMD_MOVE:
            Serial.print("Move: ");
            Serial.println(arg1);
            if(arg1 == 0) return 0;
            _trainCtrl.move(arg1);
            _isStopped = false;
            return 0;
        case CMD_DIRECTION:
            Serial.print("Direction: ");
            Serial.println(arg1);
            if(!_isStopped) return 1;
            if(arg1 != 0 && arg1 != 1) return 2;
            _trainCtrl.setDirection(arg1 == 0);
            return 0;
        default:
            return 101; 
    }
}