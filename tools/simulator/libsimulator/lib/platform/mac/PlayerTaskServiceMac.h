
#ifndef __PLAYER_TASK_SERVICE_MAC_H_
#define __PLAYER_TASK_SERVICE_MAC_H_

#include "PlayerTaskServiceProtocol.h"
#include "base/CCRef.h"

#include <unordered_map>
#include <sstream>

namespace player {
class PlayerTaskMac;
} // namespace player

@interface PlayerTaskPrivate : NSObject
{
    NSFileHandle *fileHandle;
    
    NSTask *_buildTask;
    BOOL _isRunning;
    int _exitCode;
    NSString *_output;
}

@property (assign) NSTask *buildTask;
@property (assign) BOOL isRunning;
@property (assign) int exitCode;
@property (retain) NSString *output;

- (void) runScriptAsyn:(NSString *)absScriptPath withArguments:(NSArray *) arguments;
@end

namespace player {

class PlayerTaskMac : public PlayerTask
{
public:
    static PlayerTaskMac *create(const std::string &name,
                                 const std::string &executePath,
                                 const std::string &commandLineArguments);
    
    virtual ~PlayerTaskMac();
    
    virtual bool run();
    virtual void stop();
    virtual void runInTerminal();

    // check task status
    virtual void update(float dt);
    
    void appendOutput(const char *data);
protected:
    PlayerTaskMac(const std::string &name,
                  const std::string &executePath,
                  const std::string &commandLineArguments);
    
    void cleanup();
    std::u16string makeCommandLine() const;
    
    PlayerTaskPrivate  *_taskPrivate;
};

class PlayerTaskServiceMac : public PlayerTaskServiceProtocol
{
public:
    PlayerTaskServiceMac();
    virtual ~PlayerTaskServiceMac();
    
    virtual PlayerTask *createTask(const std::string &name,
                                   const std::string &executePath,
                                   const std::string &commandLineArguments);
    virtual PlayerTask *getTask(const std::string &name);
    virtual void removeTask(const std::string &name);
    
protected:
    std::unordered_map<std::string, PlayerTaskMac*> _tasks;
};

} // namespace player


#endif // __PLAYER_TASK_SERVICE_MAC_H_
