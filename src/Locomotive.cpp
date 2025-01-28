#include <Arduino.h>
#include "Locomotive.h"

Locomotive::Locomotive() : RemoteDevice(ESP.getChipId(), "Locomotive", 1), 
    _locomotiveCtrl(this),
    _led(LED_BUILTIN, false, 100, 300, 1000)
{
    _ledStopTime = 0;
    _curTime = 0;
    _stopSend = true;
}

void Locomotive::update(unsigned long curTime)
{
    _curTime = curTime;
    RemoteDevice::update(curTime);
    _locomotiveCtrl.update(curTime);
    _led.update(curTime);
    if(_led.isBlinking() && curTime > _ledStopTime)
    {
        _led.stopBlink();
    }
    if(!_stopSend)
    {
        _stopSend = (sendPacketToServer(CMD_STOPPED, 0, 0, 0, 0) != 0);
    }
}

void Locomotive::onStopped()
{
    _stopSend = (sendPacketToServer(CMD_STOPPED, 0, 0, 0, 0) != 0);
}

void Locomotive::onExitBlock(uint8_t blockValue, uint16_t distanceToGoal, unsigned int blockTime, uint8_t curPower)
{
    uint16_t bt = blockTime < 3000 ? ((uint16_t) blockTime) : 0;
    sendPacketToServer(CMD_ON_EXIT_BLOCK, blockValue, distanceToGoal, bt, curPower, true, true);
}

uint16_t Locomotive::onPacketReceived(uint16_t command, uint16_t arg1, uint16_t arg2, uint16_t arg3, uint16_t arg4, uint8_t* pData, uint16_t size)
{
    switch(command)
    {
        case CMD_IDENTIFY:
            _led.startBlink(arg1);
            _ledStopTime = _curTime + ((unsigned long) arg2) * 1000;
            return 0;
        case CMD_MOVE:
            if(arg1 == 0) return 0;
            _locomotiveCtrl.move(arg1);
            return 0;
        case CMD_DIRECTION:
            if(!_locomotiveCtrl.isStopped()) return 1;
            if(arg1 != 0 && arg1 != 1) return 2;
            _locomotiveCtrl.setDirection(arg1 == 0);
            return 0;
        default:
            return 101; 
    }
}