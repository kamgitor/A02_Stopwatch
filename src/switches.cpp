
#include <Arduino.h>
#include "switches.h"
#include "main.h"
#include "log.h"

TaskHandle_t taskSwitchHdlr;

static bool button_left = false;
static bool button_right = false;
static bool button_external = false;

bool GetSwitchState(uint8_t sw)
{
    uint8_t state = false;
    switch (sw)
    {
    case LEFT_KEY:
        state = button_left;
        button_left = false;
    case RIGHT_KEY:
        state = button_right;
        button_right = false;
        break;
    case EXTERNAL_KEY:
        state = button_external;
        button_external = false;
        break;
    }

    return state;
}

void TaskSwitches(void *pvParameters)
{
    (void)pvParameters;

    pinMode(LEFT_KEY, INPUT_PULLUP);
    pinMode(RIGHT_KEY, INPUT_PULLUP);
    pinMode(EXTERNAL_KEY, INPUT_PULLUP);

    while (1)
    {
    static bool button_left_prev = false;
    static bool button_right_prev = false;
    static bool button_external_prev = false;

        if (digitalRead(LEFT_KEY) == 0)
        {
            if (button_left_prev == false)
                button_left = true;

            button_left_prev = true;
        }
        else
        {
            button_left_prev = false;
        }

        if (digitalRead(RIGHT_KEY) == 0)
        {
            if (button_right_prev == false)
                button_right = true;

            button_right_prev = true;
        }
        else
        {
            button_right_prev = false;
        }

        if (digitalRead(EXTERNAL_KEY) == 0)
        {
            if (button_external_prev == false)
                button_external = true;

            button_external_prev = true;
        }
        else
        {
            button_external_prev = false;
        }

        vTaskDelay(100);
    }

}