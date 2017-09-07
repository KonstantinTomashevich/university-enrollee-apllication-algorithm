#include <UEADB/Core/Application.hpp>
#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return UEADB::Application::Execute (GetCommandLineW ());
}

#elif defined(_WIN32)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    return UEADB::Application::Execute (GetCommandLineW ());
}

#else
int main(int argc, char** argv)
{
    return UEADB::Application::Execute (argc, argv);
}
#endif
