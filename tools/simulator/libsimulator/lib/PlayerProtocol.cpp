
#include "PlayerProtocol.h"
#include "base/ccMacros.h"

namespace player {

using namespace cocos2d;

PlayerProtocol *PlayerProtocol::_instance = nullptr;

PlayerProtocol::PlayerProtocol()
{
    CCASSERT(_instance == nullptr, "CAN NOT CREATE MORE PLAYER INSTANCE");
    _instance = this;
}

PlayerProtocol::~PlayerProtocol()
{
    _instance = nullptr;
}

PlayerProtocol *PlayerProtocol::getInstance()
{
    return _instance;
}

void PlayerProtocol::purgeInstance()
{
    if (_instance) delete _instance;
}

void PlayerProtocol::setPlayerSettings(const PlayerSettings &settings)
{
    _settings = settings;
}

PlayerSettings PlayerProtocol::getPlayerSettings() const
{
    return _settings;
}

} // namespace player
