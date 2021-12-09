// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "includes.h"

extern "C" { __declspec(dllimport) void Main(); }

int __stdcall IsByondMember_hk(int* a1)
{
    return 1;
}

BOOL __stdcall GVEW_hk(LPOSVERSIONINFOW a1)
{
    GVEW_o(a1);
    a1->dwPlatformId = 3;

    return FALSE;
}

BOOL __stdcall GVIA_hk(LPCSTR a1, LPSTR a2, DWORD a3, LPDWORD a4, LPDWORD a5, LPDWORD a6, LPSTR a7, DWORD a8)
{    
    UINT spoofedVolume = GetPrivateProfileIntA("CID", "volume", 7776, "C:\\BYOND\\cid.ini");
    printf("[!] GVIA called! Spoofing CID to %i...\n", spoofedVolume);
    printf("[!] Logging in as %s...\n", GetCurrentLoginKey());
    *(DWORD*)a4 = spoofedVolume;

    return TRUE;
}

void* __stdcall ComputeCid_hk(char* a1, size_t a2)
{
    printf("[!] ComputeCid called: %s\n", a1);

    //ComputeCid_o(a1, a2);
    /*char workingDir[MAX_PATH];
    GetModuleFileNameA(GetModuleHandle(0), workingDir, sizeof(workingDir));
    std::string sWorkingDir = workingDir;
    if (!sWorkingDir.empty()) {
        sWorkingDir.resize(sWorkingDir.size() - 15); //dreamseeker.exe
        sWorkingDir.append("cida.ini");
    }

    char spoofMD5[32];
    GetPrivateProfileStringA("CID", "md5", "172346606e1d24062e891d537e917a90", spoofMD5, 32, sWorkingDir.c_str()); 
    printf("[+] Patching CID with %s...\n", spoofMD5); 

    DWORD OldProtection;
    VirtualProtect(pCIDMD5, 32, PAGE_EXECUTE_READWRITE, &OldProtection);
    VirtualProtect(pCIDMD5unk, 32, PAGE_EXECUTE_READWRITE, &OldProtection);
    printf("[+] Protection changed...\n");
    memcpy(pCIDMD5, spoofMD5, 32);
    memcpy(pCIDMD5unk, spoofMD5, 32);
    printf("[+] Memory patched...\n");
    VirtualProtect(pCIDMD5, 32, OldProtection, &OldProtection);
    VirtualProtect(pCIDMD5unk, 32, OldProtection, &OldProtection);
    printf("[+] Protection changed back to original\n");*/
}

void Main() {
    AllocConsole();
    SetConsoleTitleA("ByondInstantHook");
    freopen_s((FILE**)stdin, "conin$", "r", stdin);
    freopen_s((FILE**)stdout, "conout$", "w", stdout);
    std::cout << "BYONDInstantHook build " __DATE__ << " " << __TIME__ << std::endl;
    printf("[+] ByondCore.dll address: 0x%p\n", ByondCore);
    printf("[+] kernel32.dll address: 0x%p\n", Kernel32Handle);
    printf("=====================ByondLib=====================\n");
    printf("BYOND %s, version %i.%i for %s\n", GetByondLabel(), GetByondVersion(), GetByondBuild(), GetByondOs());
    printf("BYOND HUB Path: %s\n", GetByondHubPath());
    printf("BYONDInstantHook supported version: 514.1571\n");
    printf("==================================================\n");

    if (MH_Initialize() != MH_OK)
    {
        std::cout << "[-] Failed to initialize hook management!" << std::endl;
    }

    if (MH_CreateHook(GetProcAddress(ByondCore, DungClientIsByondMember), &IsByondMember_hk, NULL) != MH_OK)
    {
        std::cout << "Failed to hook DungClient::IsByondMember..." << std::endl;
    }

    printf("[+] IsByondMember hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientIsByondMember));

    if (MH_CreateHookApiEx(L"kernel32.dll", "GetVolumeInformationA", GVIA_hk, (LPVOID*)&GVIA_o, NULL) != MH_OK)
    {
        std::cout << "Failed to hook Kernel32::GetVolumeInformationA..." << std::endl;
    }

    printf("[+] GetVolumeInformationA function hooked, address: 0x%p\n", GVIA_o);

    if (MH_CreateHookApiEx(L"kernel32.dll", "GetVersionExW", GVEW_hk, (LPVOID*)&GVEW_o, NULL) != MH_OK)
    {
        std::cout << "Failed to hook Kernel32::GetVersionExW..." << std::endl;
    }

    printf("[+] GetVersionExW function hooked, address: 0x%p\n", GVEW_o);

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        std::cout << "Fatal Error!" << std::endl;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

