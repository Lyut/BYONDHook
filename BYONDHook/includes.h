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
#define DMTextPrinterBeep "?Beep@DMTextPrinter@@UAEXXZ"
#define DungClientIsByondMember "?IsByondMember@DungClient@@QAE_NPAJ@Z"
#define DungClientGetCurrentLoginKey "?GetCurrentLoginKey@DungClient@@QAEPBDXZ"
#define DungClientGetServerIP "?GetServerIP@DungClient@@QAEPBDPBD@Z"
#define ByondLibGetByondVersion "?GetByondVersion@ByondLib@@QAEJXZ"
#define ByondLibGetByondBuild "?GetByondBuild@ByondLib@@QAEJXZ"
#define ByondLibGetByondLabel "?GetByondLabel@ByondLib@@QAEPBDXZ"
#define ByondLibGetByondOsStr "?GetByondOsStr@ByondLib@@QAEPBDXZ"
#define ByondLibGetByondHubPath "?GetByondHubPath@ByondLib@@QAEPBDXZ"

HMODULE ByondCore = GetModuleHandleA("byondcore.dll");
HMODULE Kernel32Handle = GetModuleHandleA("kernel32.dll");

typedef DWORD(__thiscall* GetByondVersionFn)(void);
GetByondVersionFn GetByondVersion = (GetByondVersionFn)GetProcAddress(ByondCore, ByondLibGetByondVersion);
typedef DWORD(__thiscall* GetByondBuildFn)(void);
GetByondBuildFn GetByondBuild = (GetByondBuildFn)GetProcAddress(ByondCore, ByondLibGetByondBuild);
typedef const char* (__thiscall* GetByondLabelFn)(void);
GetByondLabelFn GetByondLabel = (GetByondLabelFn)GetProcAddress(ByondCore, ByondLibGetByondLabel);
typedef const char* (__thiscall* GetByondOsFn)(void);
GetByondOsFn GetByondOs = (GetByondOsFn)GetProcAddress(ByondCore, ByondLibGetByondOsStr);
typedef const char* (__thiscall* GetCurrentLoginKeyFn)(void);
GetCurrentLoginKeyFn GetCurrentLoginKey = (GetCurrentLoginKeyFn)GetProcAddress(ByondCore, DungClientGetCurrentLoginKey);
typedef const char* (__thiscall* GetServerIPFn)(void);
GetServerIPFn GetServerIP = (GetServerIPFn)GetProcAddress(ByondCore, DungClientGetServerIP);
typedef const char* (__thiscall* GetByondHubPathFn)(void);
GetByondHubPathFn GetByondHubPath = (GetByondHubPathFn)GetProcAddress(ByondCore, ByondLibGetByondHubPath);

typedef void(__stdcall* ComputeCid_t)(char* a1, size_t a2);
typedef void(__stdcall* GVEW_t)(LPOSVERSIONINFOW a1);
typedef void(__stdcall* DMTextPrinter_t)(void* _this);
typedef int(__stdcall* GVIA_t)(LPCSTR a1, LPSTR a2, DWORD a3, LPDWORD a4, LPDWORD a5, LPDWORD a6, LPSTR a7, DWORD a8);
GVIA_t GVIA_o = NULL;
GVEW_t GVEW_o = NULL;
DMTextPrinter_t DMTextPrinter_o = NULL;
ComputeCid_t ComputeCid_o = NULL;

PBYTE pCIDMD5 = (PBYTE)ByondCore + 0x373660;
PBYTE pCIDMD5unk = (PBYTE)ByondCore + 0x372E6C;
PBYTE pComputeCid = (PBYTE)ByondCore + 0x228D40;
PBYTE pUnkFunc = (PBYTE)ByondCore + 0x229AB0;

