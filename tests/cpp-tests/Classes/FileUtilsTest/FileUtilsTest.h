#ifndef __FILEUTILSTEST_H__
#define __FILEUTILSTEST_H__

#include "../BaseTest.h"


DEFINE_TEST_SUITE(FileUtilsTests);

class FileUtilsDemo : public TestCase
{
public:
};

class TestResolutionDirectories : public FileUtilsDemo
{
public:
    static TestResolutionDirectories* create()
    {
        auto ret = new TestResolutionDirectories;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    std::vector<std::string> _defaultSearchPathArray;
    std::vector<std::string> _defaultResolutionsOrderArray;
};

class TestSearchPath : public FileUtilsDemo
{
public:
    static TestSearchPath* create()
    {
        auto ret = new TestSearchPath;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    std::vector<std::string> _defaultSearchPathArray;
    std::vector<std::string> _defaultResolutionsOrderArray;
};

class TestFilenameLookup : public FileUtilsDemo
{
public:
    static TestFilenameLookup* create()
    {
        auto ret = new TestFilenameLookup;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
};

class TestIsFileExist : public FileUtilsDemo
{
public:
    static TestIsFileExist* create()
    {
        auto ret = new TestIsFileExist;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestIsDirectoryExist : public FileUtilsDemo
{
public:
    static TestIsDirectoryExist* create()
    {
        auto ret = new TestIsDirectoryExist;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestFileFuncs : public FileUtilsDemo
{
public:
    static TestFileFuncs* create()
    {
        auto ret = new TestFileFuncs;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestDirectoryFuncs : public FileUtilsDemo
{
public:
    static TestDirectoryFuncs* create()
    {
        auto ret = new TestDirectoryFuncs;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestWriteString : public FileUtilsDemo
{
public:
    static TestWriteString* create()
    {
        auto ret = new TestWriteString;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestGetContents : public FileUtilsDemo
{
public:
    static TestGetContents* create()
    {
        auto ret = new TestGetContents;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    std::string _generatedFile;
};

class TestWriteData : public FileUtilsDemo
{
public:
    static TestWriteData* create()
    {
        auto ret = new TestWriteData;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestWriteValueMap : public FileUtilsDemo
{
public:
    static TestWriteValueMap* create()
    {
        auto ret = new TestWriteValueMap;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestWriteValueVector : public FileUtilsDemo
{
public:
    static TestWriteValueVector* create()
    {
        auto ret = new TestWriteValueVector;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TestUnicodePath : public FileUtilsDemo
{
public:
    static TestUnicodePath* create()
    {
        auto ret = new TestUnicodePath;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif /* __FILEUTILSTEST_H__ */
