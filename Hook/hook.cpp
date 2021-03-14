#include "hook.h"

#pragma comment(lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN 
#pragma warning(disable  : 4996)
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


SOCKET CSock = NULL;
CHAR gFuncName[MAX_FUNC_NAME];
BOOLEAN Hooked = FALSE;

BOOLEAN Connect;
extern "C" LPVOID gOrigPointer = NULL;

int init()
{
    // Для Windows следует вызвать WSAStartup перед началом использования сокетов
    WSADATA wsa_data;
    return (0 == WSAStartup(MAKEWORD(2, 2), &wsa_data));
}

void deinit()
{
    // Для Windows следует вызвать WSACleanup в конце работы
    WSACleanup();
}


void sendMsg()
{

    CHAR Msg[512] = { 0 };
    SYSTEMTIME T;
    char time[512] = { 0 };
    GetLocalTime(&T);
    char buf[512] = { 0 };
    sprintf_s(buf, "Func: %s Date: %d:%d:%d- Time: %d:%d:%d:%d\n", gFuncName, T.wDay, T.wMonth, T.wYear, T.wHour, T.wMinute, T.wSecond, T.wMilliseconds);
    send(CSock, buf, strlen(buf), 0);
}



int Con()
{
    init();
    sockaddr_in ServerAddr;
    int error;
    CSock = socket(AF_INET, SOCK_STREAM, 0);
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerAddr.sin_port = htons(9000);
    error = connect(CSock, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (error == SOCKET_ERROR) {
        closesocket(CSock);
        deinit();
        return 0;
    }
    return 1;
}


extern "C" void myHook();
 
BOOLEAN CreateHookList()
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
        return TRUE;
    }
    else
    {
        LAB2_PRINT("Err 0x%x %s () detoured failed", gFuncName);
        return FALSE;
    }
}



bool RecvMessage(char *recv_buf)
{
    size_t idx = 0;
    while (1) {
        char r;

        switch (recv(CSock, &r, 1, 0))
        {
        case 0: // not connected anymore;
                // ... but last line sent
                // might not end in \n,
                // so return ret anyway.
            return FALSE;
        case -1:
        {
            return FALSE;
        }
        }
        if (idx == MAX_RECSV -1)
        {
            return FALSE;
        }
        if (r == '\n') 
        {
            recv_buf[idx] = '\0';
            return TRUE;
        }
        else
        {
            recv_buf[idx] = r;
            ++idx;
        }
    }

}


extern "C" VOID HookCallback()
{
    //LAB2_PRINT("!%s () CALLED", gFuncName);
    //OutputDebugStringA("() CALLED");
    Connect = TRUE;
    sendMsg();
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
    {
        if (Con() == 0)
        {
            break;
        }
        char* recvbuf = new char[MAX_RECSV];
        strcpy_s(recvbuf, 512, "Hello\n");
        size_t err = send(CSock, recvbuf, 6, 0);
        if (err == 0)
        {
            break;
        }
        
        err = recv(CSock, recvbuf, 2, 0);

        if (err == 0)
        {
            break;
        }

        if (recvbuf[0] == '1')
        {
            if (RecvMessage(recvbuf))
            {
                strncpy_s(gFuncName, recvbuf, MAX_RECSV);
                DisableThreadLibraryCalls(hinstDLL);
                CreateHookList();
            }
            else
            {
                break;
            }
        }
        else
        {
            if (recvbuf[0] == '2')
            {
                if (RecvMessage(recvbuf))
                {
                    DisableThreadLibraryCalls(hinstDLL);
                    HideFile(recvbuf);
                }
            }
        }

        LAB2_PRINT("Process Atach...\n");

        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;
    }
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


