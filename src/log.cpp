#include <Arduino.h>
#include "main.h"
#include "log.h"

// Define the log message queue length
#define LOG_QUEUE_LENGTH     10

// Define the log message maximum length
#define LOG_MAX_LENGTH       64

TaskHandle_t taskLogHdlr;
static QueueHandle_t logQueue;


// **************************************************************************
void LogString(const char *str)
{
    xQueueSend(logQueue, str, 0);
}


// **************************************************************************
void TaskLog(void *pvParameters)
{
    (void)pvParameters;

    char logMessage[LOG_MAX_LENGTH];
    logQueue = xQueueCreate(LOG_QUEUE_LENGTH, LOG_MAX_LENGTH);

    Log("TaskLog started\n");

    while (1)
    {
        if (xQueueReceive(logQueue, logMessage, portMAX_DELAY) == pdTRUE)
        {
            // Print the log message to the Serial monitor
            Serial.println(logMessage);
        }
    }
    vTaskDelay(100);
}

// **************************************************************************
void Nothing(const char* format, ...)
{

}