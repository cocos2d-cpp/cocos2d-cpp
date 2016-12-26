
#ifndef __PLAYER_MessageBoxServiceMac_h
#define __PLAYER_MessageBoxServiceMac_h

#include <vector>

#include "PlayerMessageBoxServiceProtocol.h"

namespace player {

class PlayerMessageBoxServiceMac : public PlayerMessageBoxServiceProtocol
{
public:
    virtual int showMessageBox(const std::string &title,
                               const std::string &message,
                               int buttonsType = BUTTONS_OK);
protected:
    struct MessageBoxInfo
    {
        std::string title;
        const int   buttonId;
    };
    std::vector<MessageBoxInfo> getTitles(int buttons);
};

} // namespace player

#endif // __PLAYER_MessageBoxServiceMac_h
