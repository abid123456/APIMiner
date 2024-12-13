#include <windows.h>
#include "ntapi.h"
#include "time.h"
#include "ipc.h"

#define PIPE_NAME L"\\Device\\NamedPipe\\ipc_pipe"
#define EVENT_NAME L"\\BaseNamedObjects\\ipc_event"
#define FILE_PIPE_MESSAGE_TYPE 0x00000001
#define FILE_PIPE_MESSAGE_MODE 0x00000001
#define FILE_PIPE_QUEUE_OPERATION 0x00000000
#define OBJ_CASE_INSENSITIVE 0x00000040L

// --Communications - API--
NTSTATUS(__stdcall* Real_NtOpenFile)(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG ShareAccess,
    ULONG OpenOptions);

NTSTATUS(__stdcall* Real_NtWriteFile)(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID Buffer,
    ULONG Length,
    PLARGE_INTEGER ByteOffset,
    PULONG Key);

NTSTATUS(__stdcall* Real_NtClose)(
    HANDLE Handle);

NTSTATUS(__stdcall* Real_NtOpenEvent)(
    PHANDLE EventHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes);

NTSTATUS(__stdcall* Real_NtSetEvent)(
    HANDLE EventHandle,
    PLONG PreviousState);

VOID(__stdcall* Real_RtlInitUnicodeString)(
    PUNICODE_STRING         DestinationString,
    __drv_aliasesMem PCWSTR SourceString);

NTSTATUS status = { 0 };
IO_STATUS_BLOCK ioStatusBlock = { 0 };

HANDLE hEvent = { 0 };
UNICODE_STRING eventName = { 0 };
OBJECT_ATTRIBUTES eventAttr = { 0 };

HANDLE hPipe = { 0 };
UNICODE_STRING pipeName = { 0 };
OBJECT_ATTRIBUTES pipeAttr = { 0 };

HANDLE hCommsThread;
DWORD dwCommsThread;

BOOLEAN setupCompleted;
BOOLEAN commsSending;
APIDATA_SINGLE api_data = { 0 };
CRITICAL_SECTION hLock;
LARGE_INTEGER start_time;

APIDATA_SINGLE apidata_tosend;

// --Functions--
static VOID fetchNTFunc(PVOID* ppvReal, const CHAR* psz, const CHAR* lib) {
    HMODULE hNtdll = GetModuleHandle("ntdll");
    *ppvReal = (PVOID)GetProcAddress(hNtdll, psz);
}

void setupComms() {
    InitializeCriticalSection(&hLock);
    fetchNTFunc((PVOID*)&Real_NtOpenFile, "NtOpenFile", "ntdll");
    fetchNTFunc((PVOID*)&Real_NtWriteFile, "NtWriteFile", "ntdll");
    fetchNTFunc((PVOID*)&Real_NtClose, "NtClose", "ntdll");
    fetchNTFunc((PVOID*)&Real_NtOpenEvent, "NtOpenEvent", "ntdll");
    fetchNTFunc((PVOID*)&Real_NtSetEvent, "NtSetEvent", "ntdll");
    fetchNTFunc((PVOID*)&Real_RtlInitUnicodeString, "RtlInitUnicodeString", "ntdll");

    // Init pipe
    Real_RtlInitUnicodeString(&pipeName, PIPE_NAME);
    InitializeObjectAttributes(
        &pipeAttr,
        &pipeName,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    );

    // Init event
    Real_RtlInitUnicodeString(&eventName, EVENT_NAME);
    InitializeObjectAttributes(
        &eventAttr,
        &eventName,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    );

    status = Real_NtOpenEvent(
        &hEvent,
        EVENT_ALL_ACCESS,
        &eventAttr
    );

    commsSending = TRUE;
    hCommsThread = CreateThread(
        NULL,
        0,
        sendRoutine,
        NULL,
        0,
        &dwCommsThread
    );
}

void sendData() {
    // Get pipe
    memset(&ioStatusBlock, 0, sizeof(IO_STATUS_BLOCK));
    status = Real_NtOpenFile(
        &hPipe,
        GENERIC_READ | GENERIC_WRITE,
        &pipeAttr,
        &ioStatusBlock,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        0
    );

    // Send data
    memset(&ioStatusBlock, 0, sizeof(IO_STATUS_BLOCK));
    // const char* send = "Hello!";
    status = Real_NtWriteFile(
        hPipe,
        NULL,
        NULL,
        NULL,
        &ioStatusBlock,
        (void*)&apidata_tosend,
        sizeof(apidata_tosend),
        //(void*)&api_data,
        //sizeof(api_data),
        NULL,
        NULL
    );

    // Trigger Event
    status = Real_NtSetEvent(
        hEvent,
        NULL
    );

    status = Real_NtClose(hPipe);
}

DWORD WINAPI sendRoutine(LPVOID lpParam) {
    LARGE_INTEGER frequency, call_time, pre_sleep_time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&call_time);

    // std::chrono::high_resolution_clock::time_point call_time = std::chrono::high_resolution_clock::now();
    // double relative_time = std::chrono::duration<double, std::milli>(call_time - start_time).count();

    while (!setupCompleted) {}

    Sleep(COLLECTED_API_TIME_RANGE);
    while (commsSending) {
        /**
        std::chrono::high_resolution_clock::time_point curr_time1 = std::chrono::high_resolution_clock::now();
        long long relative_time1 = std::chrono::duration_cast<std::chrono::microseconds>(curr_time1 - start_time).count();
        std::cout << "about to print at " << relative_time1 << std::endl;

        std::cout << "offset: " << static_cast<int>(api_data.offset) << std::endl;
        for (size_t frame = 0; frame < COLLECTED_API_TIME_RANGE_STEPS; frame++)
        {
            std::cout << "[";
            for (size_t i = 0; i < COLLECTED_API_COUNT; i++) {
                std::cout << apidata_tosend.api_count[frame][i] << ",";
            }
            std::cout << "]" << std::endl;
        }

        std::chrono::high_resolution_clock::time_point curr_time2 = std::chrono::high_resolution_clock::now();
        long long relative_time2 = std::chrono::duration_cast<std::chrono::microseconds>(curr_time2 - start_time).count();
        std::cout << "about to send at " << relative_time2 << std::endl;
        **/

        apidata_tosend.offset = api_data.offset;
        EnterCriticalSection(&hLock);
        memcpy(apidata_tosend.api_count, api_data.api_count, sizeof(apidata_tosend.api_count));
        memset(api_data.api_count, 0, sizeof(api_data.api_count));
        LeaveCriticalSection(&hLock);
        sendData();
        //api_data.offset = INCREMENT_WRAP(api_data.offset, COLLECTED_API_TIME_RANGE_STEPS);
        api_data.offset++;

        // std::chrono::high_resolution_clock::time_point pre_sleep_time = std::chrono::high_resolution_clock::now();
        // long long relative_ps_time = std::chrono::duration_cast<std::chrono::microseconds>(pre_sleep_time - call_time).count();

        QueryPerformanceCounter(&pre_sleep_time);
        long long relative_ps_time = (pre_sleep_time.QuadPart - call_time.QuadPart) * 1000000 / frequency.QuadPart;


        int relative_ps_time_mod = relative_ps_time % (COLLECTED_API_TIME_DELAY * 1000);
        int sleep_length = COLLECTED_API_TIME_DELAY - relative_ps_time_mod / 1000;
        //std::cout << "about to print at " << relative_time1 << std::endl;
        Sleep(sleep_length);
    }

    return 0;
}

void closeComms() {
    commsSending = FALSE;
    status = Real_NtClose(hPipe);
    status = Real_NtClose(hEvent);
    DeleteCriticalSection(&hLock);
}
