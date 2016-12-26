#include "../Classes/AppDelegate.h"
#include "cocos2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace cocos2d;

int main(int argc, char **argv)
{
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
