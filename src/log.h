#ifndef _LOG_H_
#define _LOG_H_

#define USE_LOGS            0

#if USE_LOGS
    #define Log(str)        LogString(str)
#else
    #define Log             Nothing
#endif

extern TaskHandle_t taskLogHdlr;

void TaskLog(void *pvParameters);
void LogString(const char *str);

#endif