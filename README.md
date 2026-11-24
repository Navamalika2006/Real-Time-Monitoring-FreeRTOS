# Real-Time-Monitoring-FreeRTOS
ESP32 based real time task monitoring system using FreeRTOS 

1. Project Overview

The Real-Time Task Monitoring System is an ESP32-based embedded system developed using FreeRTOS.

This project demonstrates real-time multitasking, inter-task communication, task priority management, synchronization, and task monitoring.

The system uses a potentiometer as an analog sensor input and controls LEDs and a buzzer based on the sensor value. A push button is used to count user interactions, while a monitoring task displays real-time task information through the Serial Monitor.

The project is simulated using the Wokwi online simulator.

2. Project Objectives

- To understand real-time operating system concepts.
- To implement multitasking using FreeRTOS.
- To create and manage multiple independent tasks.
- To understand inter-task communication using queues.
- To implement task priority management.
- To demonstrate synchronization using a mutex.
- To monitor task states and stack usage.

3. Hardware Components

Component| Quantity
ESP32 DevKit V1| 1
10K Potentiometer| 1
Push Button| 1
Green LED| 1
Red LED| 1
220Ω Resistors| 2
Buzzer| 1

4. Pin Configuration

Component| ESP32 GPIO
Potentiometer Signal| GPIO 34
Green LED| GPIO 25
Red LED| GPIO 26
Push Button| GPIO 27
Buzzer| GPIO 14

Connections:

- Potentiometer: VCC to 3.3V, GND to GND, Signal to GPIO 34.
- Push Button: GPIO 27 and GND (using INPUT_PULLUP).
- Green LED: GPIO 25 through a 220Ω resistor to GND.
- Red LED: GPIO 26 through a 220Ω resistor to GND.
- Buzzer: Positive to GPIO 14 and Negative to GND.

5. FreeRTOS Tasks Implementation

Four independent tasks are created in this project.

1. SensorTask

- Reads the analog value from the potentiometer.
- Uses GPIO 34 as the analog input.
- Sends sensor data to the queue.
- Executes periodically with a 200 ms delay.

2. ControlTask

- Receives sensor data from the queue.
- Compares the sensor value with a threshold of 2500.
- Controls the LEDs and buzzer based on the sensor value.

Control Logic:

- If sensor value is greater than 2500:
  
  - Red LED ON
  - Green LED OFF
  - Buzzer ON

- If sensor value is 2500 or below:
  
  - Green LED ON
  - Red LED OFF
  - Buzzer OFF

3. ButtonTask

- Reads the push button state.
- Detects button press events.
- Increments the button press counter.
- Uses a 20 ms delay for periodic checking.

4. MonitorTask

- Displays real-time monitoring information through the Serial Monitor.
- Displays sensor values and button press count.
- Monitors task states.
- Displays task priorities.
- Monitors the remaining stack space of each task.
- Displays the total number of FreeRTOS tasks.

6. FreeRTOS Concepts Used

Multitasking

Multiple independent tasks execute concurrently under FreeRTOS scheduling.

Task Priority

Different priorities are assigned to tasks to control their scheduling importance.

Task| Priority
SensorTask| 2
ControlTask| 3
ButtonTask| 2
MonitorTask| 1

Queue Communication

A FreeRTOS queue is used to transfer sensor data from SensorTask to ControlTask.

Mutex Synchronization

A mutex is used to protect the Serial Monitor output section in MonitorTask from concurrent access.

Task State Monitoring

The project monitors the following task states:

- Running
- Ready
- Blocked
- Suspended
- Deleted

Stack Monitoring

The function "uxTaskGetStackHighWaterMark()" is used to monitor the stack usage of the four tasks.

7. Working Principle

1. The ESP32 initializes the hardware components and creates a FreeRTOS queue.
2. Four independent tasks are created with different priorities.
3. SensorTask continuously reads the potentiometer value.
4. Sensor data is transferred through a queue to ControlTask.
5. ControlTask operates the LEDs and buzzer based on the sensor threshold.
6. ButtonTask detects button presses and updates the counter.
7. MonitorTask periodically displays task information through the Serial Monitor.
8. Mutex synchronization is used for the monitoring task's Serial output section.

8. Software and Tools

- Microcontroller: ESP32 DevKit V1
- Programming Language: Embedded C++ (Arduino framework)
- RTOS: FreeRTOS
- Simulation Platform: Wokwi
- Development Environment: Wokwi Online Simulator

9. Expected Output

The Serial Monitor displays:

- Analog sensor value
- Button press count
- Total active FreeRTOS tasks
- Individual task states
- Task priorities
- Free stack information for all four tasks

The LEDs and buzzer respond to the potentiometer input according to the programmed threshold.

10. Results

The project was successfully simulated using the Wokwi platform.

The implementation demonstrates:

- Creation of four FreeRTOS application tasks.
- Inter-task communication using a queue.
- Task priority assignment.
- Mutex-based synchronization for monitoring output.
- Real-time task state monitoring.
- Stack usage monitoring.
- Sensor-based LED and buzzer control.

11. Future Enhancements

- Add additional sensors for real-time monitoring.
- Implement semaphore-based task synchronization.
- Monitor task execution time and CPU utilization.
- Add an OLED display for real-time status visualization.
- Extend the system to an IoT-based monitoring platform.

12. Conclusion

This project provides practical experience in implementing real-time multitasking using FreeRTOS on the ESP32 microcontroller.

It demonstrates important RTOS concepts such as task creation, task scheduling, priority management, queue-based communication, mutex synchronization, and task monitoring.

The project helped develop a practical understanding of embedded systems programming and real-time operating system fundamentals.

13. Simulation

Platform: Wokwi

Simulation Link: [https://wokwi.com/projects/481487475524963329]

---

Developed as an Embedded Systems Learning Project.
