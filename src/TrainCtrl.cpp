/*
  Author: Tobias Grundtvig
*/

#include <Arduino.h>
#include "TrainCtrl.h"

TrainCtrl::TrainCtrl
(
      uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB, unsigned long msPrPowerChange,
      uint8_t frontSensorPin, uint8_t backSensorPin, uint8_t leftSensorPin, uint8_t rightSensorPin,
      TrainListener* pTrainListener
) : _motorCtrl(pwmPin, dirPinA, dirPinB, msPrPowerChange)
{
  _blockSensors.begin(frontSensorPin, backSensorPin, leftSensorPin, rightSensorPin, this);
  _pListener = pTrainListener;
  _distToGoal = 0;
  _lastPowerIncrease = 0;
  _isStopping = false;
  _stopInitiatedTime = 0;
}

void TrainCtrl::update(unsigned long curTime)
{
  _blockSensors.update(curTime);
  _motorCtrl.update(curTime);
  //Slowly increase power if no block seen lately...
  if(_distToGoal > 0 && _lastPowerIncrease + 500 < curTime)
  {
    Serial.println("Increasing power!");
    _motorCtrl.setTargetPower(_motorCtrl.getTargetPower() + 1);
    _lastPowerIncrease = curTime;
  }
  //If we are stopping, give some time for the train to halt completely
  if(_isStopping && _stopInitiatedTime + 3000 >= curTime)
  {
    _isStopping = false;
    _pListener->onTrainStopped();
  }
}

void TrainCtrl::move(uint16_t dist)
{
  _distToGoal += dist;
  _motorCtrl.setTargetPower(getTargetPower(_distToGoal));
}

void TrainCtrl::setDirection(bool forward)
{
  if(_distToGoal == 0 && !_isStopping)
  {
    if(forward)
    {
      _motorCtrl.setDirection(0);
    }
    else
    {
      _motorCtrl.setDirection(1);
    }
  }
}

void TrainCtrl::onBlock(uint8_t blockValue, unsigned long curTime)
{
  _lastPowerIncrease = curTime;
  --_distToGoal;
  _motorCtrl.setTargetPower(getTargetPower(_distToGoal));
  _pListener->onReadBlock(blockValue, _distToGoal);
  if(_distToGoal == 0)
  {
    _motorCtrl.stop();
    _isStopping = true;
    _stopInitiatedTime = curTime;
  }
}

uint8_t TrainCtrl::getTargetPower(uint16_t distToGoal)
{
  //Experiment here...
  if(distToGoal > 10) return 255;
  if(distToGoal == 10) return 190;
  if(distToGoal == 9) return 180;
  if(distToGoal == 8) return 170;
  if(distToGoal == 7) return 160;
  if(distToGoal == 6) return 150;
  if(distToGoal == 5) return 140;
  if(distToGoal == 4) return 130;
  if(distToGoal == 3) return 120;
  if(distToGoal == 2) return 110;
  if(distToGoal == 1) return 100;
  return 0;
}