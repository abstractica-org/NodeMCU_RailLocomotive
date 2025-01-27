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
    static void setDirection(bool forward);
private:
    //Sensor callbacks
    static void IRAM_ATTR _onFrontSensor();
    static void IRAM_ATTR _onBackSensor();
    static void IRAM_ATTR _onLeftSensor();
    static void IRAM_ATTR _onRightSensor();

    static uint8_t _frontSensorPin;
    static uint8_t _backSensorPin;
    static uint8_t _leftSensorPin;
    static uint8_t _rightSensorPin;

    static volatile bool _frontActivated;
    static volatile bool _backActivated;
    static volatile bool _leftActivated;
    static volatile bool _rightActivated;

    static BlockListener* _pListener;

    static bool _forward;

    //Keeping track of blocks
    static bool _onBlock;
    static unsigned long _enterBlockTime;
};
#endif