#ifndef Locomotive_h
#define Locomotive_h

#include <Arduino.h>
#include <RemoteDevice.h>
#include <LEDBlinker.h>
#include "LocomotiveCtrl.h"

#define IDENTIFY_BLINK_DURATION 15000

//Server to locomotive commands
#define CMD_IDENTIFY 1000
#define CMD_MOVE 1001
#define CMD_DIRECTION 1002

//Train to server commands
#define CMD_ON_EXIT_BLOCK 2000
#define CMD_STOPPED 2001

class Locomotive : public RemoteDevice, LocomotiveCtrlListener
{
public:
    Locomotive();
    void update(unsigned long curTime);
    uint16_t onPacketReceived(uint16_t command, uint16_t arg1, uint16_t arg2, uint8_t* pData, uint16_t size);
    void onStopped();
    void onExitBlock(uint8_t blockValue, uint16_t distanceToGoal);
private:
    LocomotiveCtrl _locomotiveCtrl;
    LEDBlinker _led;
    unsigned long _ledStarted;
    unsigned long _curTime;
    bool _stopSend;
};

#endif