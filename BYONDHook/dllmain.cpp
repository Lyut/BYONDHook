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
            strTmp.erase(pos-1, std::string("</span>").length()+1);
        }
        printf("[BYOND] %s\n", strTmp);
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
        printf("[BYOND] %s\n", strTmp);
    }
        return oToHtml(This, a2);
}

void __fastcall hkCommandEvent(void* This, void* _EDX, unsigned char a, unsigned short b, unsigned char c)
{
    DWORD* dwCommandPtr;
    DWORD dwCommandLength;

    __asm
    {
        push eax
        mov eax, esi
        mov dwCommandPtr, eax
        mov eax, edx
        mov dwCommandLength, eax
        pop eax
    }

    if (dwCommandLength > 1000)
        return oCommandEvent(This, a, b, c);

    char* commandString = new char[dwCommandLength - 1];
    commandString = (char*)dwCommandPtr;

    if (true && commandString && commandString != NULL)
    {
        DWORD lastError = GetLastError();
        printf("((byond::commandevent)): ");
        std::cout << std::string(commandString) << std::endl;
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
    UINT spoofedVolume = GetPrivateProfileIntA("CID", "volume", 95345, "C:\\BYOND\\cid.ini");
    printf("[!] GVIA called! Spoofing CID to %i...\n", spoofedVolume);
    //printf("[!] Logging in as %s...\n", GetCurrentLoginKey());
    *(DWORD*)a4 = spoofedVolume;

    return TRUE;
}

/*void* __stdcall ComputeCid_hk(char* a1, size_t a2)
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
//} 

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
    printf("BYONDInstantHook supported version: 514.1583\n");
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

    if (MH_CreateHook(GetProcAddress(ByondCore, DungClientInitClient), hkInitClient, reinterpret_cast<LPVOID*>(&oInitClient)) != MH_OK)
    {
        std::cout << "Failed to hook DungClient::InitClient..." << std::endl;
        return;
    }
    printf("[+] InitClient hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientInitClient));

    if (MH_CreateHook(GetProcAddress(ByondCore, DMTextPrinterToHtml), hkToHtml, reinterpret_cast<LPVOID*>(&oToHtml)) != MH_OK)
    {
        std::cout << "Failed to hook DMTextPrinter::ToHtml..." << std::endl;
        return;
    }
    printf("[+] ToHtml hooked, address: 0x%p\n", GetProcAddress(ByondCore, DMTextPrinterToHtml));

    if (MH_CreateHook(GetProcAddress(ByondCore, DungClientCommandEvent), hkCommandEvent, reinterpret_cast<LPVOID*>(&oCommandEvent)) != MH_OK)
    {
        std::cout << "Failed to hook DungClient::CommandEvent..." << std::endl;
    }
    printf("[+] CommandEvent hooked, address: 0x%p\n", GetProcAddress(ByondCore, DungClientCommandEvent));

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

//  if (MH_CreateHook(pComputeCid, &ComputeCid_hk, reinterpret_cast<LPVOID*>(&ComputeCid_o)) != MH_OK)
//  {
 //     std::cout << "Failed to hook ComputeCID function..." << std::endl;
 // }
  //printf("[+] ComputeCid function hooked, address: 0x%p\n", pComputeCid);

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

