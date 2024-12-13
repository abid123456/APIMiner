#include "analytics.h"
#include "string.h"

struct entry ApiEnumMap[] = {
    {"CoInitializeEx", Enum_CoInitializeEx},
    {"CreateActCtxW", Enum_CreateActCtxW},
    {"CreateDirectoryW", Enum_CreateDirectoryW},
    {"DeviceIoControl", Enum_DeviceIoControl},
    {"DrawTextExW", Enum_DrawTextExW},
    {"EnumWindows", Enum_EnumWindows},
    {"GetDiskFreeSpaceExW", Enum_GetDiskFreeSpaceExW},
    {"GetFileInformationByHandleEx", Enum_GetFileInformationByHandleEx},
    {"GetFileVersionInfoSizeW", Enum_GetFileVersionInfoSizeW},
    {"GetForegroundWindow", Enum_GetForegroundWindow},
    {"GetKeyState", Enum_GetKeyState},
    {"GetSystemMetrics", Enum_GetSystemMetrics},
    {"GetSystemTimeAsFileTime", Enum_GetSystemTimeAsFileTime},
    {"GetSystemWindowsDirectoryW", Enum_GetSystemWindowsDirectoryW},
    {"LoadResource", Enum_LoadResource},
    {"LdrGetDllHandle", Enum_LdrGetDllHandle},
    {"LdrLoadDll", Enum_LdrLoadDll},
    {"NtAllocateVirtualMemory", Enum_NtAllocateVirtualMemory},
    {"NtClose", Enum_NtClose},
    {"NtCreateFile", Enum_NtCreateFile},
    {"NtCreateKey", Enum_NtCreateKey},
    {"NtDeleteKey", Enum_NtDeleteKey},
    {"NtFreeVirtualMemory", Enum_NtFreeVirtualMemory},
    {"NtOpenFile", Enum_NtOpenFile},
    {"NtOpenKey", Enum_NtOpenKey},
    {"NtOpenMutant", Enum_NtOpenMutant},
    {"NtProtectVirtualMemory", Enum_NtProtectVirtualMemory},
    {"NtQueryAttributesFile", Enum_NtQueryAttributesFile},
    {"NtQueryDirectoryFile", Enum_NtQueryDirectoryFile},
    {"NtQueryInformationFile", Enum_NtQueryInformationFile},
    {"NtQueryKey", Enum_NtQueryKey},
    {"NtQueryValueKey", Enum_NtQueryValueKey},
    {"NtReadFile", Enum_NtReadFile},
    {"NtSetValueKey", Enum_NtSetValueKey},
    {"NtWriteFile", Enum_NtWriteFile},
    {"RegEnumKeyExW", Enum_RegEnumKeyExW},
    {"RegOpenKeyExW", Enum_RegOpenKeyExW},
    {"SearchPathW", Enum_SearchPathW},
    {"SendNotifyMessageW", Enum_SendNotifyMessageW},
    {"SetErrorMode", Enum_SetErrorMode},
    {"SetFileTime", Enum_SetFileTime},
    {"SHGetFolderPathW", Enum_SHGetFolderPathW},
    {0, 0}
};

int enumerate_api(const char* api_name) {
    int left = 0;
    int right = sizeof(ApiEnumMap) / sizeof(ApiEnumMap[0]) - 2;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(api_name, ApiEnumMap[mid].str);

        if (cmp == 0) return ApiEnumMap[mid].enumval;
        else if (cmp < 0) right = mid - 1;
        else left = mid + 1;
    }

    return -1;
}
