/*
  Author: Tobias Grundtvig
*/

#ifndef BlockSensors_h
#define BlockSensors_h


#include <Arduino.h>
#include "BlockListener.h"

class BlockSensors
{
public:
    static void begin
    (
        uint8_t frontSensorPin,
        uint8_t backSensorPin,
        uint8_t leftSensorPin,
        uint8_t rightSensorPin,
        BlockListener* pListener
    );
    static void update(unsigned long curTime);
    static void setDirection(uint8_t dir);
private:
    //Hallsensor interrupt callbacks
    static void IRAM_ATTR _onFrontSensor();
    static void IRAM_ATTR _onBackSensor();
    static void IRAM_ATTR _onLeftSensor();
    static void IRAM_ATTR _onRightSensor();

    static uint8_t _frontSensorPin;
    static uint8_t _backSensorPin;
    static uint8_t _leftSensorPin;
    static uint8_t _rightSensorPin;

    static BlockListener* _pListener;

    static volatile uint8_t _curDirection;

    //Keeping track of blocks
    static volatile bool _onBlock;
    static volatile bool _leftBlock;
    static volatile bool _rightBlock;
    static volatile uint8_t _blockCount;
    static uint8_t _lastBlockCount;
};


#endif