
#ifndef __PLAYER_SETTINGS_H_
#define __PLAYER_SETTINGS_H_

namespace player {

class PlayerSettings
{
public:
    PlayerSettings()
        : openLastProject(false)
        , offsetX(0)
        , offsetY(0)
    {}

    bool openLastProject;
    int offsetX;
    int offsetY;
};

} // namespace player

#endif // __PLAYER_SETTINGS_H_
