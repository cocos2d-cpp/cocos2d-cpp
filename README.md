cocos2d-cpp
===========

cocos2d-cpp is a 2D game engine written in C++14. cocos2d-cpp was originally cloned from cocos2d-x game engine with an attempt to refactor some legacy features.

Since then, it turned into a complete re-write of the engine. Hence, though cocos2d-cpp is based on cocos2d-x, it is not compatible with it.

cocos2d-cpp is C++ and C++ only. It wouldn't be practical to attempt a re-write in such scale keeping lua and javascript bindings, so those bindings were erased.

cocos2d-cpp is targeted for Android, iOS, Linux, MacOS and Windows. As cocos2d-cpp is in active development now that includes re-write of essential components, the portability for some platforms might occasionally be broken. Linux is currently the main platform for development.

The main objectives of cocos2d-cpp:

1.  Make it really C++

    The original engine badly suffered from its objective-C origin. cocos2d-cpp is an uncompromised C++11 implementation. For example, cocos2d::Ref intrusive pointer - a base of all classes - is to be removed and substituted with C++11 standard smart pointers - std::unique_ptr, std::shared_ptr, etc. Objects created not via static create methods but via direct calls to public constructors.

2.  Make it clear, simple, and stable

    The aim for cocos2d to make the engine simple in use ironically resulted in an engine itself being unnecessarily complicated. Which in turn inevitably makes the engine vulnerable to bugs and hard to maintain. Moreover, it eventually leads to ambiguity and complexity in usage patterns. cocos2d-cpp is trying to follow the KISS principle.

3.  Increase performance

    The original codebase contains many suboptimal implementations that result in decreased performance of the engine. Many optimizations in the original codebase might have made sense a while ago, but these days some of them are nothing but insufficiencies. This issues are gradually addressed in development of cocos2d-cpp engine.

4.  Improve test coverage

    As cocos2d-cpp's units' interfaces become mature and stable, they are to be covered with unit and performance tests. For now, the engine is prepared to be unit-testable. For example, singletons are being removed.
