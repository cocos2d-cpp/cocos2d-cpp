#ifndef __BUG_CCDRAWNODE_H__
#define __BUG_CCDRAWNODE_H__

#include "BugsTest.h"

class BugDrawNodeLayer : public BugsTestBase
{
public:
    static BugDrawNodeLayer* create()
    {
        auto ret = new BugDrawNodeLayer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

#endif
