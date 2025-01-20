/*
  Author: Tobias Grundtvig
*/

#ifndef TrainCtrl_h
#define TrainCtrl_h

#include <Arduino.h>
#include "MotorCtrl.h"
#include "BlockSensors.h"
#include "BlockListener.h"
#include "TrainListener.h"

class TrainCtrl : BlockListener
{
public:
    TrainCtrl
    (
      uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB, unsigned long msPrPowerChange,
      uint8_t frontSensorPin, uint8_t backSensorPin, uint8_t leftSensorPin, uint8_t rightSensorPin,
      TrainListener* pTrainListener
    );
    void update(unsigned long curTime);
    void move(uint16_t dist);
    void setDirection(bool forward);
    void onBlock(uint8_t blockValue, unsigned long blockTime);
private:
    uint8_t getTargetPower(uint16_t distToGoal);
    MotorCtrl _motorCtrl;
    BlockSensors _blockSensors;
    TrainListener* _pListener;
    uint16_t _distToGoal;
    unsigned long _lastPowerIncrease;
    //Stopping ctrl
    bool _isStopping;
    unsigned long _stopInitiatedTime;
};


#endif