#include "main.h"
#include "AppDelegate.h"

using namespace cocos2d;

int WINAPI _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
