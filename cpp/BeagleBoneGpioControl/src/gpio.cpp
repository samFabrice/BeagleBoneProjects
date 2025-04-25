#include "gpio.h"

namespace gpio_control {
    GPIO::GPIO(int pin) : gpioNumber(pin)
    {

    }
    GPIO::~GPIO()
    {
        unexportGpio();
    }

    int GPIO::getPin() const
    {
        return gpioNumber;
    }

    std::string GPIO::getName() const
    {
        return "gpio_" + std::to_string(gpioNumber);
    }

    bool GPIO::exportGpio()
    {
        std::ofstream exportFile(GPIO_PATH + "/export");
        if(exportFile.is_open())
        {
            exportFile << gpioNumber << std::endl;
            exportFile.close();
            return true;
        }
        else
        {
            std::cerr << "Error while opening /sys/class/gpio/export for GPIO export" << gpioNumber;
            return false;
        }
    }

    bool GPIO::unexportGpio()
    {
        std::ofstream unexportFile(GPIO_PATH + "/unexport");
        if(unexportFile.is_open())
        {
            unexportFile << gpioNumber << std::endl;
            unexportFile.close();
            return true;
        }
        else {
            std::cerr << "Erreur lors de l'ouverture de /sys/class/gpio/unexport pour la désactivation du GPIO " << gpioNumber << std::endl;
            return false;
        }
    }

    int GPIO::readValue( ) const {
        std::ifstream  valueFile(GPIO_PATH + "/gpio"+ std::to_string(gpioNumber) + "/value");
        if(valueFile.is_open())
        {
            int value;
            valueFile >> value;
            return value;
        }
        else
        {
            std::cerr << "[Err :] Erreur ouverture du fichier de lecture GPIO" << gpioNumber <<std::endl;
            return -1;
        }

    }

    bool GPIO::isExported() const {
        std::string path = GPIO_PATH + "/gpio" + std::to_string(gpioNumber);
        struct stat st;
        return (stat(path.c_str(), &st) == 0);
    }

    bool GPIO::setDirection(const std::string& direction){
        std::ofstream directionFile(GPIO_PATH + "/gpio" + std::to_string(gpioNumber) + "/direction");
        if(directionFile.is_open())
        {
            directionFile << direction << std::endl;
            directionFile.close();
            return true;
        }
        else {
            std::cerr << "[Err :] Erreur ouverture du fichier de direction GPIO " << gpioNumber << std::endl;
            return false;
        }
    }

    bool GPIO::writeValue(int value)
    {
        std::ofstream file(GPIO_PATH + "/gpio" + std::to_string(gpioNumber) + "/value");
        if(file.is_open())
        {
            file << value <<std::endl;
            file.close();
            return true;
        }
        else
            std::cerr<<"[Err]:Erreur lors de l'ouverture du fichier de valeur GPIO "<< getName();
            return false;
    }
    
} // namespace gpio_control
