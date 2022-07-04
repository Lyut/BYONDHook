// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#include "includes.h"

extern "C" { __declspec(dllimport) void Main(); }

void __fastcall hkInitClient(void* This, void* _EDX)
{
    printf("[!] DungClient::InitClient called, stealing class instance...\n");
    DungClient = This;
    return oInitClient(This);
}

void __fastcall hkToHtml(void* This, void* _EDX, const char* a2)
{
    UINT biLogToHtml = GetPrivateProfileIntA("Settings", "LogToHtml", 0, ByondHookIniFile);

    if (!a2)
        return oToHtml(This, a2);

    if (biLogToHtml == 1)
        if (a2 && a2 != nullptr)
            printf_s("[toHtml] %s\n", a2);

    if (a2[21] == char(76) && a2[31] == char(46)) {
        printf("[BYOND] Logging in...\n");
    }
    else if (a2[21] == char(99) && a2[29] == char(100)) {
        printf("[BYOND] connected\n");
    }
    else if (a2[21] == char(76) && a2[32] == char(115)) {
        std::string strTmp = a2;
        try {
            strTmp = strTmp.substr(21);
        }
        catch (std::out_of_range&) {
            return;
        }
        size_t pos = strTmp.find("</span>");
        if (pos != std::string::npos)
        {
            strTmp.erase(pos - 1, std::string("</span>").length() + 1);
        }
        printf_s("[BYOND] %s\n", strTmp.c_str());
    }
    else if (a2[21] == char(67) && a2[30] == char(103)) {
        std::string strTmp = a2;
        try {
            strTmp = strTmp.substr(21);
        }
        catch (std::out_of_range&) {
            return;
        }
        size_t pos = strTmp.find("</span>");
        if (pos != std::string::npos)
        {
            strTmp.erase(pos, std::string("</span>").length());
        }
        printf_s("[BYOND] %s\n", strTmp.c_str());
    }
    return oToHtml(This, a2);
}

BOOL __fastcall hkBrowseLink(void* This, void* _EDX, const char* a2)
{
    DWORD* dwLinkPtr;
    __asm
    {
        push eax
        mov eax, esi
        mov dwLinkPtr, eax
        pop eax
    }

    char* linkString = new char[520];
    linkString = (char*)dwLinkPtr;

    if (true && linkString && linkString != nullptr)
    {
        printf_s("[!] Blocked ByondWin::CLink::BrowseLink! => %s\n", linkString);
    }

    return TRUE;
}

void __fastcall hkCommandEvent(void* This, void* _EDX, unsigned char a, unsigned short b, unsigned char c)
{
    DWORD* dwCommandPtr;
    DWORD dwCommandLength;

    __asm
    {
        push eax
        mov eax, ebx
        mov dwCommandPtr, eax
        mov eax, edx
        mov dwCommandLength, eax
        pop eax
    }

    if (!dwCommandLength)
        return oCommandEvent(This, a, b, c);

    if (dwCommandLength > 1000)
        return oCommandEvent(This, a, b, c);

    char* commandString = new char[dwCommandLength - 1];
    commandString = (char*)dwCommandPtr;

    if (true && commandString && commandString != nullptr)
    {
        printf_s("[DungClient::CommandEvent]: %s\n", commandString);
    }

    return oCommandEvent(This, a, b, c);
}

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
    UINT iSpoofedVolume = GetPrivateProfileIntA("Settings", "Volume", 94545, ByondHookIniFile);
    printf_s("[!] GVIA called! Spoofing CID to %i...\n", iSpoofedVolume);
    //printf("[!] Logging in as %s...\n", GetCurrentLoginKey());
    *(DWORD*)a4 = iSpoofedVolume;

    return TRUE;
}

void Main() {

    UINT ibAllocConsole = GetPrivateProfileIntA("Settings", "AllocConsole", 1, ByondHookIniFile);
    UINT ibHookIsByondMember = GetPrivateProfileIntA("Hooks", "IsByondMember", 1, ByondHookIniFile);
    UINT ibHookInitClient = GetPrivateProfileIntA("Hooks", "InitClient", 1, ByondHookIniFile);
    UINT ibHookToHtml = GetPrivateProfileIntA("Hooks", "ToHtml", 1, ByondHookIniFile);
    UINT ibHookGetVolumeInformationA = GetPrivateProfileIntA("Hooks", "GetVolumeInformationA", 1, ByondHookIniFile);
    UINT ibHookGetVersionExW = GetPrivateProfileIntA("Hooks", "GetVersionExW", 1, ByondHookIniFile);
    UINT ibHookCommandEvent = GetPrivateProfileIntA("Hooks", "CommandEvent", 1, ByondHookIniFile);
    UINT ibHookBrowseLink = GetPrivateProfileIntA("Hooks", "BrowseLink", 0, ByondHookIniFile);

    if (ibAllocConsole == 1) {
        AllocConsole();
        SetConsoleTitleA("ByondInstantHook");
        freopen_s((FILE**)stdin, "conin$", "r", stdin);
        freopen_s((FILE**)stdout, "conout$", "w", stdout);
        std::cout << "BYONDInstantHook build " __DATE__ << " " << __TIME__ << std::endl;
        printf_s("[+] ByondCore.dll address: 0x%p\n", ByondCore);
        printf_s("[+] ByondWin.dll address: 0x%p\n", ByondWin);
        printf_s("[+] kernel32.dll address: 0x%p\n", Kernel32Handle);
        printf_s("=====================ByondLib=====================\n");
        printf_s("BYOND %s, version %i.%i for %s\n", GetByondLabel(), GetByondVersion(), GetByondBuild(), GetByondOs());
        printf_s("BYOND HUB Path: %s\n", GetByondHubPath());
        printf_s("BYONDInstantHook supported version: 514.1585\n");
        printf_s("==================================================\n");
    }

    if (MH_Initialize() != MH_OK)
    {
        std::cout << "[-] Failed to initialize hook management!" << std::endl;
        return;
    }

    if (ibHookIsByondMember == 1)
    {
        if (MH_CreateHook(GetProcAddress(ByondCore, DungClientIsByondMember) ?
            GetProcAddress(ByondCore, DungClientIsByondMember) :
            (LPVOID)(sigscan::FindPattern("byondcore.dll",
                "\x55\x8B\xEC\xFF\x75?\xE8????\x83\xC4?\x5D\xC2??\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xE9????\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x83\x3D?????\x74?\x68????\x68????\x6A?\xE8????\x83\xC4?\x56")),
            &IsByondMember_hk, NULL) != MH_OK)
        {
            std::cout << "Failed to hook DungClient::IsByondMember..." << std::endl;
            return;
        }
        printf_s("[+] byondcore.dll->DungClient::IsByondMember hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientIsByondMember));
    }

    if (ibHookInitClient == 1)
    {
        if (MH_CreateHook(GetProcAddress(ByondCore, DungClientInitClient) ?
            GetProcAddress(ByondCore, DungClientInitClient) :
            (LPVOID)(sigscan::FindPattern("byondcore.dll",
                "\xE9????\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\xFF\x75?\xE8????\x83\xC4?\x5D\xC2??\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xE9????\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x83\x3D?????\x74?\x68????\x68????\x6A?\xE8????\x83\xC4?\x56")),
            hkInitClient, reinterpret_cast<LPVOID*>(&oInitClient)) != MH_OK)
        {
            std::cout << "Failed to hook DungClient::InitClient..." << std::endl;
            return;
        }
        printf_s("[+] byondcore.dll->DungClient::InitClient hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientInitClient));
    }

    if (ibHookToHtml == 1)
    {
        if (MH_CreateHook(GetProcAddress(ByondCore, DMTextPrinterToHtml) ?
            GetProcAddress(ByondCore, DMTextPrinterToHtml) :
            (LPVOID)(sigscan::FindPattern("byondcore.dll",
                "\x55\x8B\xEC\x6A?\x68????\x64\xA1????\x50\x83\xEC?\xA1????\x33\xC5\x89\x45?\x53\x56\x57\x50\x8D\x45?\x64\xA3????\x8B\xD9\x89\x5D")),
            hkToHtml, reinterpret_cast<LPVOID*>(&oToHtml)) != MH_OK)
        {
            std::cout << "Failed to hook DMTextPrinter::ToHtml..." << std::endl;
            return;
        }
        printf_s("[+] byondcore.dll->DMTextPrinter::ToHtml hooked, address: 0x%p\n", GetProcAddress(ByondCore, DMTextPrinterToHtml));
    }

    if (ibHookCommandEvent == 1)
    {
        if (MH_CreateHook(GetProcAddress(ByondCore, DungClientCommandEvent) ?
            GetProcAddress(ByondCore, DungClientCommandEvent) :
            (LPVOID)(sigscan::FindPattern("byondcore.dll",
                "\x55\x8B\xEC\xFF\x75?\xFF\x75?\xFF\x75?\xE8????\x83\xC4?\x5D\xC2??\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xE9????\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55")),
            hkCommandEvent, reinterpret_cast<LPVOID*>(&oCommandEvent)) != MH_OK)
        {
            std::cout << "Failed to hook DungClient::CommandEvent..." << std::endl;
            return;
        }
        printf_s("[+] byondcore.dll->DungClient::CommandEvent hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientCommandEvent));
    }

    if (ibHookGetVolumeInformationA == 1)
    {
        if (MH_CreateHookApiEx(L"kernel32.dll", "GetVolumeInformationA", GVIA_hk, (LPVOID*)&GVIA_o, NULL) != MH_OK)
        {
            std::cout << "Failed to hook Kernel32::GetVolumeInformationA..." << std::endl;
            return;
        }
        printf_s("[+] kernel32.dll->GetVolumeInformationA function hooked, address: 0x%p\n", GVIA_o);
    }

    if (ibHookGetVersionExW == 1)
    {
        if (MH_CreateHookApiEx(L"kernel32.dll", "GetVersionExW", GVEW_hk, (LPVOID*)&GVEW_o, NULL) != MH_OK)
        {
            std::cout << "Failed to hook Kernel32::GetVersionExW..." << std::endl;
            return;
        }
        printf_s("[+] kernel32.dll->GetVersionExW function hooked, address: 0x%p\n", GVEW_o);
    }

    if (ibHookBrowseLink == 1)
    {
        if (MH_CreateHook(GetProcAddress(ByondWin, ByondWinBrowseLink) ?
            GetProcAddress(ByondWin, ByondWinBrowseLink) :
            (LPVOID)(sigscan::FindPattern("byondwin.dll",
                "\x55\x8B\xEC\x6A?\x68????\x64\xA1????\x50\x81\xEC????\xA1????\x33\xC5\x89\x45?\x53\x56\x57\x50\x8D\x45?\x64\xA3????\x89\x8D????\x8B\x45")),
            hkBrowseLink, reinterpret_cast<LPVOID*>(&oBrowseLink)) != MH_OK)
        {
            std::cout << "Failed to hook ByondWin::CLink::BrowseLink..." << std::endl;
            return;
        }
        printf_s("[+] ByondWin.dll->CLink::BrowseLink function hooked, address: 0x%p\n", GVEW_o);
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        std::cout << "Fatal Error!" << std::endl;
        return;
    }

}

BOOL APIENTRY DllMain(HMODULE hModule,
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

