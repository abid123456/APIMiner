#ifndef _ANALYTICS_H
#define _ANALYTICS_H

enum AnalyzedFunctions {
    // Paper functions
    Enum_RegEnumKeyExW,
    Enum_CreateDirectoryW,
    Enum_DrawTextExW,
    Enum_CoInitializeEx,
    Enum_NtDeleteKey,
    Enum_SHGetFolderPathW,
    Enum_GetFileInformationByHandleEx,
    Enum_GetForegroundWindow,
    Enum_NtQueryAttributesFile,
    Enum_DeviceIoControl,
    Enum_SearchPathW,
    Enum_SetFileTime,
    Enum_SendNotifyMessageW,
    Enum_GetSystemMetrics,
    Enum_GetKeyState,
    Enum_NtCreateKey,
    Enum_LoadResource,
    Enum_GetDiskFreeSpaceExW,
    Enum_EnumWindows,
    Enum_RegOpenKeyExW,
    Enum_NtQueryKey,
    Enum_NtQueryValueKey,
    Enum_NtSetValueKey,
    Enum_CreateActCtxW,
    Enum_GetSystemTimeAsFileTime,
    Enum_GetSystemWindowsDirectoryW,
    Enum_SetErrorMode,
    Enum_GetFileVersionInfoSizeW,
    Enum_NtOpenMutant,

    // Additional Functions
    Enum_NtOpenKey,
    Enum_NtClose,
    Enum_NtCreateFile,
    Enum_NtReadFile,
    Enum_NtWriteFile,
    Enum_LdrGetDllHandle,
    Enum_NtOpenFile,
    Enum_NtFreeVirtualMemory,
    Enum_NtAllocateVirtualMemory,
    Enum_NtProtectVirtualMemory,
    Enum_LdrLoadDll,
    Enum_NtQueryInformationFile,
    Enum_NtQueryDirectoryFile
};

struct entry {
    char* str;
    int enumval;
};

int enumerate_api(const char* api_call);

#endif