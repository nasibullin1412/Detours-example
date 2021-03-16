#include "hook.h"

HANDLE(WINAPI* pCreateFileA) (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = NULL;
HANDLE(WINAPI* pCreateFileW) (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = NULL;
HANDLE(WINAPI* pFindFirstFileW) (LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData) = NULL;
HANDLE(WINAPI* pFindFirstFileA)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) = NULL;
BOOL(WINAPI* pFindNextFileW) (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData) = NULL;
BOOL(WINAPI* pFindNextFileA) (HANDLE hFindFile, LPWIN32_FIND_DATAA  lpFindFileData) = NULL;
HANDLE(WINAPI* pFindFirstFileExA) (LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags) = NULL;
HANDLE(WINAPI* pFindFirstFileExW) (LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags) = NULL;


char full_name[MAX_RECSV] = { 0 };


wchar_t w_full_name[MAX_RECSV] = { 0 };



/*
Creates or opens a file or I/O device. 
The most commonly used I/O devices are as follows: file, file stream, directory, physical disk, volume, console buffer, tape drive,
communications resource, mailslot, and pipe. The function returns a handle that can be used to access the file or device for various
types of I/O depending on the file or device and the flags and attributes specified.
*/

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

	LAB2_PRINT("\n[MCFA]: ");
	OutputDebugStringW((LPCWSTR)lpFileName);
	if (strcmp(lpFileName, full_name) == 0)
	{
		return INVALID_HANDLE_VALUE;	
	}
	
	return pCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


/*
Creates or opens a file or I/O device. The most commonly used I/O devices are as follows:
file, file stream, directory, physical disk, volume, console buffer, tape drive, communications resource, mailslot, and pipe. 
The function returns a handle that can be used to access
the file or device for various types of I/O depending on the file or device and the flags and attributes specified.
*/

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
	LAB2_PRINT("\n[MCFW]: %d ", wcscmp(lpFileName, w_full_name));
	OutputDebugStringW((LPCWSTR)lpFileName);
	if (wcscmp(lpFileName, w_full_name) == 0)
	{
		return INVALID_HANDLE_VALUE;
	}
	return pCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


/*Searches a directory for a file or subdirectory with a name that matches a specific name (or partial name if wildcards are used).*/

HANDLE WINAPI MyFindFirstFileA(
	LPCSTR lpFileName,
	LPWIN32_FIND_DATAA lpFindFileData
)
{
	LAB2_PRINT("\n[MFFA]: ");
	OutputDebugStringW((LPCWSTR)lpFileName);
	if (strcmp(lpFileName, full_name) == 0)
	{
		return INVALID_HANDLE_VALUE;
	}
	return pFindFirstFileA(lpFileName, lpFindFileData);
}


/*Searches a directory for a file or subdirectory with a name that matches a specific name (or partial name if wildcards are used).*/

HANDLE WINAPI MyFindFirstFileW(
	LPCWSTR lpFileName,
	LPWIN32_FIND_DATA lpFindFileData
)
{
	LAB2_PRINT("\n[MFFW]: %d", wcscmp(lpFileName, w_full_name));
	OutputDebugStringW((LPCWSTR)lpFileName);
	if (wcscmp(lpFileName, w_full_name) == 0)
	{
		return INVALID_HANDLE_VALUE;
	}
	return pFindFirstFileW(lpFileName, lpFindFileData);
}


/*Continues a file search from a previous call to the FindFirstFile, FindFirstFileEx, or FindFirstFileTransacted functions.*/

BOOL WINAPI MyFindNextFileA(
	HANDLE hFindFile,
	LPWIN32_FIND_DATAA lpFindFileData
)
{
	LAB2_PRINT("\n[MCFNA]: MyFindNextFileA");
	bool ret = pFindNextFileA(hFindFile, lpFindFileData);
	if (strcmp(lpFindFileData->cFileName, full_name) == 0) {
		return 0;
	}
	return ret;
}


BOOL WINAPI MyFindNextFileW(
	HANDLE hFindFile,
	LPWIN32_FIND_DATAW lpFindFileData
)
{
	LAB2_PRINT("\n[MCFNW]: ");
	bool ret = pFindNextFileW(hFindFile, lpFindFileData);
	if (wcscmp(lpFindFileData->cFileName, w_full_name) == 0) {
		return 0;
	}
	return ret;

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
	LAB2_PRINT("\n[MFFFEW]: ");
	OutputDebugStringW((LPCWSTR)lpFileName);
	if (wcscmp(lpFileName, w_full_name) == 0)
	{
		return INVALID_HANDLE_VALUE;
	}
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
	LAB2_PRINT("\n[MFFFEA]: ");
	OutputDebugStringW((LPCWSTR)lpFileName);
	if (strcmp(lpFileName, full_name) == 0)
	{
		return INVALID_HANDLE_VALUE;
	}
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


BOOL ParsePath(char* full_path)
{
	
	memcpy_s(full_name, MAX_RECSV, full_path, MAX_RECSV);
	LAB2_PRINT("[FULL PATH]: ");
	OutputDebugStringA(full_name);
	LAB2_PRINT("\n[FULL WPATH]: ");
	int length = MultiByteToWideChar(1251, 0, full_name, -1, w_full_name, 0);
	MultiByteToWideChar(1251, 0, full_name, -1, w_full_name, length);
	OutputDebugStringW(w_full_name);
	return TRUE;


}


BOOL HideFile(char* file_path)
{
	DetourRestoreAfterWith();


	ParsePath(file_path);

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





