#ifndef GPIO_H
#define GPIO_H
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "gpio_interface.h"

namespace gpio_control{
    class GPIO : public GPIOInterface{
        public:
            explicit GPIO(int );
            ~GPIO();
            int getPin( ) const ;
            std::string getName( ) const ;
            bool exportGpio();
            bool unexportGpio();
            int readValue( ) const override;
            bool setDirection(const std::string& ) override;
            bool isExported() const;
            bool writeValue(int ) override ;
        private:
            int gpioNumber;
            const std::string GPIO_PATH = "/sys/class/gpio";
    };
} // namespace gpio_control

#endif