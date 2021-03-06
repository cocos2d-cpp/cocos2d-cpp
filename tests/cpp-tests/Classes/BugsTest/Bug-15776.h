//
//  Bug-15776.hpp
//  cocos2d_tests
//
//  Created by Ricardo Quesada on 6/17/16.
//
//

#ifndef Bug_15776_hpp
#define Bug_15776_hpp

#include "BugsTest.h"

class Bug15776Layer : public BugsTestBase
{
public:
    static Bug15776Layer* create()
    {
        auto ret = new Bug15776Layer;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


#endif /* Bug_15776_hpp */
