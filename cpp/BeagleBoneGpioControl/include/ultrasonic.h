#ifndef _UltrasonicSensor_H
#define _UltrasonicSensor_H
#include "gpio.h"
#include <chrono>
#include <thread>

const double SPEED_OF_SOUND = 343.0;

class UltrasonicSensor{
public:
    UltrasonicSensor(int , int , gpio_control::GPIO& , gpio_control::GPIO& );
    void sendTriggerSignal();
    double getDistance();
private:
    int m_echoPin;
    int m_triggerPin;
    gpio_control::GPIO& m_echoGpio;
    gpio_control::GPIO& m_triggerGpio;

};

#endif // !_UltrasonicSensor_H
