
#ifndef __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_
#define __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_

#include "cocos2dx_extra.h"

namespace cocos2d {
namespace extra {

class HTTPRequest;

class HTTPRequestDelegate
{
public:
    virtual void requestFinished(HTTPRequest* request) {}
    virtual void requestFailed(HTTPRequest* request) {}
};

} // namespace extra
} // namespace cocos2d

#endif // __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_
