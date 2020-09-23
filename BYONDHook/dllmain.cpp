// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <easyhook.h>

#pragma comment(lib, "EasyHook32.lib")

using namespace std;
int spoof1, spoof2;

#define IsByondMember_Ordinal MAKEINTRESOURCEA(1450)

extern "C" { __declspec(dllimport) void Init(); }

BOOL WINAPI GetVersionExW_hook(LPOSVERSIONINFOW lpVersionInformation)
{
    lpVersionInformation->dwOSVersionInfoSize = spoof2;
    return TRUE;
}

BOOL WINAPI GVIA_hook(LPCSTR lpRootPathName, LPSTR lpVolumeNameBuffer, DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
 
    *(DWORD*)lpVolumeSerialNumber = spoof1;
    return TRUE;
}

int __stdcall IsByondMember_hk(INT32* a2)
{
    return 1;
}

void hookVirtual(const char* hkName, void* InEntryPoint, void* InHookProc, void* InCallback, TRACED_HOOK_HANDLE OutHandle) {
    NTSTATUS result = LhInstallHook(InEntryPoint, InHookProc, InCallback, OutHandle);
    if (FAILED(result))
    {
        wstring s(RtlGetLastErrorString());
        wcout << "Failed to install hook: ";
        wcout << s << endl;
    }
    else
    {
        ULONG ACLEntries[1] = { 0 };
        LhSetExclusiveACL(ACLEntries, 1, OutHandle);
        printf("[+] %s hooked!\n", hkName);
    }
}

void Init() {
    DWORD VolumeSerialNumber;
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    printf("[>] Initializing... \n");
    HMODULE ByondCore = GetModuleHandleA("byondcore.dll");
    printf("[+] ByondCore.dll address: 0x%p\n", ByondCore);

    HOOK_TRACE_INFO hHook = { NULL };
    hookVirtual("IsByondMember", GetProcAddress(ByondCore, IsByondMember_Ordinal), IsByondMember_hk, NULL, &hHook);
    
    printf("[+] IsByondMember address: 0x%p\n", GetProcAddress(ByondCore, IsByondMember_Ordinal));

    HMODULE Kernel32 = LoadLibraryA("kernel32.dll");
    if (!Kernel32)
        printf("[!] FAILED TO GET KERNEL32 HANDLE\n");
    printf("[+] Got Kernel32 Handle!\n");

    int GetVolumeInformationA_pointer = (int)GetProcAddress(Kernel32, "GetVolumeInformationA");
    if (!GetVolumeInformationA_pointer)
        printf("[!] FAILED TO GET POINTER TO GetVolumeInformationA\n");
    printf("[+] Pointer to GetVolumeInformationA: 0x%p\n", GetVolumeInformationA_pointer);

    int GetVersionExW_pointer = (int)GetProcAddress(Kernel32, "GetVersionExW");
    if (!GetVersionExW_pointer)
        printf("[!] FAILED TO GET POINTER TO GetVersionExW\n");
    printf("[+] Pointer to GetVersionExW: 0x%p\n", GetVersionExW_pointer);
    
    GetVolumeInformationA("C:\\", 0, 0, &VolumeSerialNumber, 0, 0, 0, 0);
    cout << "[+] Current Serial ID: " << VolumeSerialNumber << endl;

    LPCTSTR path = ("C:\\cid.ini");
    spoof1 = GetPrivateProfileInt("CID", "GetVolumeInformationA", 696966, path);
    cout << "[*] Serial ID to spoof: " << spoof1 << endl;

    HOOK_TRACE_INFO hHook2 = { NULL };
    hookVirtual("GetVolumeInformationA", GetProcAddress(Kernel32, "GetVolumeInformationA"), GVIA_hook, NULL, &hHook2);

    spoof2 = GetPrivateProfileInt("CID", "GetVersionExW", 420, path);
    cout << "[*] Version to spoof: " << spoof2 << endl;

    HOOK_TRACE_INFO hHook3 = { NULL };
    hookVirtual("GetVersionExW", GetProcAddress(Kernel32, "GetVersionExW"), GetVersionExW_hook, NULL, &hHook3);

}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (PTHREAD_START_ROUTINE)Init, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

