#ifndef _IPC_H
#define _IPC_H

#define COLLECTED_API_COUNT 42
#define COLLECTED_API_TIME_DELAY 200
#define COLLECTED_API_TIME_RANGE_STEPS 5
#define COLLECTED_API_TIME_RANGE (COLLECTED_API_TIME_RANGE_STEPS*COLLECTED_API_TIME_DELAY)

typedef struct _APIDATA_SINGLE {
    uint16_t api_count[COLLECTED_API_COUNT];
    uint32_t offset;
} APIDATA_SINGLE;

typedef struct _APIDATA {
    uint16_t api_count[COLLECTED_API_TIME_RANGE_STEPS][COLLECTED_API_COUNT];
    uint32_t offset;
} APIDATA;

void setupComms();
void sendData();
DWORD WINAPI sendRoutine(LPVOID lpParam);
void closeComms();

#endif