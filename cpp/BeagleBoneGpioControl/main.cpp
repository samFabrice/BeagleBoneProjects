#include "gpio.h"
#include "ultrasonic.h"
#include <csignal>
#include <unistd.h>
#include <atomic>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>

std::atomic<bool> stop_flag(false);

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content;
    if (file.is_open()) {
        std::getline(file, content);
        file.close();
        return content;
    } else {
        std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;
        return "";
    }
}

bool writeFile(const std::string& filename, const std::string& value) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << value;
        file.close();
        return true;
    } else {
        std::cerr << "Impossible d'ouvrir le fichier " << filename << " (errno: " << errno << ")" << std::endl;
        return false;
    }
}

void signal_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "\n Signal Ctrl+C reçu, arrêt du programme...\n" << std::endl;
        stop_flag.store(true);
    }
}

int main() {
    signal(SIGINT, signal_handler);
    int triggerPin = 46;
    int echoPin = 47;
    gpio_control::GPIO triggerGpio(triggerPin);
    gpio_control::GPIO echoGpio(echoPin);

    triggerGpio.exportGpio();
    echoGpio.exportGpio();
    triggerGpio.setDirection("out");
    echoGpio.setDirection("in");

    UltrasonicSensor sensor(echoPin, triggerPin, echoGpio, triggerGpio);

    int pwm_channel = 0;
    std::string ledPath = "/sys/class/pwm/pwmchip1/pwm" + std::to_string(pwm_channel);

    if (!writeFile("/sys/class/pwm/pwmchip1/export", std::to_string(pwm_channel))) return 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (!writeFile(ledPath + "/period", "20000000")) return 1;
    if (!writeFile(ledPath + "/duty_cycle", "0")) return 1;
    if (!writeFile(ledPath + "/enable", "1")) return 1;

    double filtered_distance = 100.0; // initial value

    while (!stop_flag.load()) {
        double distance = sensor.getDistance();
        if (distance < 0) {
            std::cerr << "Erreur de mesure, saut du cycle..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // Lissage exponentiel
        double alpha = 0.7;
        filtered_distance = alpha * distance + (1 - alpha) * filtered_distance;

        // Normalisation et intensité quadratique
        double normalized = std::clamp((200.0 - filtered_distance) / 190.0, 0.0, 1.0);
        int intensity = static_cast<int>(normalized * normalized * 100);

        unsigned long dutyCycle = (static_cast<unsigned long>(intensity) * 20000000UL) / 100UL;
        if (!writeFile(ledPath + "/duty_cycle", std::to_string(dutyCycle))) {
            std::cerr << "Erreur écriture duty_cycle." << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    writeFile(ledPath + "/enable", "0");
    writeFile("/sys/class/pwm/pwmchip1/unexport", std::to_string(pwm_channel));
    return 0;
}
