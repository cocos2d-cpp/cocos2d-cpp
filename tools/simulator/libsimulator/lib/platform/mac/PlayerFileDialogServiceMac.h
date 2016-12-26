#ifndef __PLAY_FILE_DIALOG_SERVICE_MAC_H
#define __PLAY_FILE_DIALOG_SERVICE_MAC_H

#include "PlayerFileDialogServiceProtocol.h"

namespace player {

class PlayerFileDialogServiceMac : public PlayerFileDialogServiceProtocol
{
public:
    virtual std::string openFile(const std::string &title,
                                 const std::string &directory,
                                 const std::string &extensions) const;
    
    virtual std::vector<std::string> openMultiple(const std::string &title,
                                                  const std::string &directory,
                                                  const std::string &extensions) const;
    
    virtual std::string saveFile(const std::string &title,
                                 const std::string &path) const;
    
    virtual std::string openDirectory(const std::string &title,
                                      const std::string &directory)const;
};

} // namespace player

#endif // __PLAY_FILE_DIALOG_SERVICE_MAC_H
