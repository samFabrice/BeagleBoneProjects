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
    const int GPIO_TO_TEST = 46;
    TEST(GpioTest, GetGpioNameReturnsSomeThing)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        std::string name = gpio.getName();
        ASSERT_FALSE(name.empty());
        ASSERT_EQ(gpio.getPin(), GPIO_TO_TEST);
        ASSERT_EQ(name,"gpio_46");

    }

    TEST(GpioTest, ExportGpioShouldReturnTrueOnSuccess)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        if(!gpio.isExported()){
            ASSERT_TRUE(gpio.exportGpio());
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, UnexportGpioShouldReturnTrueOnSuccess)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        if(!gpio.isExported()){
            ASSERT_TRUE(gpio.exportGpio());
            return;
        }
        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, setDirectionReturnTrueOnSuccessAndWriteOutToFile)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        if(!gpio.isExported()){
            ASSERT_TRUE(gpio.exportGpio());
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.setDirection("out"));
        
        ASSERT_EQ(readFile("/sys/class/gpio/gpio" + std::to_string(GPIO_TO_TEST) + "/direction"),"out");

        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, setDirectionReturnTrueOnSuccessAndWriteInToFile)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        if(!gpio.isExported()){
            ASSERT_TRUE(gpio.exportGpio());
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.setDirection("in"));
        
        ASSERT_EQ(readFile("/sys/class/gpio/gpio" + std::to_string(GPIO_TO_TEST) + "/direction"),"in");

        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, setDirectionReturnFalseOnInvalidDirectiont)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        if(!gpio.isExported()){
            ASSERT_TRUE(gpio.exportGpio());
            return;
        }
        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_FALSE(gpio.setDirection("titi_atta"));
        

        ASSERT_TRUE(gpio.unexportGpio());
    }

    TEST(GpioTest, ReadValueReturnCorrectValue)
    {
        gpio_control::GPIO gpio(GPIO_TO_TEST);
        if(!gpio.isExported()){
            ASSERT_TRUE(gpio.exportGpio());
            return;
        }

        ASSERT_TRUE(gpio.exportGpio());
        ASSERT_TRUE(gpio.setDirection("in"));
 
        auto value = gpio.readValue();
        ASSERT_TRUE(value == 0 || value == 1 || value == -1);

        ASSERT_TRUE(gpio.unexportGpio());
    }

} // namespace