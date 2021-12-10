#include "includes.h"

int main(int argc, char* argv[])
{
    SetConsoleTitleW(L"DS Patcher v1.5");
    if (argc > 0) {
        PEFile pe(argv[1]);
        printf("%s\n", argv[1]);
        char* functions[] = { (char*)"Main" };
        pe.addImport((char*)"C:\\BYOND\\BYONDHook.dll", functions, 1);
        pe.addSection((char*)".dspatch", 0x1000, false);
        pe.saveToFile((char*)"dreamseeker.exe");
    }
    std::cout << "Patched!" << std::endl;
    return 0;
}