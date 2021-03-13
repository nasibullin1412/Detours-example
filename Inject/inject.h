#pragma once

#include "mysocket.h"
#include "share.h"
#include <tlhelp32.h>
#include <ShlObj.h>
#include <vector>
#include <string>

#define LAB2_PRINT(a, ...) {printf("LAB2 Inject" a, ##__VA_ARGS__); fflush(stdout);}


namespace myconst 
{
    const std::string var_func = "-func";
    const std::string var_pid = "-pid";
    const std::string var_name = "-name";
    const std::string var_hide = "-hide";
    constexpr size_t max_length = 150;

}


enum MyArgs
{
	FIRST_ARG,
	SECOND_ARG = 2
};