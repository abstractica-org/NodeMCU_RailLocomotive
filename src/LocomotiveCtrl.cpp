#include <Arduino.h>
#include "LocomotiveCtrl.h"
#include "BlockSensors.h"

#define MOTOR_PWM_PIN D8
#define MOTOR_DIR_PIN_A D2
#define MOTOR_DIR_PIN_B D3
#define FRONT_SENSOR_PIN D1
#define BACK_SENSOR_PIN D5
#define LEFT_SENSOR_PIN D6
#define RIGHT_SENSOR_PIN D7

LocomotiveCtrl::LocomotiveCtrl(LocomotiveCtrlListener* pListener) : _powerCtrl(MOTOR_PWM_PIN, MOTOR_DIR_PIN_A, MOTOR_DIR_PIN_B)
{
    BlockSensors::begin(FRONT_SENSOR_PIN, BACK_SENSOR_PIN, LEFT_SENSOR_PIN, RIGHT_SENSOR_PIN, this);
    _pListener = pListener;
    _state = LOCOMOTIVE_CTRL_STATE_STOPPED;
    _distanceToGoal = 0;
    _powerAdjust = 40;
    _lastActionTime = 0;
    _stoppedTime = 0;
}

void LocomotiveCtrl::update(unsigned long curTime)
{
    //Save curTime
    _curTime = curTime;

    //Update sub-systems
    BlockSensors::update(curTime);
    _powerCtrl.update(curTime);

    //Adjust power if we might have come to a halt 
    if( _state == LOCOMOTIVE_CTRL_STATE_RUNNING &&
        _curTime > _lastActionTime + LOCOMOTIVE_CTRL_MAXIMUM_BLOCK_TIME &&
        _powerCtrl.getActualPower() == _powerCtrl.getTargetPower() &&
        _powerCtrl.getTargetPower() < LOCOMOTIVE_CTRL_START_POWER)
    {
        uint8_t curPower = _powerCtrl.getTargetPower();
        _powerCtrl.setTargetPower(curPower+1, 1);
        _lastActionTime = _curTime;
    }
    //Handle stopping
    else if(_state == LOCOMOTIVE_CTRL_STATE_STOPPING_POWERDOWN && _powerCtrl.getActualPower() == 0)
    {
        _stoppedTime = _curTime;
        _state = LOCOMOTIVE_CTRL_STATE_STOPPING_WAIT;
    }
    else if(_state == LOCOMOTIVE_CTRL_STATE_STOPPING_WAIT && _curTime > _stoppedTime + 1000)
    {
        _state == LOCOMOTIVE_CTRL_STATE_STOPPED;
        _pListener->onStopped();
    }
}

void LocomotiveCtrl::setDirection(bool forward)
{
    if(_state == LOCOMOTIVE_CTRL_STATE_STOPPED)
    {
        BlockSensors::setDirection(forward);
        _powerCtrl.setDirection(forward);
    }
}

void LocomotiveCtrl::move(uint16_t distance)
{
    _distanceToGoal += distance;
    if(_state == LOCOMOTIVE_CTRL_STATE_RUNNING)
    {
        _powerCtrl.setTargetPower(getTargetPower(), 20);
    }
    else
    {
        _powerCtrl.setTargetPower(LOCOMOTIVE_CTRL_START_POWER, 20);
        _state = LOCOMOTIVE_CTRL_STATE_RUNNING;
    }
}

bool LocomotiveCtrl::isStopped()
{
    return _state == LOCOMOTIVE_CTRL_STATE_STOPPED;
}

void LocomotiveCtrl::enterBlock(bool direction)
{
    _lastActionTime = _curTime;
}

void LocomotiveCtrl::exitBlock(bool direction, uint8_t blockValue, unsigned long blockTime)
{
    _lastActionTime = _curTime;
    --_distanceToGoal;
    if(_distanceToGoal == 0)
    {
        //Adjust the final speed before a stop
        if(blockTime > LOCOMOTIVE_CTRL_MAXIMUM_BLOCK_TIME)
        {
            ++_powerAdjust;
        }
        else if(blockTime < LOCOMOTIVE_CTRL_MAXIMUM_BLOCK_TIME)
        {
            --_powerAdjust;
        }
        //Stop the locomotive
        _powerCtrl.setTargetPower(0, 2);
        _state = LOCOMOTIVE_CTRL_STATE_STOPPING_POWERDOWN;
    }
    else
    {
        _powerCtrl.setTargetPower(getTargetPower(), 10);
    }
    _pListener->onExitBlock(blockValue, _distanceToGoal);
}

uint8_t LocomotiveCtrl::getTargetPower()
{
    if(_distanceToGoal > 25) return 255;
    uint16_t power = _distanceToGoal * 10 + _powerAdjust;
    if(power > 255) return 255;
    return (uint8_t) power;
}