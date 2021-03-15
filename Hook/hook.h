
#pragma once
#include <locale.h>
#include <share.h>
#include <detours.h>
#define MAX_FUNC_NAME 1024
#define MAX_RECSV 512
#define DLL_TO_HOOK TEXT("kernel32.dll")


#define LAB2_PRINT(a, ...) DebugOut(" " a, ##__VA_ARGS__)


BOOL ParsePath(char* full_path);


HANDLE WINAPI MyCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
);


HANDLE WINAPI MyCreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
);



HANDLE WINAPI MyFindFirstFileA(
	LPCSTR lpFileName,
	LPWIN32_FIND_DATAA lpFindFileData
);



HANDLE WINAPI MyFindFirstFileW(
	LPCWSTR lpFileName,
	LPWIN32_FIND_DATA lpFindFileData
);


BOOL WINAPI MyFindNextFileA(
	HANDLE hFindFile,
	LPWIN32_FIND_DATAA lpFindFileData
);


BOOL WINAPI MyFindNextFileW(
	HANDLE hFindFile,
	LPWIN32_FIND_DATAW lpFindFileData
);

HANDLE WINAPI MyFindFirstFileExW(
	LPCWSTR lpFileName,
	FINDEX_INFO_LEVELS fInfoLvelld,
	LPWIN32_FIND_DATAW lpFindFileData,
	FINDEX_SEARCH_OPS fSearchop,
	LPVOID lpSearchFilter,
	DWORD dwAdditionalFlags
);


HANDLE WINAPI MyFindFirstFileExA(
	LPCSTR lpFileName,
	FINDEX_INFO_LEVELS fInfoLvelld,
	LPWIN32_FIND_DATAA lpFindFileData,
	FINDEX_SEARCH_OPS fSearchop,
	LPVOID lpSearchFilter,
	DWORD dwAdditionalFlags
);







BOOL HideFile(char* file_path);