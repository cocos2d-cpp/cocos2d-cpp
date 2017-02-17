cocos2d-cpp
===========

cocos2d-cpp is a 2D game engine written in C++11. cocos2d-cpp was originally cloned from cocos2d-x game engine with an attempt to refactor some legacy features.

Since then, it turned into a complete re-write of the engine. Hence, though cocos2d-cpp is based on cocos2d-x, it is not compatible with it.

cocos2d-cpp is C++11 and C++11 only. It wouldn't be practical to attempt a re-write in such scale keeping lua and javascript bindings, so those bindings were erased.

cocos2d-cpp is targeted for Android, iOS, MacOS and Linux. Portability with Windows is being kept, but it might occasionally be broken.

The main objectives of cocos2d-cpp:

1.  Clean memory model

    cocos2d::Ref intrusive pointer - a base of all classes - is to be removed and substituted with C++11 standard smart pointers - std::unique_ptr, std::shared_ptr, etc.

2.  Make it really C++11

    The original engine badly suffered from its objective-C origin. cocos2d-cpp is an uncompromised C++11 implementation.

3.  Make it clear and simple

    The aim for cocos2d to make the engine simple in use ironically resulted in an engine itself being unnecessarily complicated. Which in turn inevitably makes the engine vulnerable to bugs and hard to maintain. Moreover, it eventually leads to ambiguity and complexity in usage patterns. cocos2d-cpp is trying to follow the KISS principle.

4.  Increase performance

    The original codebase contains many suboptimal implementations that result in decreased performance of the engine. Many optimizations in the original codebase might have made sense a while ago, but these days some of them are nothing but insufficiencies. This issues are gradually addressed in development of cocos2d-cpp engine.

5.  Testability

    As cocos2d-cpp's units' interfaces become mature and stable, they are to be covered with unit and performance tests. For now, the engine is prepared to be unit-testable. For example, singletons are being removed.
