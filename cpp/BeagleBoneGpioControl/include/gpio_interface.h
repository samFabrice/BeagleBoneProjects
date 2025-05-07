#ifndef _GPIO_INTERFACE_H
#define _GPIO_INTERFACE_H
#include <string>

namespace gpio_control{
    class GPIOInterface{
        public : 
            virtual ~GPIOInterface()  = default ;
            virtual int readValue() const = 0;
            virtual bool writeValue(int) = 0;
            virtual bool setDirection(const std::string& ) = 0;

    };
}

#endif // !_GPIO_INTERFACE_H
