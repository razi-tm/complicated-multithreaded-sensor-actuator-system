#include "system.h"
#include <thread>
#include <queue>
#include <chrono>

std::mutex dataMutex;
std::mutex commandMutex;
std::queue<std::pair<std::string, float>> commandQueue;
std::vector<Sensor*> sensors;
std::unique_ptr<Actuator> led = std::make_unique<LEDController>();

// Sensor Task
void sensorTask() {
    while (true) {
        for (Sensor* sensor : sensors) {
            sensor->read();
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                std::cout << sensor->getName() << " Value: " << sensor->getValue() << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate periodic reading
    }
}

// Logging Task
void loggingTask() {
    std::ofstream logFile("sensor_log.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not open log file." << std::endl;
        return;
    }

    while (true) {
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            for (Sensor* sensor : sensors) {
                logFile << sensor->getName() << ": " << sensor->getValue() << std::endl;
            }
            logFile << "----" << std::endl;
        }
        logFile.flush();
        std::this_thread::sleep_for(std::chrono::seconds(2));  // Log every 2 seconds
    }
}

// Actuator Task
void actuatorTask() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            for (Sensor* sensor : sensors) {
                bool state = sensor->getValue() > sensor->getThreshold();
                led->control(state);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Check every second
    }
}

// Command Interface Task (non-blocking)
void commandInterfaceTask() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(commandMutex);
            while (!commandQueue.empty()) {
                auto command = commandQueue.front();
                commandQueue.pop();

                std::string name = command.first;
                float newThreshold = command.second;

                bool sensorFound = false;

                {
                    std::lock_guard<std::mutex> dataLock(dataMutex);
                    for (Sensor* sensor : sensors) {
                        if (sensor->getName() == name) {  // Match sensor name
                            sensor->setThreshold(newThreshold);
                            std::cout << "Threshold for " << name << " updated to " << newThreshold << std::endl;
                            sensorFound = true;
                            break;
                        }
                    }
                }

                if (!sensorFound) {
                    std::cerr << "Error: Sensor '" << name << "' not found." << std::endl;
                }
            }
        }

        // Short sleep to prevent busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



// Input Capture Task
void inputCaptureTask() {
    while (true) {
        std::cout << "Enter sensor name and new threshold (e.g., Temperature 25): ";
        std::cout.flush(); // Ensure immediate display of the prompt

        std::string name;
        float newThreshold;

        // Blocking call to wait for user input
        if (std::cin >> name >> newThreshold) {
            {
                std::lock_guard<std::mutex> lock(commandMutex);
                commandQueue.push({name, newThreshold});
            }

            std::cout << "Command queued: " << name << " with threshold " << newThreshold << std::endl;
            std::cout.flush(); // Ensure immediate output
        } else {
            // Handle invalid input (e.g., Ctrl+D or non-numeric values)
            std::cout << "Invalid input. Please try again." << std::endl;
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining input
        }

        // Add a short delay to prevent tight-loop execution in edge cases
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

