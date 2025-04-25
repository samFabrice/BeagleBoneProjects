#include "gpio.h"
#include <csignal>
#include <unistd.h>
#include <atomic>


std::atomic<bool> stop_flag(false);

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content;
    std::getline(file, content);
    file.close();
    return content;
}

void signal_handler(int signum)
{
    if(signum == SIGINT)
    {
        std::cout << "\n Signal Ctrl+C est recu , Arret du clignotement ... \n"<<std::endl;
        stop_flag.store(true);
    }
}

int main()
{
    signal(SIGINT, signal_handler);

    gpio_control::GPIO ledPin(60);
    
    if(!ledPin.isExported()){
        if(!ledPin.exportGpio()){
            std::cerr << "[Err:] Erreur lors de l'exportation du GPIO" << ledPin.getName();
            return 1;
        }
    }

    if(!ledPin.setDirection("out")){
        std::cerr << "Erreur lors de la configuration de la direction du GPIO" << ledPin.getName();
        ledPin.unexportGpio();
        return 1;
    }
    
    std::cout << "Direction configurée : " << readFile("/sys/class/gpio/gpio60/direction") << std::endl;

    std::cout << "Clignotement de la LED (Ctrl+C pour arrêter)..." << std::endl;
    while(!stop_flag.load())
    {
        if (!ledPin.writeValue(1)) {
            std::cerr << "Erreur lors de l'écriture de la valeur HIGH." << std::endl;
        }
        std::cout << "after writing 1 :" << ledPin.readValue() << std::endl;
        usleep(500000);

        if (!ledPin.writeValue(0)) {
            std::cerr << "Erreur lors de l'écriture de la valeur LOW." << std::endl;
        }
        std::cout << "after writing 0 :" << ledPin.readValue() << std::endl;

        usleep(500000); 
    }
    // Nettoyage final : désexporter le GPIO
    if (!ledPin.unexportGpio()) {
        std::cerr << "Erreur lors de la désactivation du GPIO " << ledPin.getPin() << std::endl;
    }
    ledPin.writeValue(0);
    std::cout << "Arrêt terminé. GPIO désactivé." << std::endl;
    return 0;
}