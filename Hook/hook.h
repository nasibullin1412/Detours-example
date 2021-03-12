
#pragma once

#include <share.h>
#define MAX_FUNC_NAME 1024
#define DLL_TO_HOOK TEXT("kernel32.dll")


#define LAB2_PRINT(a, ...) DebugOut("LAB2 HOOK " a, ##__VA_ARGS__)