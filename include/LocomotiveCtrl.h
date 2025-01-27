#ifndef LocomotiveCtrl_h
#define LocomotiveCtrl_h

#include <Arduino.h>
#include "MotorPowerCtrl.h"
#include "BlockListener.h"
#include "LocomotiveCtrlListener.h"

#define LOCOMOTIVE_CTRL_STATE_STOPPED 0
#define LOCOMOTIVE_CTRL_STATE_RUNNING 1
#define LOCOMOTIVE_CTRL_STATE_STOPPING_POWERDOWN 2
#define LOCOMOTIVE_CTRL_STATE_STOPPING_WAIT 3


#define LOCOMOTIVE_CTRL_MINIMUM_SPEED 60
#define LOCOMOTIVE_CTRL_MAXIMUM_BLOCK_TIME (60000/LOCOMOTIVE_CTRL_MINIMUM_SPEED)
#define LOCOMOTIVE_CTRL_START_POWER 125

class LocomotiveCtrl : BlockListener
{
public:
    LocomotiveCtrl(LocomotiveCtrlListener* pListener);
    void update(unsigned long curTime);
    void setDirection(bool forward);
    void move(uint16_t distance);
    bool isStopped();
    void enterBlock(bool direction);
    void exitBlock(bool direction, uint8_t blockValue, unsigned long blockTime);
private:
    uint8_t getTargetPower();
    MotorPowerCtrl _powerCtrl;
    LocomotiveCtrlListener* _pListener;
    unsigned long _curTime;
    unsigned long _stoppedTime;
    unsigned long _lastActionTime;
    uint8_t _state;
    uint16_t _distanceToGoal;
    uint8_t _powerAdjust;
};

#endif