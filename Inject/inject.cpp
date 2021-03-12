#include "inject.h"





DWORD getProcessID(LPCWSTR targetName) {
    DWORD processID = 0;
    HANDLE snapHandle;
    PROCESSENTRY32 processEntry = { 0 };

    if ((snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) {
        return 0;
    }

    processEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(snapHandle, &processEntry);
    do {
        if (wcscmp(processEntry.szExeFile, targetName) == 0) {
            return processEntry.th32ProcessID;
        }
    } while (Process32Next(snapHandle, &processEntry));

    if (snapHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(snapHandle);
    }

    return 0;
}


BOOL setPrivilege(HANDLE hToken, LPCTSTR szPrivName, BOOL fEnable) {
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    LookupPrivilegeValue(NULL, szPrivName, &tp.Privileges[0].Luid);
    tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    return((GetLastError() == ERROR_SUCCESS));
}



HANDLE InjectDllIntoProccess(DWORD procId, LPCWSTR dllName)
{
    HANDLE hToken = NULL;
    HANDLE hCurrentProc = GetCurrentProcess();
    HANDLE processHandel = NULL;
    LPVOID dll_name = 0;
    SIZE_T dwWritten = 0;
    LPVOID load_library_p = NULL;
    DWORD ThreadID = 0;
    HANDLE hThread = NULL;

    if (!OpenProcessToken(hCurrentProc, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken)) {
       LAB2_PRINT("OpenProcessToken Error", GetLastError());
       return NULL;
    }
    else {
        if (!setPrivilege(hToken, SE_DEBUG_NAME, TRUE)) {
            LAB2_PRINT("SetPrivlegesSE_DEBUG_NAME Error 0x%x!", GetLastError());
            return NULL;
        }
    }

    processHandel = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
    dll_name = VirtualAllocEx(processHandel, NULL, MAX_PATH*sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
    if (WriteProcessMemory(processHandel, dll_name, dllName,(lstrlen(dllName)+1)*sizeof(TCHAR), &dwWritten) == 0) {
        LAB2_PRINT("WriteProcessMemory error", GetLastError());
        return NULL;
    }

#ifdef UNICODE
    load_library_p = GetProcAddress(LoadLibrary(TEXT("kernel32.dll")), "LoadLibraryW");
#else
    LPVOID load_library_p = GetProcAddress(LoadLibrary(TEXT("kernek32.dll")), "LoadLibraryA");
#endif // UNICODE

    hThread = CreateRemoteThread(processHandel, NULL, 0, (LPTHREAD_START_ROUTINE)load_library_p, dll_name, 0, &ThreadID);
    if (hThread == NULL) {
        LAB2_PRINT("\nError creating thread 0x%x!", GetLastError());
        goto COMPLETE;
    }

COMPLETE:
    return hThread;


    return hThread;
}

void usage()
{
	printf("\nInject.exe <target_proc>...");

}


int main(int argc, char* argv[])
{
    LPCTSTR proc_name = TEXT("notepad.exe");
    LPCTSTR dll_name = TEXT("Hook.dll");

    DWORD procId = 0;
    HANDLE remoute_thread_h = NULL;
    TCHAR full_dll_path[MAX_PATH];

    memset(full_dll_path, 0, sizeof(full_dll_path));
    if (GetFullPathName(dll_name, sizeof(full_dll_path) / sizeof(TCHAR), full_dll_path, NULL) == 0)
    {
        LAB2_PRINT("Error");
    }

    if (argc < 1)
    {
        usage();
        return __LINE__;
    }
    LAB2_PRINT("\nStarting...");

    procId = getProcessID(proc_name);
    if (procId == 0)
    {
        LAB2_PRINT("\nERROR: Unable to find process %s", proc_name);
        return __LINE__;
    }
    remoute_thread_h = InjectDllIntoProccess(procId, full_dll_path);
    if (remoute_thread_h == NULL)
    {
        LAB2_PRINT("\nERROR: Unable to Inject DLL to prosess %s", proc_name);
        return __LINE__;
    }
    LAB2_PRINT("\nInjection success");
    LAB2_PRINT("\nWaiting for remoute thread...");

    WaitForSingleObject(remoute_thread_h, -1);
    while (1);
    LAB2_PRINT("\nComplited!");
    return 0;
}