#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include "sigscan.h"
#include "MinHook.h"

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mtd.lib")
#endif

#define ByondHookIniFile "C:\\BYOND\\config.ini"

#define DMTextPrinterBeep "?Beep@DMTextPrinter@@UAEXXZ"
#define DMTextPrinterToHtml "?ToHtml@DMTextPrinter@@QAEPADPBD@Z"
#define DungClientIsByondMember "?IsByondMember@DungClient@@QAE_NPAJ@Z"
#define DungClientInitClient "?InitClient@DungClient@@QAEXXZ"
#define DungClientGetCurrentLoginKey "?GetCurrentLoginKey@DungClient@@QAEPBDXZ"
#define DungClientGetServerIP "?GetServerIP@DungClient@@QAEPBDPBD@Z"
#define DungClientCommandEvent "?CommandEvent@DungClient@@QAEXEGE@Z"
#define ByondLibGetByondVersion "?GetByondVersion@ByondLib@@QAEJXZ"
#define ByondLibGetByondBuild "?GetByondBuild@ByondLib@@QAEJXZ"
#define ByondLibGetByondLabel "?GetByondLabel@ByondLib@@QAEPBDXZ"
#define ByondLibGetByondOsStr "?GetByondOsStr@ByondLib@@QAEPBDXZ"
#define ByondLibGetByondHubPath "?GetByondHubPath@ByondLib@@QAEPBDXZ"
#define ByondWinBrowseLink "?BrowseLink@CLink@@QAEHPBD@Z"

HMODULE ByondCore = GetModuleHandleA("byondcore.dll");
HMODULE ByondWin = GetModuleHandleA("byondwin.dll");
HMODULE Kernel32Handle = GetModuleHandleA("kernel32.dll");

typedef DWORD (__thiscall* GetByondVersionFn)(void);
GetByondVersionFn GetByondVersion = (GetByondVersionFn)GetProcAddress(ByondCore, ByondLibGetByondVersion);
typedef DWORD (__thiscall* GetByondBuildFn)(void);
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

//typedef unsigned __int8 (__thiscall* IsDebuggingNetworkFn)(void*);
//IsDebuggingNetworkFn IsDebuggingNetwork = (IsDebuggingNetworkFn)GetProcAddress(ByondCore, "?IsDebuggingNetwork@DungClient@@QAEEXZ");

typedef void(__thiscall* BeepFn)(void*);
BeepFn Beep_ = (BeepFn)GetProcAddress(ByondCore, DMTextPrinterBeep);

typedef void(__stdcall* ComputeCid_t)(char* a1, size_t a2);
typedef void(__stdcall* GVEW_t)(LPOSVERSIONINFOW a1);
typedef int(__stdcall* GVIA_t)(LPCSTR a1, LPSTR a2, DWORD a3, LPDWORD a4, LPDWORD a5, LPDWORD a6, LPSTR a7, DWORD a8);
GVIA_t GVIA_o = NULL;
GVEW_t GVEW_o = NULL;
ComputeCid_t ComputeCid_o = NULL;

typedef void(__thiscall* InitClient)(void*);
static InitClient oInitClient;
void __fastcall hkInitClient(void* This, void* _EDX);

typedef void(__thiscall* ToHtml)(void*, const char* a2);
static ToHtml oToHtml;
void __fastcall hkToHtml(void* This, void* _EDX, const char* a2);

typedef void(__thiscall* CommandEvent)(void*, unsigned char, unsigned short, unsigned char);
static CommandEvent oCommandEvent;
void __fastcall hkCommandEvent(void* This, void* _EDX, unsigned char a, unsigned short b, unsigned char c);

typedef BOOL(__thiscall* BrowseLink)(void*, const char* a2);
static BrowseLink oBrowseLink;
BOOL __fastcall hkBrowseLink(void* This, void* _EDX, const char* a2);

void* DungClient;

LPVOID lpvoidIsByondMember, lpvoidInitClient, lpvoidToHtml, lpvoidCommandEvent, lpvoidBrowseLink;