/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017      Iakov Sergeev <yahont@github>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "2d/CCAnimationCache.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

using namespace std;

namespace cocos2d {

AnimationCache* AnimationCache::s_sharedAnimationCache = nullptr;

AnimationCache* AnimationCache::getInstance()
{
    if (! s_sharedAnimationCache)
    {
        s_sharedAnimationCache = new (std::nothrow) AnimationCache();
        s_sharedAnimationCache->init();
    }

    return s_sharedAnimationCache;
}

void AnimationCache::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(s_sharedAnimationCache);
}

bool AnimationCache::init()
{
    return true;
}

AnimationCache::AnimationCache()
{
}

AnimationCache::~AnimationCache()
{
    CCLOGINFO("deallocing AnimationCache: %p", this);
}

void AnimationCache::addAnimation(std::unique_ptr<Animation> animation, const std::string& name)
{
    _animations[name] = std::move(animation);
}

std::unique_ptr<Animation> AnimationCache::extractAnimation(const std::string& name)
{
    std::unique_ptr<Animation> ret;

    auto it = _animations.find(name);
    if (it != _animations.end())
    {
        ret = std::move(it->second);
        _animations.erase(it);
    }
    return ret;
}

std::unique_ptr<Animation> AnimationCache::cloneAnimation(const std::string& name)
{
    std::unique_ptr<Animation> ret;

    auto it = _animations.find(name);
    if (it != _animations.end())
    {
        ret = it->second->clone();
    }

    return ret;
}

AnimationFrame* AnimationCache::getAnimationFrame(const std::string& animationName, size_t frameIndex)
{
    auto it = _animations.find(animationName);
    if (it == _animations.end())
    {
        throw std::runtime_error("AnimationCache: " + animationName + ": Not found");
    }
    return it->second->getFrames().at(frameIndex).get();
}

 void AnimationCache::parseVersion1(const ValueMap& animations)
{
    SpriteFrameCache & frameCache = Director::getInstance()->getSpriteFrameCache();

    for (const auto& anim : animations)
    {
        const ValueMap& animationDict = anim.second.asValueMap();
        const ValueVector& frameNames = animationDict.at("frames").asValueVector();
        float delay = animationDict.at("delay").asFloat();

        if ( frameNames.empty() )
        {
            CCLOG("cocos2d: AnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", anim.first.c_str());
            continue;
        }

        size_t frameNameSize = frameNames.size();
        std::vector<retaining_ptr<AnimationFrame>> frames;
        frames.reserve(frameNameSize);

        for (auto& frameName : frameNames)
        {
            SpriteFrame* spriteFrame = frameCache.getSpriteFrameByName(frameName.asString());

            if ( ! spriteFrame ) {
                CCLOG("cocos2d: AnimationCache: Animation '%s' refers to frame '%s' which is not currently in the SpriteFrameCache. This frame will not be added to the animation.", anim.first.c_str(), frameName.asString().c_str());

                continue;
            }

            AnimationFrame* animFrame = AnimationFrame::create(spriteFrame, 1, ValueMap());
            frames.push_back(to_retaining_ptr(animFrame));
        }

        if ( frames.empty() )
        {
            CCLOG("cocos2d: AnimationCache: None of the frames for animation '%s' were found in the SpriteFrameCache. Animation is not being added to the Animation Cache.", anim.first.c_str());
            continue;
        }
        else if ( frames.size() != frameNameSize )
        {
            CCLOG("cocos2d: AnimationCache: An animation in your dictionary refers to a frame which is not in the SpriteFrameCache. Some or all of the frames for the animation '%s' may be missing.", anim.first.c_str());
        }

        AnimationCache::getInstance()->addAnimation(
            std::unique_ptr<Animation>(new Animation(std::move(frames), delay, 1)),
            anim.first
        );
    }
}

void AnimationCache::parseVersion2(const ValueMap& animations)
{
    SpriteFrameCache & frameCache = Director::getInstance()->getSpriteFrameCache();

    for (const auto& anim : animations)
    {
        std::string name = anim.first;
        ValueMap& animationDict = const_cast<ValueMap&>(anim.second.asValueMap());

        const Value& loops = animationDict["loops"];
        bool restoreOriginalFrame = animationDict["restoreOriginalFrame"].asBool();

        ValueVector& frameArray = animationDict["frames"].asValueVector();

        if ( frameArray.empty() )
        {
            CCLOG("cocos2d: AnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", name.c_str());
            continue;
        }

        // Array of AnimationFrames
        std::vector<retaining_ptr<AnimationFrame>> array;
        array.reserve(frameArray.size());

        for (auto& obj : frameArray)
        {
            ValueMap& entry = obj.asValueMap();
            std::string spriteFrameName = entry["spriteframe"].asString();
            SpriteFrame *spriteFrame = frameCache.getSpriteFrameByName(spriteFrameName);

            if( ! spriteFrame ) {
                CCLOG("cocos2d: AnimationCache: Animation '%s' refers to frame '%s' which is not currently in the SpriteFrameCache. This frame will not be added to the animation.", name.c_str(), spriteFrameName.c_str());

                continue;
            }

            float delayUnits = entry["delayUnits"].asFloat();
            Value& userInfo = entry["notification"];

            AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, delayUnits, userInfo.getType() == Value::Type::MAP ? userInfo.asValueMap() : ValueMap());

            array.push_back(to_retaining_ptr(animFrame));
        }

        float delayPerUnit = animationDict["delayPerUnit"].asFloat();

        std::unique_ptr<Animation> animation(
            new Animation(
                std::move(array),
                delayPerUnit,
                loops.getType() != Value::Type::NONE ? loops.asInt() : 1
            ));

        animation->setRestoreOriginalFrame(restoreOriginalFrame);

        AnimationCache::getInstance()->addAnimation(std::move(animation), name);
    }
}

void AnimationCache::addAnimationsWithDictionary(const ValueMap& dictionary,const std::string& plist)
{
    if ( dictionary.find("animations") == dictionary.end() )
    {
        CCLOG("cocos2d: AnimationCache: No animations were found in provided dictionary.");
        return;
    }
    
    const Value& animations = dictionary.at("animations");
    unsigned int version = 1;

    if( dictionary.find("properties") != dictionary.end() )
    {
        const ValueMap& properties = dictionary.at("properties").asValueMap();
        version = properties.at("format").asInt();
        const ValueVector& spritesheets = properties.at("spritesheets").asValueVector();

        SpriteFrameCache & frameCache = Director::getInstance()->getSpriteFrameCache();

        for(const auto &value : spritesheets) {
            std::string path = FileUtils::getInstance()->fullPathFromRelativeFile(value.asString(),plist);
            frameCache.addSpriteFramesWithFile(path);
        }
    }

    switch (version) {
        case 1:
            parseVersion1(animations.asValueMap());
            break;
        case 2:
            parseVersion2(animations.asValueMap());
            break;
        default:
            CCASSERT(false, "Invalid animation format");
    }
}

/** Read an NSDictionary from a plist file and parse it automatically for animations */
void AnimationCache::addAnimationsWithFile(const std::string& plist)
{
    CCASSERT(!plist.empty(), "Invalid texture file name");
    if (plist.empty()) {
        log("%s error:file name is empty!", __FUNCTION__);
        return;
    }
    
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(plist);

    CCASSERT( !dict.empty(), "CCAnimationCache: File could not be found");
    if (dict.empty()) {
        log("AnimationCache::addAnimationsWithFile error:%s not exist!", plist.c_str());
    }

    addAnimationsWithDictionary(dict,plist);
}


} // namespace cocos2d
