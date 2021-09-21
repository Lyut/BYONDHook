#pragma once
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <regex>
#include "MinHook.h"

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mtd.lib")
#endif
/*
Name								Address		Ordinal
DungClient::IsByondMember(long *)	10078AA0	1652
DungPager::IsByondMember(long *)	1024F270	1653
*/
#define DungClientIsByondMember MAKEINTRESOURCEA(1652)

