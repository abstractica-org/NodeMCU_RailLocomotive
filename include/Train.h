#ifndef Train_h
#define Train_h

#include <Arduino.h>
#include <TrainCtrl.h>
#include <RemoteDevice.h>
#include <LEDBlinker.h>

#define IDENTIFY_BLINK_DURATION 15000

//Server to train commands
#define CMD_IDENTIFY 1000
#define CMD_MOVE 1001
#define CMD_DIRECTION 1002

//Train to server commands
#define CMD_ON_BLOCK 2000
#define CMD_STOPPED 2001

class Train : public RemoteDevice, TrainListener
{
public:
    Train
    (
        uint8_t pwmPin, uint8_t dirPinA, uint8_t dirPinB, unsigned long msPrPowerChange,
        uint8_t frontSensorPin, uint8_t backSensorPin, uint8_t leftSensorPin, uint8_t rightSensorPin
    );
    void update(unsigned long curTime);
    void onReadBlock(uint8_t blockValue, uint16_t distToGoal);
    void onTrainStopped();
    uint16_t onPacketReceived(uint16_t command, uint16_t arg1, uint16_t arg2, uint8_t* pData, uint16_t size);
private:
    TrainCtrl _trainCtrl;
    LEDBlinker _led;
    unsigned long _ledStarted;
    unsigned long _curTime;
    bool _isStopped;
    bool _isStoppedSent;
};

#endif