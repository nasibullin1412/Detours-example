#include "hook.h"
#include <detours.h>

#define MAX_PRINT 1025

void WINAPIV DebugOut(const char* fmt, ...) {
    char s[MAX_PRINT];
    va_list args;
    va_start(args, fmt);
    vsnprintf(s, MAX_PRINT, fmt, args);
    va_end(args);
    OutputDebugStringA(s);
}

#define FUNC_TO_HOOK "CreateFileW"

CHAR gFuncName[MAX_FUNC_NAME];
BOOLEAN Hooked = FALSE;
extern "C" LPVOID gOrigPointer = NULL;


extern "C" void myHook();
 
BOOLEAN CreateHook()
{

    if (Hooked == FALSE || gOrigPointer == NULL)
    {
        gOrigPointer = GetProcAddress(LoadLibrary(DLL_TO_HOOK), gFuncName);

        if (gOrigPointer == 0)
        {
            LAB2_PRINT("Error: Unable to get proc address%s!", gFuncName);
            return FALSE;
        }
        Hooked = TRUE;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)gOrigPointer, myHook);
    LONG err = DetourTransactionCommit();
    if (err == NO_ERROR)
    {
        LAB2_PRINT("%s () detoured successfully", gFuncName);
    }
    else
    {
        LAB2_PRINT("Err 0x%x %s () detoured failed", gFuncName);
        return FALSE;
    }
}

extern "C" VOID HookCallback()
{
    LAB2_PRINT("!%s () CALLED", gFuncName);
    //OutputDebugStringA("() CALLED");
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
        
        strncpy_s(gFuncName, FUNC_TO_HOOK, MAX_FUNC_NAME);
        DisableThreadLibraryCalls(hinstDLL);
        CreateHook();
        LAB2_PRINT("Process Atach...\n");

        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        LAB2_PRINT("Thread Atach...\n");

        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        LAB2_PRINT("Thread Detach...\n");
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        LAB2_PRINT("Process Detach...\n");
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}