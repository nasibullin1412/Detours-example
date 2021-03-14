#include "hook.h"

HANDLE(WINAPI* pCreateFileA) (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = NULL;
HANDLE(WINAPI* pCreateFileW) (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = NULL;
HANDLE(WINAPI* pFindFirstFileW) (LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData) = NULL;
HANDLE(WINAPI* pFindFirstFileA)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) = NULL;
BOOL(WINAPI* pFindNextFileW) (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData) = NULL;
BOOL(WINAPI* pFindNextFileA) (HANDLE hFindFile, LPWIN32_FIND_DATAA  lpFindFileData) = NULL;
HANDLE(WINAPI* pFindFirstFileExA) (LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags) = NULL;
HANDLE(WINAPI* pFindFirstFileExW) (LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags) = NULL;



HANDLE WINAPI MyCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
)
{
	LAB2_PRINT("\nI'm here 1");
	return pCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


HANDLE WINAPI MyCreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
)
{
	LAB2_PRINT("\nI'm here 2");
	return pCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}



HANDLE WINAPI MyFindFirstFileA(
	LPCSTR lpFileName,
	LPWIN32_FIND_DATAA lpFindFileData
)
{
	LAB2_PRINT("\nI'm here 3");
	return pFindFirstFileA(lpFileName, lpFindFileData);
}



HANDLE WINAPI MyFindFirstFileW(
	LPCWSTR lpFileName,
	LPWIN32_FIND_DATA lpFindFileData
)
{
	LAB2_PRINT("\nI'm here 4");
	//return NULL;
	return pFindFirstFileW(lpFileName, lpFindFileData);
}


BOOL WINAPI MyFindNextFileA(
	HANDLE hFindFile,
	LPWIN32_FIND_DATAA lpFindFileData
)
{
	LAB2_PRINT("\nI'm here 5");
	return pFindNextFileA(hFindFile, lpFindFileData);
}


BOOL WINAPI MyFindNextFileW(
	HANDLE hFindFile,
	LPWIN32_FIND_DATAW lpFindFileData
)
{
	LAB2_PRINT("\nI'm here 6");
	return pFindNextFileW(hFindFile, lpFindFileData);
}

HANDLE WINAPI MyFindFirstFileExW(
	LPCWSTR lpFileName,
	FINDEX_INFO_LEVELS fInfoLvelld,
	LPWIN32_FIND_DATAW lpFindFileData,
	FINDEX_SEARCH_OPS fSearchop,
	LPVOID lpSearchFilter,
	DWORD dwAdditionalFlags
)
{
	LAB2_PRINT("\nI'm here 7");
	return pFindFirstFileExW(lpFileName, fInfoLvelld, lpFindFileData, fSearchop, lpSearchFilter, dwAdditionalFlags);
}


HANDLE WINAPI MyFindFirstFileExA(
	LPCSTR lpFileName,
	FINDEX_INFO_LEVELS fInfoLvelld,
	LPWIN32_FIND_DATAA lpFindFileData,
	FINDEX_SEARCH_OPS fSearchop,
	LPVOID lpSearchFilter,
	DWORD dwAdditionalFlags
)
{
	LAB2_PRINT("\nI'm here 8");
	return pFindFirstFileExA(lpFileName, fInfoLvelld, lpFindFileData, fSearchop, lpSearchFilter, dwAdditionalFlags);
}






BOOL CreateHook(PVOID* funcOrig, PVOID funcChange)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(funcOrig, funcChange);
	LONG err = DetourTransactionCommit();
	if (err == NO_ERROR)
	{
		LAB2_PRINT("\n[OK]: CreateHook () detoured successfully");
		return TRUE;
	}
	else
	{
		LAB2_PRINT("[ERROR] () detoured failed");
		return FALSE;
	}
}



BOOL HideFile(char* file_path)
{
	DetourRestoreAfterWith();	//Start finding function pointers
	/*psend = (int (WINAPI*)(SOCKET, const char*, int, int))
		DetourFindFunction("Ws2_32.dll", "send");
	precv = (int (WINAPI*)(SOCKET, char*, int, int))
		DetourFindFunction("Ws2_32.dll", "recv");
	pWSASend = (int (WINAPI*)(SOCKET, LPWSABUF, DWORD,
		LPDWORD, DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))
		DetourFindFunction("Ws2_32.dll", "WSASend");
	pWSARecv = (int (WINAPI*)(SOCKET, LPWSABUF, DWORD,
		LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))
		DetourFindFunction("Ws2_32.dll", "WSARecv");*/

	/*HANDLE(WINAPI * pCreateFileA) (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = NULL;
	HANDLE(WINAPI * pCreateFileW) (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = NULL;
	HANDLE(WINAPI * pFindFirstFileW) (LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData) = NULL;
	HANDLE(WINAPI * pFindFirstFileA)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) = NULL;
	BOOL(WINAPI * pFindNextFileW) (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData) = NULL;
	BOOL(WINAPI * pFindNextFileA) (HANDLE hFindFile, LPWIN32_FIND_DATAA  lpFindFileData) = NULL;
	HANDLE(WINAPI * pFindFirstFileExA) (LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags) = NULL;
	HANDLE(WINAPI * pFindFirstFileExW) (LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags) = NULL;
	*/



	pCreateFileA = (HANDLE (WINAPI*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))
		DetourFindFunction("kernel32.dll", "CreateFileA");
	pCreateFileW = (HANDLE(WINAPI*) (LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))
		DetourFindFunction("kernel32.dll", "CreateFileW");
	pFindFirstFileA = (HANDLE(WINAPI*)(LPCSTR, LPWIN32_FIND_DATAA))
		DetourFindFunction("kernel32.dll", "FindFirstFileW");
	pFindFirstFileExA = (HANDLE(WINAPI*) (LPCSTR, FINDEX_INFO_LEVELS, LPVOID, FINDEX_SEARCH_OPS, LPVOID, DWORD))
		DetourFindFunction("kernel32.dll", "FindFirstFileExA");
	pFindFirstFileW = (HANDLE(WINAPI*) (LPCWSTR, LPWIN32_FIND_DATA))
		DetourFindFunction("kernel32.dll", "FindFirstFileW");
	pFindFirstFileExW = (HANDLE(WINAPI*) (LPCWSTR, FINDEX_INFO_LEVELS, LPVOID, FINDEX_SEARCH_OPS, LPVOID, DWORD))
		DetourFindFunction("kernel32.dll", "FindFirstFileExW");
	pFindNextFileA = (BOOL(WINAPI*)(HANDLE, LPWIN32_FIND_DATAA))
		DetourFindFunction("kernel32.dll", "FindNextFileA");
	pFindNextFileW = (BOOL(WINAPI*)(HANDLE, LPWIN32_FIND_DATA))
		DetourFindFunction("kernel32.dll", "FindNextFileW");

	CreateHook(&(PVOID&) pCreateFileA, MyCreateFileA);
	CreateHook(&(PVOID&) pCreateFileW, MyCreateFileW);
	CreateHook(&(PVOID&) pFindFirstFileA, MyFindFirstFileA);
	CreateHook(&(PVOID&) pFindFirstFileExA, MyFindFirstFileExA);
	CreateHook(&(PVOID&) pFindFirstFileW, MyFindFirstFileW);
	CreateHook(&(PVOID&) pFindFirstFileExW, FindFirstFileExW);
	CreateHook(&(PVOID&) pFindNextFileA, FindNextFileA);
	CreateHook(&(PVOID&) pFindNextFileW, FindNextFileW);

    return 0;
}





