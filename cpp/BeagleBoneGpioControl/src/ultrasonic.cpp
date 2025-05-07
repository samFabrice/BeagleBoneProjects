#include "ultrasonic.h"

UltrasonicSensor::UltrasonicSensor(int echoPin, int triggerPin, 
                                    gpio_control::GPIO& echoGpio, 
                                    gpio_control::GPIO& triggerGpio):
                                    m_echoPin(echoPin),
                                    m_triggerPin(triggerPin),
                                    m_echoGpio(echoGpio),
                                    m_triggerGpio(triggerGpio)
{

}

void UltrasonicSensor::sendTriggerSignal()
{
    m_triggerGpio.writeValue(1);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    m_triggerGpio.writeValue(0);

}
double UltrasonicSensor::getDistance() {
    constexpr int TIMEOUT_MS = 100;

    this->sendTriggerSignal();
    std::this_thread::sleep_for(std::chrono::microseconds(TIMEOUT_MS)); // petit délai

    auto echo_start_time = std::chrono::high_resolution_clock::now();

    // Attente du front montant (echo HIGH)
    while (m_echoGpio.readValue() == 0) {
        if (std::chrono::high_resolution_clock::now() - echo_start_time > std::chrono::milliseconds(TIMEOUT_MS)) {
            std::cerr << "Timeout waiting for echo to go HIGH" << std::endl;
            return -1.0; // timeout
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    // Attente du front descendant (echo LOW)
    while (m_echoGpio.readValue() == 1) {
        if (std::chrono::high_resolution_clock::now() - start_time > std::chrono::milliseconds(TIMEOUT_MS)) {
            std::cerr << "Timeout waiting for echo to go LOW" << std::endl;
            return -1.0; // timeout
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    double distance = (static_cast<double>(duration) * SPEED_OF_SOUND) / 20000.0;
    return distance;
}
