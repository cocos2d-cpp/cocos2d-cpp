//
// Bug-458 test case by nedrafehi
// http://code.google.com/p/cocos2d-iphone/issues/detail?id=458
//

#include "Bug-458.h"
#include "QuestionContainerSprite.h"

#include "2d/CCMenu.h"
#include "2d/CCMenuItem.h"
#include "base/CCDirector.h"

using namespace cocos2d;

bool Bug458Layer::init()
{
    if(BugsTestBase::init())
    {
        // ask director the the window size
        auto size = Director::getInstance()->getWinSize();

        auto question = to_node_ptr(new QuestionContainerSprite());
        auto question2 = to_node_ptr(new QuestionContainerSprite());
        
        auto sprite = MenuItemSprite::create(std::move(question2), std::move(question),
                                             CC_CALLBACK_1(Bug458Layer::selectAnswer, this) );

        auto sprite2 = MenuItemSprite::create(make_node_ptr<LayerColor>(Color4B(0,0,255,255), 100, 100),
                                              make_node_ptr<LayerColor>(Color4B(255,0,0,255), 100, 100),
                                              CC_CALLBACK_1(Bug458Layer::selectAnswer, this) );
        auto menu = make_node_ptr<Menu>(sprite, sprite2, nullptr);
        menu->alignItemsVerticallyWithPadding(100);
        menu->setPosition(size.width / 2, size.height / 2);

        // add the label as a child to this Layer
        addChild( std::move(menu) );

        return true;
    }
    return false;
}

void Bug458Layer::selectAnswer(Ref*)
{
    log("Selected");
}
