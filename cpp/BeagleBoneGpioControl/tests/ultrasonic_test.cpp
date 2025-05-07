#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ultrasonic.h"
#include "gpio.h"

using namespace testing;

class MockGPIO : public gpio_control::GPIO
{
    public:
        MockGPIO(int pin) : gpio_control::GPIO(pin) {};
        virtual ~MockGPIO() {};
        MOCK_METHOD(int, readValue, (), (const,override));
        MOCK_METHOD(bool, writeValue, (int), (override));
        MOCK_METHOD(bool, setDirection,(const std::string&), (override));
};

TEST(UltrasonicSensorTest, CanCreateInstance)
{
    int triggerPin = 45;
    int echoPin = 12;
    MockGPIO mockechogpio(echoPin);
    MockGPIO mocktriggergpio(triggerPin);
    UltrasonicSensor sensor(triggerPin, echoPin, mockechogpio, mocktriggergpio);

    ASSERT_NE(nullptr, &sensor);
}

TEST(UltrasonicSensorTest, SendTriggerSignalCorrectly)
{
    int triggerPin = 45;
    int echoPin = 12;
    MockGPIO mockechogpio(echoPin);
    MockGPIO mocktriggergpio(triggerPin);
    UltrasonicSensor sensor(triggerPin, echoPin, mockechogpio, mocktriggergpio);
    EXPECT_CALL(mocktriggergpio, writeValue(1));
    EXPECT_CALL(mocktriggergpio, writeValue(0));
    sensor.sendTriggerSignal();
}

TEST(UltrasonicSensorTest, GetDistance) {
    int triggerPin = 45;
    int echoPin = 12;
    MockGPIO mockEchoGpio(echoPin);
    MockGPIO mockTriggerGpio(triggerPin);
    UltrasonicSensor sensor(echoPin, triggerPin, mockEchoGpio, mockTriggerGpio);

    EXPECT_CALL(mockTriggerGpio, writeValue(1)).Times(1);
    EXPECT_CALL(mockTriggerGpio, writeValue(0)).Times(1);

    InSequence seq;
    EXPECT_CALL(mockEchoGpio, readValue()).WillOnce(Return(0)); // LOW initial
    EXPECT_CALL(mockEchoGpio, readValue()).WillOnce(Return(1)); // HIGH start

    // Simuler 10 lectures à HIGH
    for (int i = 0; i < 10; ++i) {
        EXPECT_CALL(mockEchoGpio, readValue()).WillOnce(Return(1));
    }

    EXPECT_CALL(mockEchoGpio, readValue()).WillOnce(Return(0)); // fin du HIGH

    double distance = sensor.getDistance();
    std::cout << "distance mesurée : " << distance << " cm" << std::endl;

    ASSERT_NEAR(distance, 17.15, 1.0);
}
