#include <Arduino.h>
#include <TFT_eSPI.h>

#include "main.h"
#include "tft.h"
#include "switches.h"
#include "log.h"


// tasks handlers
TaskHandle_t taskMainHdlr;



// **************************************************************************
void TaskMain(void *pvParameters)
{
    Log("TaskMain started\n");

    while(1)
    {
        bool key_state = GetSwitchState(EXTERNAL_KEY);
        tft_do state = TftGetState();

        switch (state)
        {
        case READY_TO_START:
            if (key_state)
            {
                Log("Goto counting\n");
                TftDo(COUNTING);
            }
            break;
        case COUNTING:
            if (key_state)
            {
                Log("Goto stoped\n");
                TftDo(COUNTING_STOPED);
            }
            break;
        case COUNTING_STOPED:
            if (key_state)
            {
                Log("Goto ready\n");
                TftDo(READY_TO_START);
            }
            break;
        }

        vTaskDelay(100);
    }
}


// **************************************************************************
// LCD: 135 x 240
void setup()
{
    // LogMutex = xSemaphoreCreateMutex();

    Serial.begin(9600);

    xTaskCreatePinnedToCore(TaskLog, "TaskLog", 1024, NULL, 3, &taskLogHdlr, 0);
    xTaskCreatePinnedToCore(TaskTFT, "TaskTFT", 1024, NULL, 3, &taskTftHdlr, 0);
    xTaskCreatePinnedToCore(TaskSwitches, "TaskSwitches", 1024, NULL, 3, &taskSwitchHdlr, 0);
    xTaskCreatePinnedToCore(TaskMain, "TaskMain", 1024, NULL, 3, &taskMainHdlr, 0);

    Log("Tasks created\n");

}   // setup


// **************************************************************************
void loop()
{


}   // loop

