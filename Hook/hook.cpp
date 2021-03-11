#include "hook.h"

#define MAX_PRINT 1025

void WINAPIV DebugOut(const char* fmt, ...) {
    char s[MAX_PRINT];
    va_list args;
    va_start(args, fmt);
    vsnprintf(s, MAX_PRINT, fmt, args);
    va_end(args);
    OutputDebugStringA(s);
}



BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: 
        OutputDebugStringW(L"My output string.");
        LAB2_PRINT("Process Atach...");

        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        LAB2_PRINT("Thread Atach...");
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        LAB2_PRINT("Thread Detach...");
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        LAB2_PRINT("Process Detach...");
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}