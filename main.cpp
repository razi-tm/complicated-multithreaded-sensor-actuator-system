#include "system.h"
#include <thread>
#include <iostream>

int main() {
    std::cout << "Starting sensor system..." << std::endl;

    // Initialize sensors
    sensors.push_back(new TemperatureSensor(25.0f));
    sensors.push_back(new HumiditySensor(30.0f));

    // Debug: Print available sensors
    std::cout << "Available sensors:" << std::endl;
    for (Sensor* sensor : sensors) {
        std::cout << " - " << sensor->getName() << std::endl;
    }

    // Start threads
    std::thread sensorThread(sensorTask);
    std::thread actuatorThread(actuatorTask);
    std::thread loggerThread(loggingTask);
    std::thread commandThread(commandInterfaceTask);
    std::thread inputThread(inputCaptureTask);

    std::cout << "Threads started." << std::endl;
    std::cout << "You can now enter commands in the format: <SensorName> <NewThreshold>" << std::endl;

    // Join threads (blocks until threads finish)
    sensorThread.join();
    actuatorThread.join();
    loggerThread.join();
    commandThread.join();
    inputThread.join();

    // Clean up
    for (Sensor* sensor : sensors) {
        delete sensor;
    }
    std::cout << "Sensor system terminated." << std::endl;

    return 0;
}
