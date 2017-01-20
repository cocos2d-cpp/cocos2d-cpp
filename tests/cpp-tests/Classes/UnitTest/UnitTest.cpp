#include "UnitTest.h"
#include "ui/UIHelper.h"

using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#if defined (__arm64__)
#define USE_NEON64
#define INCLUDE_NEON64
#elif defined (__ARM_NEON__)
#define USE_NEON32
#define INCLUDE_NEON32
#else
#endif
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined (__arm64__) || defined (__aarch64__)
#define USE_NEON64
#define INCLUDE_NEON64
#elif defined (__ARM_NEON__)
#define INCLUDE_NEON32
#else
#endif
#else

#endif

#if defined (__SSE__)
#define USE_SSE
#define INCLUDE_SSE
#endif

#if (defined INCLUDE_NEON64) || (defined INCLUDE_NEON32) // FIXME: || (defined INCLUDE_SSE)
#define UNIT_TEST_FOR_OPTIMIZED_MATH_UTIL
#endif


// For ' < o > ' multiply test scene.

UnitTests::UnitTests()
{
    ADD_TEST_CASE(ValueTest);
    ADD_TEST_CASE(UTFConversionTest);
    ADD_TEST_CASE(UIHelperSubStringTest);
#ifdef UNIT_TEST_FOR_OPTIMIZED_MATH_UTIL
    ADD_TEST_CASE(MathUtilTest);
#endif
};

std::string UnitTestDemo::title() const
{
    return "UnitTest";
}

//----------------------------------

void ValueTest::onEnter()
{
    UnitTestDemo::onEnter();
    
    Value v1;
    CCASSERT(v1.getType() == Value::Type::NONE, "v1's value type should be VALUE::Type::NONE.");
    CCASSERT(v1.isNull(), "v1 is null.");
    
    Value v2(100);
    CCASSERT(v2.getType() == Value::Type::INTEGER, "v2's value type should be VALUE::Type::INTEGER.");
    CCASSERT(!v2.isNull(), "v2 is not null.");
    
    Value v3(101.4f);
    CCASSERT(v3.getType() == Value::Type::FLOAT, "v3's value type should be VALUE::Type::FLOAT.");
    CCASSERT(!v3.isNull(), "v3 is not null.");
    
    Value v4(106.1);
    CCASSERT(v4.getType() == Value::Type::DOUBLE, "v4's value type should be VALUE::Type::DOUBLE.");
    CCASSERT(!v4.isNull(), "v4 is not null.");
    
    unsigned char byte = 50;
    Value v5(byte);
    CCASSERT(v5.getType() == Value::Type::BYTE, "v5's value type should be Value::Type::BTYE.");
    CCASSERT(!v5.isNull(), "v5 is not null.");
    
    Value v6(true);
    CCASSERT(v6.getType() == Value::Type::BOOLEAN, "v6's value type is Value::Type::BOOLEAN.");
    CCASSERT(!v6.isNull(), "v6 is not null.");
    
    Value v7("string");
    CCASSERT(v7.getType() == Value::Type::STRING, "v7's value type is Value::type::STRING.");
    CCASSERT(!v7.isNull(), "v7 is not null.");
    
    Value v8(std::string("string2"));
    CCASSERT(v8.getType() == Value::Type::STRING, "v8's value type is Value::Type::STRING.");
    CCASSERT(!v8.isNull(), "v8 is not null.");

    auto createValueVector = [&](){
        ValueVector ret;
        ret.push_back(v1);
        ret.push_back(v2);
        ret.push_back(v3);
        return ret;
    };
    
    
    Value v9(createValueVector());
    CCASSERT(v9.getType() == Value::Type::VECTOR, "v9's value type is Value::Type::VECTOR.");
    CCASSERT(!v9.isNull(), "v9 is not null.");

    auto createValueMap = [&](){
        ValueMap ret;
        ret["aaa"] = v1;
        ret["bbb"] = v2;
        ret["ccc"] = v3;
        return ret;
    };
    
    Value v10(createValueMap());
    CCASSERT(v10.getType() == Value::Type::MAP, "v10's value type is Value::Type::MAP.");
    CCASSERT(!v10.isNull(), "v10 is not null.");
    
    auto createValueMapIntKey = [&](){
        ValueMapIntKey ret;
        ret[111] = v1;
        ret[222] = v2;
        ret[333] = v3;
        return ret;
    };
    
    Value v11(createValueMapIntKey());
    CCASSERT(v11.getType() == Value::Type::INT_KEY_MAP, "v11's value type is Value::Type::INT_KEY_MAP.");
    CCASSERT(!v11.isNull(), "v11 is not null.");
}

std::string ValueTest::subtitle() const
{
    return "Value Test, should not crash";
}

void ValueTest::constFunc(const Value& /*value*/) const
{
}

// UTFConversionTest

// FIXME: made as define to prevent compile warnings in release mode. Better is to be a `const static int`
#define TEST_CODE_NUM 11

static const char16_t __utf16Code[] =
{
    0x3042,
    0x3044,
    0x3046,
    0x3048,
    0x304A,
    0x3042,
    0x3044,
    0x3046,
    0x3048,
    0x304A,
    0x0041,
    0x0000,
};

// to avoid Xcode error, char => unsigned char
// If you use this table, please cast manually as (const char *).
static const unsigned char __utf8Code[] =
{
    0xE3,0x81,0x82,
    0xE3,0x81,0x84,
    0xE3,0x81,0x86,
    0xE3,0x81,0x88,
    0xE3,0x81,0x8A,
    0xE3,0x81,0x82,
    0xE3,0x81,0x84,
    0xE3,0x81,0x86,
    0xE3,0x81,0x88,
    0xE3,0x81,0x8A,
    0x41,
    0x00,
};


static const char16_t WHITE_SPACE_CODE[] =
{
    0x0009,
    0x000A,
    0x000B,
    0x000C,
    0x000D,
    0x0020,
    0x0085,
    0x00A0,
    0x1680,
    0x2000,
    0x2001,
    0x2002,
    0x2003,
    0x2004,
    0x2005,
    0x2006,
    0x2007,
    0x2008,
    0x2009,
    0x200A,
    0x2028,
    0x2029,
    0x202F,
    0x205F,
    0x3000
};

static void doUTFConversion()
{
    bool isSuccess = false;
    
    std::string originalUTF8 = (const char*)__utf8Code;
    std::u16string originalUTF16 = __utf16Code;
    
    //---------------------------
    std::string utf8Str;
    isSuccess = StringUtils::UTF16ToUTF8(originalUTF16, utf8Str);
    
    if (isSuccess)
    {
        isSuccess = memcmp(utf8Str.data(), originalUTF8.data(), originalUTF8.length()+1)==0;
    }
    
    CCASSERT(isSuccess, "StringUtils::UTF16ToUTF8 failed");
    
    //---------------------------
    std::u16string utf16Str;
    isSuccess = StringUtils::UTF8ToUTF16(originalUTF8, utf16Str);
    
    if (isSuccess)
    {
        isSuccess = memcmp(utf16Str.data(), originalUTF16.data(), originalUTF16.length()+1)==0;
    }
    
    CCASSERT(isSuccess && (utf16Str.length() == TEST_CODE_NUM), "StringUtils::UTF8ToUTF16 failed");
    
    //---------------------------
    auto vec1 = StringUtils::getChar16VectorFromUTF16String(originalUTF16);
    
    CCASSERT(vec1.size() == originalUTF16.length(), "StringUtils::getChar16VectorFromUTF16String failed");
    
    //---------------------------
    std::vector<char16_t> vec2( vec1 );
    vec2.push_back(0x2009);
    vec2.push_back(0x2009);
    vec2.push_back(0x2009);
    vec2.push_back(0x2009);
    
    std::vector<char16_t> vec3( vec2 );
    StringUtils::trimUTF16Vector(vec2);
    
    CCASSERT(vec1.size() == vec2.size(), "StringUtils::trimUTF16Vector failed");
    
    for (size_t i = 0; i < vec2.size(); i++ )
    {
        CCASSERT(vec1.at(i) == vec2.at(i), "StringUtils::trimUTF16Vector failed");
    }
    
    //---------------------------
    CCASSERT(StringUtils::getCharacterCountInUTF8String(originalUTF8) == TEST_CODE_NUM, "StringUtils::getCharacterCountInUTF8String failed");
    
    //---------------------------
    CCASSERT(StringUtils::getIndexOfLastNotChar16(vec3, 0x2009) == (vec1.size()-1), "StringUtils::getIndexOfLastNotChar16 failed");
    
    //---------------------------
    CCASSERT(originalUTF16.length() == TEST_CODE_NUM, "The length of the original utf16 string isn't equal to TEST_CODE_NUM");
    
    //---------------------------
    size_t whiteCodeNum = sizeof(WHITE_SPACE_CODE) / sizeof(WHITE_SPACE_CODE[0]);
    for( size_t i = 0; i < whiteCodeNum; i++ )
    {
        CCASSERT(StringUtils::isUnicodeSpace(WHITE_SPACE_CODE[i]), "StringUtils::isUnicodeSpace failed");
    }
    
    CCASSERT(!StringUtils::isUnicodeSpace(0xFFFF), "StringUtils::isUnicodeSpace failed");
    
    CCASSERT(!StringUtils::isCJKUnicode(0xFFFF) && StringUtils::isCJKUnicode(0x3100), "StringUtils::isCJKUnicode failed");
}

void UTFConversionTest::onEnter()
{
    UnitTestDemo::onEnter();

    for (int i = 0; i < 10000; ++i)
    {
        doUTFConversion();
    }
}

std::string UTFConversionTest::subtitle() const
{
    return "UTF8 <-> UTF16 Conversion Test, no crash";
}

// UIHelperSubStringTest

void UIHelperSubStringTest::onEnter()
{
    UnitTestDemo::onEnter();

    using cocos2d::ui::Helper;
    {
        // Trivial case
        std::string source = "abcdefghij";
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 2) == "ab");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 2, 2) == "cd");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 4, 2) == "ef");
    }
    {
        // Empty string
        std::string source = "";

        // OK
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 1) == "");

        // Error: These cases cause "out of range" error
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 1) == "");
    }
    {
        // Ascii
        std::string source = "abc";

        // OK
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 2, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 3, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 3) == "abc");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 4) == "abc");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 2) == "bc");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 3) == "bc");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 2, 1) == "c");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 2, 2) == "c");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 3, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 3, 2) == "");

        // Error: These cases cause "out of range" error
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 4, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 4, 1) == "");
    }
    {
        // CJK characters
        std::string source = "这里是中文测试例";

        // OK
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 7, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 8, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 8, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 1) == "\xe8\xbf\x99");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 4) == "\xe8\xbf\x99\xe9\x87\x8c\xe6\x98\xaf\xe4\xb8\xad");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 8) == "\xe8\xbf\x99\xe9\x87\x8c\xe6\x98\xaf\xe4\xb8\xad\xe6\x96\x87\xe6\xb5\x8b\xe8\xaf\x95\xe4\xbe\x8b");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 100) == "\xe8\xbf\x99\xe9\x87\x8c\xe6\x98\xaf\xe4\xb8\xad\xe6\x96\x87\xe6\xb5\x8b\xe8\xaf\x95\xe4\xbe\x8b");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 2, 5) == "\xe6\x98\xaf\xe4\xb8\xad\xe6\x96\x87\xe6\xb5\x8b\xe8\xaf\x95");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 6, 2) == "\xe8\xaf\x95\xe4\xbe\x8b");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 6, 100) == "\xe8\xaf\x95\xe4\xbe\x8b");

        // Error: These cases cause "out of range" error
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 9, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 9, 1) == "");
    }
    {
        // Redundant UTF-8 sequence for Directory traversal attack (1)
        std::string source = "\xC0\xAF";

        // Error: Can't convert string to correct encoding such as UTF-32
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 2) == "");
    }
    {
        // Redundant UTF-8 sequence for Directory traversal attack (2)
        std::string source = "\xE0\x80\xAF";

        // Error: Can't convert string to correct encoding such as UTF-32
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 3) == "");
    }
    {
        // Redundant UTF-8 sequence for Directory traversal attack (3)
        std::string source = "\xF0\x80\x80\xAF";

        // Error: Can't convert string to correct encoding such as UTF-32
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 0) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 1, 1) == "");
        CC_ASSERT(Helper::getSubStringOfUTF8String(source, 0, 4) == "");
    }
}

std::string UIHelperSubStringTest::subtitle() const
{
    return "ui::Helper::getSubStringOfUTF8String Test";
}

// MathUtilTest

namespace UnitTest {

#ifdef INCLUDE_NEON32
#include "math/MathUtilNeon.inl"
#endif

#ifdef INCLUDE_NEON64
#include "math/MathUtilNeon64.inl"
#endif

#ifdef INCLUDE_SSE
//FIXME: #include "math/MathUtilSSE.inl"
#endif

#include "math/MathUtil.inl"

} // namespace UnitTest {

// I know the next line looks ugly, but it's a way to test MathUtil. :)
using namespace UnitTest::cocos2d;

static void __checkMathUtilResult(const char* description, const float* a1, const float* a2, int size)
{
    log("-------------checking %s ----------------------------", description);
    // Check whether the result of the optimized instruction is the same as which is implemented in C
    for (int i = 0; i < size; ++i)
    {
        bool r = fabs(a1[i] - a2[i]) < 0.00001f;//FLT_EPSILON;
        if (r)
        {
            log("Correct: a1[%d]=%f, a2[%d]=%f", i, a1[i], i, a2[i]);
        }
        else
        {
            log("Wrong: a1[%d]=%f, a2[%d]=%f", i, a1[i], i, a2[i]);
        }
        CCASSERT(r, "The optimized instruction is implemented in a wrong way, please check it!");
    }
}

void MathUtilTest::onEnter()
{
    UnitTestDemo::onEnter();
    
    const int MAT4_SIZE = 16;
    const int VEC4_SIZE = 4;
    
    const float inMat41[MAT4_SIZE] = {
        0.234023f, 2.472349f, 1.984244f, 2.23348f,
        0.634124f, 0.234975f, 6.384572f, 0.82368f,
        0.738028f, 1.845237f, 1.934721f, 1.62343f,
        0.339023f, 3.472452f, 1.324714f, 4.23852f,
    };
    
    const float inMat42[MAT4_SIZE] = {
        1.640232f, 4.472349f, 0.983244f, 1.23343f,
        2.834124f, 8.234975f, 0.082572f, 3.82464f,
        3.238028f, 2.845237f, 0.331721f, 4.62544f,
        4.539023f, 9.472452f, 3.520714f, 2.23252f,
    };
    
    const float scalar = 1.323298f;
    const float x = 0.432234f;
    const float y = 1.333229f;
    const float z = 2.535292f;
    const float w = 4.632234f;
    
    const float inVec4[VEC4_SIZE] = {2.323478f, 0.238482f, 4.223783f, 7.238238f};
    const float inVec42[VEC4_SIZE] = {0.322374f, 8.258883f, 3.293683f, 2.838337f};
    
    float outMat4Opt[MAT4_SIZE] = {0};
    float outMat4C[MAT4_SIZE] = {0};
    float outVec4Opt[VEC4_SIZE] = {0};
    float outVec4C[VEC4_SIZE] = {0};
    
    // inline static void addMatrix(const float* m, float scalar, float* dst);
    MathUtilC::addMatrix(inMat41, scalar, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::addMatrix(inMat41, scalar, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::addMatrix(inMat41, scalar, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
// FIXME:
#endif
    
    __checkMathUtilResult("inline static void addMatrix(const float* m, float scalar, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));
    
    // inline static void addMatrix(const float* m1, const float* m2, float* dst);
    MathUtilC::addMatrix(inMat41, inMat42, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::addMatrix(inMat41, inMat42, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::addMatrix(inMat41, inMat42, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void addMatrix(const float* m1, const float* m2, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));

    // inline static void subtractMatrix(const float* m1, const float* m2, float* dst);
    MathUtilC::subtractMatrix(inMat41, inMat42, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::subtractMatrix(inMat41, inMat42, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::subtractMatrix(inMat41, inMat42, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void subtractMatrix(const float* m1, const float* m2, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));
    
    // inline static void multiplyMatrix(const float* m, float scalar, float* dst);
    MathUtilC::multiplyMatrix(inMat41, scalar, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::multiplyMatrix(inMat41, scalar, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::multiplyMatrix(inMat41, scalar, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void multiplyMatrix(const float* m, float scalar, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));
    
    // inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);
    MathUtilC::multiplyMatrix(inMat41, inMat42, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::multiplyMatrix(inMat41, inMat42, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::multiplyMatrix(inMat41, inMat42, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));
    
    // inline static void negateMatrix(const float* m, float* dst);
    MathUtilC::negateMatrix(inMat41, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::negateMatrix(inMat41, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::negateMatrix(inMat41, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void negateMatrix(const float* m, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));
    
    // inline static void transposeMatrix(const float* m, float* dst);
    MathUtilC::transposeMatrix(inMat41, outMat4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::transposeMatrix(inMat41, outMat4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::transposeMatrix(inMat41, outMat4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void transposeMatrix(const float* m, float* dst);", outMat4C, outMat4Opt, MAT4_SIZE);
    // Clean
    memset(outMat4C, 0, sizeof(outMat4C));
    memset(outMat4Opt, 0, sizeof(outMat4Opt));
    
    // inline static void transformVec4(const float* m, float x, float y, float z, float w, float* dst);
    MathUtilC::transformVec4(inMat41, x, y, z, w, outVec4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::transformVec4(inMat41, x, y, z, w, outVec4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::transformVec4(inMat41, x, y, z, w, outVec4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void transformVec4(const float* m, float x, float y, float z, float w, float* dst);", outVec4C, outVec4Opt, VEC4_SIZE);
    // Clean
    memset(outVec4C, 0, sizeof(outVec4C));
    memset(outVec4Opt, 0, sizeof(outVec4Opt));
    
    // inline static void transformVec4(const float* m, const float* v, float* dst);
    MathUtilC::transformVec4(inMat41, inVec4, outVec4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::transformVec4(inMat41, inVec4, outVec4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::transformVec4(inMat41, inVec4, outVec4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void transformVec4(const float* m, const float* v, float* dst);", outVec4C, outVec4Opt, VEC4_SIZE);
    // Clean
    memset(outVec4C, 0, sizeof(outVec4C));
    memset(outVec4Opt, 0, sizeof(outVec4Opt));
    
    // inline static void crossVec3(const float* v1, const float* v2, float* dst);
    MathUtilC::crossVec3(inVec4, inVec42, outVec4C);
    
#ifdef INCLUDE_NEON32
    MathUtilNeon::crossVec3(inVec4, inVec42, outVec4Opt);
#endif
    
#ifdef INCLUDE_NEON64
    MathUtilNeon64::crossVec3(inVec4, inVec42, outVec4Opt);
#endif
    
#ifdef INCLUDE_SSE
    // FIXME:
#endif
    
    __checkMathUtilResult("inline static void crossVec3(const float* v1, const float* v2, float* dst);", outVec4C, outVec4Opt, VEC4_SIZE);
    // Clean
    memset(outVec4C, 0, sizeof(outVec4C));
    memset(outVec4Opt, 0, sizeof(outVec4Opt));
}

std::string MathUtilTest::subtitle() const
{
    return "MathUtilTest";
}
