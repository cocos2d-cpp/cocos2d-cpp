#pragma once

#include <string>
#include "SimulatorExport.h"

namespace player {

class CC_LIBSIM_DLL DeviceEx
{
public:
    static DeviceEx *getInstance();

    std::string getCurrentUILangName();
    std::string getUserGUID();

private:
    DeviceEx();
    void init();
    void makeUILangName();
    std::string makeUserGUID();

    std::string _uiLangName;
    std::string _userGUID;
};

} // namespace player
