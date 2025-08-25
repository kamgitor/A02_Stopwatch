#include <Arduino.h>
#include <TFT_eSPI.h>
#include "tft.h"
#include "log.h"
#include "main.h"

static TFT_eSPI tft = TFT_eSPI();
TaskHandle_t taskTftHdlr;
uint8_t tft_mode = 0;

static uint8_t disp_mode = 0;
static bool clear_scr = false;


void DispNumber(int val)        // 0:00 - 99:99
{
    char str[8];
    int dec = val / 10;
    int frac = val % 10;

    if (disp_mode == 0)
    {
        sprintf(str, "%02d.%01d", dec, frac);
        tft.drawString(str, 10, 20, 8);
    }
    else
    {
        sprintf(str, "%03d.%01d", dec, frac);
        tft.drawString(str, 40, 35, 7);
    }
}

void TaskTFT(void *pvParameters)
{
    (void)pvParameters;

    Log("TaskTft started\n");

    tft.init();
    tft.setRotation(1);     // horizontal
    // tft.setRotation(0);        // vertical ?
    tft.fillScreen(TFT_BLACK);

    while (1)
    {
        static int val = 0;

        switch (tft_mode)
        {
        case 0:     // ready to start
            tft.fillScreen(TFT_YELLOW);
            vTaskSuspend(taskTftHdlr);
            break;
        case 1:     // prepare to counting
            val = 0;
            ++tft_mode;
            tft.fillScreen(TFT_BLACK);
            break;
        case 2:     // counting
            {
                TickType_t taskStartTime = xTaskGetTickCount();

                if (clear_scr)
                {
                    clear_scr = 0;
                    tft.fillScreen(TFT_BLACK);
                }

                DispNumber(val);
                if (disp_mode == 0)
                {
                    if (++val >= 999)
                        val = 0;
                }
                else
                {
                    if (++val >= 9999)
                        val = 0;
                }

                TickType_t currentTime = xTaskGetTickCount();
                TickType_t elapsedTime = currentTime - taskStartTime;
                TickType_t desiredDelay = pdMS_TO_TICKS(100);
                if (elapsedTime < desiredDelay)
                {
                    vTaskDelay(desiredDelay - elapsedTime);
                }
            }
            break;

        default:     // disp counted value
            vTaskSuspend(taskTftHdlr);
            break;
        }
    }
}


tft_do TftGetState(void)
{
    switch (tft_mode)
    {
    case 0:
        return READY_TO_START;
    case 1:
    case 2:
        return COUNTING;
    default:
        return COUNTING_STOPED;
    }
}


void TftDo(tft_do what)
{
    switch (what)
    {
    case READY_TO_START:
        tft_mode = 0;
        vTaskResume(taskTftHdlr);
        break;
    case COUNTING:
        tft_mode = 1;
        vTaskResume(taskTftHdlr);
        break;
    case COUNTING_STOPED:
        tft_mode = 3;
        break;
    }
}

void DispModeChange(void)
{
    clear_scr = true;

    if (disp_mode)
        disp_mode = 0;
    else
        disp_mode = 1;
}