// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "includes.h"

extern "C" { __declspec(dllimport) void Main(); }

int __cdecl IsByondMember_hk(int* a1)
{
    return 1;
}

void Main() {
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << "BYONDInstantHook for BYOND 514.1566 Build " __DATE__ << " " << __TIME__ << std::endl;
    HMODULE ByondCore = GetModuleHandleA("byondcore.dll");
    printf("[+] ByondCore.dll address: 0x%p\n", ByondCore);

    PBYTE pCIDMD5 = (PBYTE)ByondCore + 0x373660;
    PBYTE pCIDMD52 = (PBYTE)ByondCore + 0x372E6C;

    if (MH_Initialize() != MH_OK)
    {
        std::cout << "[-] Failed to initialize hook management!" << std::endl;
    }
    if (MH_CreateHook(GetProcAddress(ByondCore, DungClientIsByondMember), &IsByondMember_hk, NULL) != MH_OK)
    {
        std::cout << "Failed to hook DungClient::IsByondMember..." << std::endl;
    }
    printf("[+] IsByondMember hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientIsByondMember));

    char workingDir[MAX_PATH];
    GetModuleFileNameA(GetModuleHandle(0), workingDir, sizeof(workingDir));
    std::string sWorkingDir = workingDir;
    if (!sWorkingDir.empty()) {
        sWorkingDir.resize(sWorkingDir.size() - 15); //dreamseeker.exe
        sWorkingDir.append("cid.ini");
    }

    char spoofMD5[32];
    GetPrivateProfileStringA("CID", "md5", "172346606e1d24062e891d537e917a90", spoofMD5, 32, sWorkingDir.c_str());

    printf("[+] Patching CID with %s...\n", spoofMD5);

    DWORD OldProtection;
    VirtualProtect(pCIDMD5, 32, PAGE_EXECUTE_READWRITE, &OldProtection);
    VirtualProtect(pCIDMD52, 32, PAGE_EXECUTE_READWRITE, &OldProtection);
    memcpy(pCIDMD5, spoofMD5, 32);
    memcpy(pCIDMD52, spoofMD5, 32);
    VirtualProtect(pCIDMD5, 32, OldProtection, &OldProtection);
    VirtualProtect(pCIDMD52, 32, OldProtection, &OldProtection);

    printf("[+] CID Spoofed!");
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

