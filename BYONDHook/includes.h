#pragma once
#include <windows.h>
#include <iostream>
#include <string>
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

typedef void(__stdcall* ComputeCid_t)(char* a1, size_t a2);
typedef void(__stdcall* GVEW_t)(LPOSVERSIONINFOW a1);
typedef int(__stdcall* GVIA_t)(LPCSTR a1, LPSTR a2, DWORD a3, LPDWORD a4, LPDWORD a5, LPDWORD a6, LPSTR a7, DWORD a8);
GVIA_t GVIA_o = NULL;
GVEW_t GVEW_o = NULL;
ComputeCid_t ComputeCid_o = NULL;

HMODULE ByondCore = GetModuleHandleA("byondcore.dll");
HMODULE Kernel32Handle = GetModuleHandleA("kernel32.dll");

PBYTE pCIDMD5 = (PBYTE)ByondCore + 0x373660;
PBYTE pCIDMD5unk = (PBYTE)ByondCore + 0x372E6C;
PBYTE pComputeCid = (PBYTE)ByondCore + 0x228D40;
PBYTE pUnkFunc = (PBYTE)ByondCore + 0x229AB0;

