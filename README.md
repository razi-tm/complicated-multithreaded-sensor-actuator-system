# Complicated multithreaded sensor-actuator system

This is a multithreaded sensor monitoring and actuator control system implemented in C++. It simulates sensors and an LED actuator, allowing users to monitor sensor values and adjust thresholds dynamically through a command interface.

---

## Features
- **Sensor Monitoring**: Periodically reads data from temperature and humidity sensors.
- **Threshold-based Actuation**: Controls an LED actuator based on whether sensor values exceed their thresholds.
- **Logging**: Logs sensor data periodically to a file.
- **Dynamic Threshold Adjustment**: Users can update sensor thresholds in real-time through a command-line interface.
- **Multithreading**: Utilizes multiple threads for concurrent tasks, including sensor monitoring, logging, actuator control, and command input handling.

---

## System Components

### 1. **Sensors**
- **TemperatureSensor**
- **HumiditySensor**

Each sensor has:
- A **name** (e.g., `Temperature`, `Humidity`)
- A **current value** (simulated)
- A **threshold**

### 2. **Actuator**
- **LEDController**: Turns the LED ON or OFF based on whether any sensor value exceeds its threshold.

### 3. **Tasks**
- **Sensor Task**: Reads sensor values periodically.
- **Logging Task**: Writes sensor values to a log file.
- **Actuator Task**: Checks sensor thresholds and controls the LED state.
- **Command Interface Task**: Handles user input for updating thresholds.
- **Input Capture Task**: Captures and queues user commands.

---

## How It Works
1. **Starting the System**:
   - The system initializes sensors and starts multiple threads.
2. **Sensor Value Updates**:
   - Sensors periodically update their values (simulated).
3. **Threshold Checking**:
   - The LED state is updated if any sensor value exceeds its threshold.
4. **Dynamic Input**:
   - Users can update thresholds using commands like:
     ```
     <SensorName> <NewThreshold>
     ```
     Example:
     ```
     Temperature 30
     ```
5. **Logging**:
   - Sensor data is periodically logged to a file (`sensor_log.txt`).

---

## Setup and Usage

### 1. **Requirements**
- g++ (C++14 or later)
- POSIX-compliant system (for pthreads)

### 2. **Build Instructions**
Run the following command to compile the program:
```bash
g++ -std=c++14 -pthread main.cpp system.cpp -o sensor_system
```

### 3. **Run the Program**
```bash
./sensor_system
```

### 4. **User Commands**
When prompted, you can enter commands to update sensor thresholds:
```bash
Enter sensor name and new threshold (e.g., Temperature 25):
```
Example inputs:
- `Temperature 30`
- `Humidity 40`

---

## File Structure
```
.
├── main.cpp         # Program entry point
├── system.cpp       # Implementation of system tasks and functionality
├── system.h         # Declarations of sensors, actuators, and tasks
├── sensor_log.txt   # Log file for sensor data (created at runtime)
└── README.md        # Documentation for the system
```

---

## Key Components in Code

### Threshold Logic for LED
The **LED** is controlled in the `actuatorTask`:
```cpp
bool state = sensor->getValue() > sensor->getThreshold();
led->control(state);
```
If any sensor's value exceeds its threshold, the LED turns ON. Otherwise, it remains OFF.

---

## Example Output
### Startup
```
Starting sensor system...
Threads started.
You can now enter commands in the format: <SensorName> <NewThreshold>
Enter sensor name and new threshold (e.g., Temperature 25):
```

### Commands
```
Temperature 30
Command queued: Temperature with threshold 30
Threshold for Temperature updated to 30
```

### Sensor Updates
```
Temperature Value: 28.3
Humidity Value: 38.6
LED is ON
Temperature Value: 24.5
Humidity Value: 35.1
LED is OFF
```

---

## Improvements and Customization
- Add more sensors and actuators.
- Implement additional control logic (e.g., hysteresis for actuators).
- Improve logging with timestamps.

---

## License
This project is open-source and free to use for educational purposes.


