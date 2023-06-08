#ifndef _TFT_H_
#define _TFT_H_

extern TaskHandle_t taskTftHdlr;

enum tft_do
{
    READY_TO_START,
    COUNTING,
    COUNTING_STOPED,
};

void TaskTFT(void *pvParameters);
tft_do TftGetState(void);
void TftDo(tft_do what);
void DispNumber(int val);

#endif