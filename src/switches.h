#ifndef _SWITCHES_H_
#define _SWITCHES_H_

#define LEFT_KEY        0
#define RIGHT_KEY       35
#define EXTERNAL_KEY    27

extern TaskHandle_t taskSwitchHdlr;

bool GetSwitchState(uint8_t sw);
void TaskSwitches(void *pvParameters);


#endif
