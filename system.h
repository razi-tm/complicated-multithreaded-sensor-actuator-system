#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>

// Sensor Base Class
class Sensor {
protected:
    std::string name;
    float value;
    float threshold;

public:
    Sensor(const std::string& name, float threshold)
        : name(name), value(0.0), threshold(threshold) {}

    virtual void read() = 0;
    virtual ~Sensor() {}

    float getValue() const { return value; }
    float getThreshold() const { return threshold; }
    void setThreshold(float newThreshold) { threshold = newThreshold; }
    std::string getName() const { return name; }
};

// Temperature Sensor
class TemperatureSensor : public Sensor {
public:
    TemperatureSensor(float threshold) : Sensor("Temperature", threshold) {}

    void read() override {
        value = 20.0 + (rand() % 100) / 10.0;  // Simulated temperature
    }
};

// Humidity Sensor
class HumiditySensor : public Sensor {
public:
    HumiditySensor(float threshold) : Sensor("Humidity", threshold) {}

    void read() override {
        value = 30.0 + (rand() % 100) / 10.0;  // Simulated humidity
    }
};

// Actuator Base Class
class Actuator {
protected:
    std::string name;

public:
    Actuator(const std::string& name) : name(name) {}
    virtual void control(bool state) = 0;
    virtual ~Actuator() {}
};

// LED Controller
class LEDController : public Actuator {
public:
    LEDController() : Actuator("LED") {}

    void control(bool state) override {
        std::cout << (state ? "LED is ON" : "LED is OFF") << std::endl;
    }
};

// Mutex and Queues for Thread Safety
extern std::mutex dataMutex;
extern std::mutex commandMutex;
extern std::queue<std::pair<std::string, float>> commandQueue;

// Global Sensor and Actuator Lists
extern std::vector<Sensor*> sensors;
extern std::unique_ptr<Actuator> led;

// Function Prototypes
void sensorTask();
void loggingTask();
void actuatorTask();
void commandInterfaceTask();
void inputCaptureTask();

#endif
