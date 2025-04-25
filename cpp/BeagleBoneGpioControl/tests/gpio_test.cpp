#include "gtest/gtest.h"
#include "gpio.h"

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::string content;
    std::getline(file, content);
    file.close();
    return content;
}

namespace{
    
    TEST(GpioTest, GetGpioNameReturnsSomeThing)
    {
        gpio_control::GPIO gpio(42);
        std::string name = gpio.getName();
        ASSERT_FALSE(name.empty());
        ASSERT_EQ(gpio.getPin(), 42);
        ASSERT_EQ(name,"gpio_42");

    }

    TEST(GpioTest, ExportGpioShouldReturnTrueOnSuccess)
    {
        int pin = 3;
        gpio_control::GPIO gpio(pin);
        if(!gpio.isExported()){
            GTEST_SKIP() << "GPIO " << pin << " n'est pas exporté. Test ignoré.";
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, UnexportGpioShouldReturnTrueOnSuccess)
    {
        int pin = 35;
        gpio_control::GPIO gpio(pin);
        if(!gpio.isExported()){
            GTEST_SKIP() << "GPIO " << pin << " n'est pas exporté. Test ignoré.";
            return;
        }
        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, setDirectionReturnTrueOnSuccessAndWriteOutToFile)
    {
        int pin = 3;
        gpio_control::GPIO gpio(pin);
        if(!gpio.isExported()){
            GTEST_SKIP() << "GPIO " << pin << " n'est pas exporté. Test ignoré.";
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.setDirection("out"));
        
        ASSERT_EQ(readFile("/sys/class/gpio/gpio" + std::to_string(pin) + "/direction"),"out");

        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, setDirectionReturnTrueOnSuccessAndWriteInToFile)
    {
        int pin = 116;
        gpio_control::GPIO gpio(pin);
        if(!gpio.isExported()){
            GTEST_SKIP() << "GPIO " << pin << " n'est pas exporté. Test ignoré.";
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.setDirection("in"));
        
        ASSERT_EQ(readFile("/sys/class/gpio/gpio" + std::to_string(pin) + "/direction"),"in");

        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, setDirectionReturnFalseOnInvalidDirectiont)
    {
        int pin = 79;
        gpio_control::GPIO gpio(pin);
        if(!gpio.isExported()){
            GTEST_SKIP() << "GPIO " << pin << " n'est pas exporté. Test ignoré.";
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_FALSE(gpio.setDirection("titi_atta"));
        

        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, ReadValueReturnCorrectValue)
    {
        int pin = 74;
        gpio_control::GPIO gpio(pin);
        if(!gpio.isExported()){
            GTEST_SKIP() << "GPIO " << pin << " n'est pas exporté. Test ignoré.";
            return;
        }

        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.setDirection("in"));
 
        auto value = gpio.readValue();
        ASSERT_TRUE(value == 0 || value == 1 || value == -1);

        ASSERT_TRUE(gpio.unexportGpio());
    }

} // namespace