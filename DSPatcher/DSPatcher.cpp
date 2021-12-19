#include "includes.h"

void patch(char* ds) {
    PEFile pe(ds);
    std::string strDsPath = ds;
    strDsPath += ".bak";
    std::filesystem::copy(ds, strDsPath);
    printf("Input file: %s\nPatching...\n", ds);
    char* functions[] = { (char*)"Main" };
    if (!std::filesystem::exists("C:\\BYOND\\BYONDHook.dll")) {
        printf("Error: C:\\BYOND\\BYONDHook.dll not found!\n");
        return;
    }
    pe.addImport((char*)"C:\\BYOND\\BYONDHook.dll", functions, 1);
    pe.addSection((char*)".dspatch", 0x1000, false);
    if (pe.saveToFile(ds))
    {
        printf("Patched!\nDelete backup dreamseeker.exe? (y/n)\n");
        char choice;
        std::cin >> choice;
        if (!(choice != 'Y' && choice != 'y')) {
            try {
                if (std::filesystem::remove(strDsPath))
                    printf("Done. Enjoy!\n");
                else
                    printf("Error removing dreamseeker.bak!\n");
            }
            catch (const std::filesystem::filesystem_error& err) {
                printf("Error: %s\n", err.what());
            }
        }
    }
    else
        printf("Error: couldn't save to file!\n");
    return;
}

int main(int argc, char* argv[])
{
    SetConsoleTitleW(L"DS Patcher");
    printf("DreamSeeker Patcher\n");
    if (argc > 1) {
        patch(argv[1]);
    }
    else
    {
        if (std::filesystem::exists("C:\\Program Files (x86)\\byond\\bin\\dreamseeker.exe")) {
            patch((char*)"C:\\Program Files (x86)\\byond\\bin\\dreamseeker.exe");
        }
        else
            printf("Abnormal BYOND installation. Please close this window and open it again by dragging dreamseeker.exe onto DSPatcher.exe");
    }
    system("pause");
    return 0;
}