#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Pin configuration
#define POT_PIN     34
#define GREEN_LED   25
#define RED_LED     26
#define BUTTON_PIN  27
#define BUZZER_PIN  14

// Sensor data structure
typedef struct {
  int sensorValue;
} SensorData;

// FreeRTOS queue
QueueHandle_t sensorQueue;

SemaphoreHandle_t serialMutex;

// Task handles
TaskHandle_t sensorHandle = NULL;
TaskHandle_t controlHandle = NULL;
TaskHandle_t buttonHandle = NULL;
TaskHandle_t monitorHandle = NULL;

// Shared counters
volatile unsigned long buttonCount = 0;
volatile int latestSensorValue = 0;

// Task state helper
const char* getTaskState(TaskHandle_t handle) {
  if (handle == NULL) return "Not created";

  switch (eTaskGetState(handle)) {
    case eRunning:   return "Running";
    case eReady:     return "Ready";
    case eBlocked:   return "Blocked";
    case eSuspended: return "Suspended";
    case eDeleted:   return "Deleted";
    default:         return "Unknown";
  }
}

// TASK 1: Sensor Task
void SensorTask(void *parameter) {
  SensorData data;

  while (1) {
    data.sensorValue = analogRead(POT_PIN);
    latestSensorValue = data.sensorValue;

    xQueueSend(sensorQueue, &data, 0);

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// TASK 2: Control Task
void ControlTask(void *parameter) {
  SensorData receivedData;

  while (1) {
    if (xQueueReceive(sensorQueue, &receivedData,
                      portMAX_DELAY) == pdTRUE) {

      if (receivedData.sensorValue > 2500) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BUZZER_PIN, HIGH);
      } else {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
      }
    }
  }
}

// TASK 3: Button Task
void ButtonTask(void *parameter) {

  int lastButtonState = HIGH;

  while (1) {

    int currentButtonState = digitalRead(BUTTON_PIN);

    // Detect button press
    if (lastButtonState == HIGH && currentButtonState == LOW) {

      buttonCount++;

      Serial.print("Button Pressed! Count: ");
      Serial.println(buttonCount);
    }

    lastButtonState = currentButtonState;

    // Button checking interval
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}  
// TASK 4: Monitoring Task
void MonitorTask(void *parameter) {
  while (1) {

    if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {

  // Your existing Serial.println() statements here

  
    Serial.println("\n===== FREERTOS TASK MONITOR =====");

    Serial.print("Sensor Value: ");
    Serial.println(latestSensorValue);

    Serial.print("Button Press Count: ");
    Serial.println(buttonCount);

    Serial.print("Total Active Tasks: ");
    Serial.println(uxTaskGetNumberOfTasks());

    Serial.print("Sensor Task: ");
    Serial.println(getTaskState(sensorHandle));

    Serial.print("Control Task: ");
    Serial.println(getTaskState(controlHandle));

    Serial.print("Button Task: ");
    Serial.println(getTaskState(buttonHandle));

    Serial.print("Monitor Task: ");
    Serial.println(getTaskState(monitorHandle));

    Serial.println("\n--- TASK PRIORITIES ---");

    Serial.print("Sensor Priority: ");
    Serial.println(uxTaskPriorityGet(sensorHandle));

    Serial.print("Control Priority: ");
    Serial.println(uxTaskPriorityGet(controlHandle));

    Serial.print("Button Priority: ");
    Serial.println(uxTaskPriorityGet(buttonHandle));

    Serial.print("Monitor Priority: ");
    Serial.println(uxTaskPriorityGet(monitorHandle));

    Serial.println("\n--- TASK STACK MONITORING ---");

    Serial.print("Sensor Task Free Stack: ");
    Serial.println(uxTaskGetStackHighWaterMark(sensorHandle));

    Serial.print("Control Task Free Stack: ");
    Serial.println(uxTaskGetStackHighWaterMark(controlHandle));

    Serial.print("Button Task Free Stack: ");
    Serial.println(uxTaskGetStackHighWaterMark(buttonHandle));

    Serial.print("Monitor Task Free Stack: ");
    Serial.println(uxTaskGetStackHighWaterMark(monitorHandle));

    Serial.println("================================");

    vTaskDelay(pdMS_TO_TICKS(2000));
    xSemaphoreGive(serialMutex);
}
  }
}

void setup() {
  Serial.begin(115200);

  serialMutex = xSemaphoreCreateMutex();

  pinMode(POT_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  sensorQueue = xQueueCreate(5, sizeof(SensorData));

  if (sensorQueue == NULL) {
    Serial.println("Queue creation failed!");
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  xTaskCreate(SensorTask, "SensorTask", 2048,
              NULL, 2, &sensorHandle);

  xTaskCreate(ControlTask, "ControlTask", 2048,
              NULL, 3, &controlHandle);

  xTaskCreate(ButtonTask, "ButtonTask", 2048,
              NULL, 2, &buttonHandle);

  xTaskCreate(MonitorTask, "MonitorTask", 3072,
              NULL, 1, &monitorHandle);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}