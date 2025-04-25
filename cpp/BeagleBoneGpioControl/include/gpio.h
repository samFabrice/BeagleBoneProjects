#ifndef GPIO_H
#define GPIO_H
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace gpio_control{
    class GPIO{
        public:
            explicit GPIO(int );
            ~GPIO();
            int getPin( ) const ;
            std::string getName( ) const ;
            bool exportGpio();
            bool unexportGpio();
            int readValue( ) const;
            bool setDirection(const std::string& );
            bool isExported() const;
            bool writeValue(int ) ;
        private:
            int gpioNumber;
            const std::string GPIO_PATH = "/sys/class/gpio";
    };
} // namespace gpio_control

#endif