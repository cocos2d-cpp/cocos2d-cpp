#include "DrawPrimitivesTest.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"

using namespace cocos2d;

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif

using namespace std;

DrawNodeTests::DrawNodeTests()
{
    ADD_TEST_CASE(SimpleDrawNodeTest);
    ADD_TEST_CASE(DrawNodeTest);
    ADD_TEST_CASE(PrimitivesCommandTest);
    ADD_TEST_CASE(Issue11942Test);
}

string DrawNodeBaseTest::title() const
{
    return "No title";
}

// SimpleDrawNodeTest

SimpleDrawNodeTest::SimpleDrawNodeTest()
{
}

void SimpleDrawNodeTest::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(SimpleDrawNodeTest::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void SimpleDrawNodeTest::onDraw(const Mat4 &transform, uint32_t flags)
{
    Director* director = Director::getInstance();

    auto s = director->getWinSize();
    
    auto draw = DrawNode::create();

    addChild(draw, 10);

    // draw a line
    draw->setLineWidth(1.0f);
    draw->drawLine(VisibleRect::leftBottom(),
                   VisibleRect::rightTop(),
                   Color4F(1.0f, 1.0f, 1.0f, 1.0f));

    // Different length of the line requires another DrawNode
    auto draw1 = DrawNode::create();
    addChild(draw1, 10);
    draw1->setLineWidth(5.0f);
    draw1->drawLine(VisibleRect::leftTop(),
                    VisibleRect::rightBottom(),
                    Color4F(1.0f, 0.0f, 0.0f, 1.0f));
    
    // TIP:
    // If you are going to use always thde same color or width, you don't
    // need to call it before every draw
    //
    // Remember: OpenGL is a state-machine.
    
    // draw big point in the center (i.e. a square)
    // FIXME: opacity doesn't seem to work
    draw->drawPoint(VisibleRect::center(), 128.0f, Color4F(0, 0, 1, 0.5));
    
    std::vector<Vec2> points = { Vec2(60,60), Vec2(70,70), Vec2(60,70), Vec2(70,60) };
    draw->drawPoints(points.data(), static_cast<unsigned int>(points.size()), 8.0f, Color4F(0, 1.0f, 1.0f, 1.0f));

    auto draw2 = DrawNode::create();
    addChild(draw2, 10);
    draw2->setLineWidth(16.0f);
    draw2->drawCircle(VisibleRect::center(),
                      100.0f, // radius
                      0.0f, // angle
                      10, // segments
                      false, // drawLineToCenter
                      Color4F(0.0f, 1.0f, 0.0f, 1.0f));

    // draw a green circle with 50 segments with line to center
    auto draw3 = DrawNode::create();
    addChild(draw3, 10);
    draw3->setLineWidth(2);
    draw3->drawCircle(VisibleRect::center(),
                      50,
                      CC_DEGREES_TO_RADIANS(90),
                      50,
                      true,
                      Color4F(0.0f, 1.0f, 1.0f, 1.0f));
 
    // draw a pink solid circle with 50 segments
    draw3->drawSolidCircle(VisibleRect::center() + Vec2(140, 0),
                           40,
                           CC_DEGREES_TO_RADIANS(90),
                           50,
                           1.0f, // scaleX
                           1.0f, // scaleY
                           Color4F(1.0f, 0.0f, 1.0f, 1.0f));
 
    // open yellow poly
    std::vector<Vec2> vertices = {
        Vec2(0,0),    Vec2(50,50),
        Vec2(100,50), Vec2(100,100),
        Vec2(50,100)
    };
    auto draw4 = DrawNode::create();
    addChild(draw4, 10);
    draw4->setLineWidth(10);
    draw4->drawPoly(vertices.data(),
                    static_cast<unsigned int>(vertices.size()),
                    false,
                    Color4F(1.0f, 1.0f, 0.0f, 1.0f));

    // filled poly
    std::vector<Vec2> filledVertices = {
        Vec2(0,120), Vec2(50,120),
        Vec2(50,170), Vec2(25,200),
        Vec2(0,170)
    };
    draw->drawSolidPoly(filledVertices.data(),
                        static_cast<unsigned int>(filledVertices.size()),
                        Color4F(0.5f, 0.5f, 1.0f, 1.0f));

    // closed purble poly
    std::vector<Vec2> vertices2 = { Vec2(30,130), Vec2(30,230), Vec2(50,200) };
    draw3->drawPoly(vertices2.data(), static_cast<unsigned int>(vertices2.size()), true, Color4F(1.0f, 0, 1.0f, 1.0f));
 
    // draw quad bezier path
    draw3->drawQuadBezier(VisibleRect::leftTop(),
                          VisibleRect::center(),
                          VisibleRect::rightTop(),
                          50,
                          Color4F(1.0f, 0, 1.0f, 1.0f));

    // draw cubic bezier path
    draw3->drawCubicBezier(VisibleRect::center(),
                           Vec2(VisibleRect::center().x + 30,
                                VisibleRect::center().y + 50),
                           Vec2(VisibleRect::center().x + 60,
                                VisibleRect::center().y - 50),
                           VisibleRect::right(),
                           100,
                           Color4F(1.0f, 0, 1.0f, 1.0f));

    //draw a solid polygon
    std::vector<Vec2> vertices3 = {Vec2(60,160), Vec2(70,190), Vec2(100,190), Vec2(90,160)};
    draw3->drawSolidPoly(vertices3.data(),
                         static_cast<unsigned int>(vertices3.size()),
                         Color4F(1, 1, 0, 1));
}

string SimpleDrawNodeTest::title() const
{
    return "DrawNode simple";
}

string SimpleDrawNodeTest::subtitle() const
{
    return "Color.a doesn't work for the large center point";
}

// DrawNodeTest
DrawNodeTest::DrawNodeTest()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto draw = DrawNode::create();
    addChild(draw, 10);
    
    draw->drawPoint(Vec2(s.width/2-120, s.height/2-120), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    draw->drawPoint(Vec2(s.width/2+120, s.height/2+120), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    // draw 4 small points
    Vec2 position[] = { Vec2(60,60), Vec2(70,70), Vec2(60,70), Vec2(70,60) };
    draw->drawPoints( position, 4, 5, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
 
    // draw a line
    draw->drawLine(Vec2(0,0), Vec2(s.width, s.height), Color4F(1.0, 0.0, 0.0, 0.5));
    
    // draw a rectangle
    draw->drawRect(Vec2(23,23), Vec2(7,7), Color4F(1,1,0,1));
    
    draw->drawRect(Vec2(15,30), Vec2(30,15), Vec2(15,0), Vec2(0,15), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    // draw a circle
    draw->drawCircle(VisibleRect::center() + Vec2(140,0), 100, CC_DEGREES_TO_RADIANS(90), 50, true, 1.0f, 2.0f, Color4F(1.0, 0.0, 0.0, 0.5));
    
    draw->drawCircle(VisibleRect::center() - Vec2(140,0), 50, CC_DEGREES_TO_RADIANS(90), 30, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    // Draw some beziers
    draw->drawQuadBezier(Vec2(s.width - 150, s.height - 150), Vec2(s.width - 70, s.height - 10), Vec2(s.width - 10, s.height - 10), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    draw->drawQuadBezier(Vec2(0, s.height), Vec2(s.width/2, s.height/2), Vec2(s.width, s.height), 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    draw->drawCubicBezier(VisibleRect::center(), Vec2(VisibleRect::center().x+30,VisibleRect::center().y+50), Vec2(VisibleRect::center().x+60,VisibleRect::center().y-50),VisibleRect::right(),100, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    draw->drawCubicBezier(Vec2(s.width - 250, 40), Vec2(s.width - 70, 100), Vec2(s.width - 30, 250), Vec2(s.width - 10, s.height - 50), 10, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    auto array = PointArray::create(20);
    array->addControlPoint(Vec2(0,0));
    array->addControlPoint(Vec2(80,80));
    array->addControlPoint(Vec2(s.width-80,80));
    array->addControlPoint(Vec2(s.width-80,s.height-80));
    array->addControlPoint(Vec2(80,s.height-80));
    array->addControlPoint(Vec2(80,80));
    array->addControlPoint(Vec2(s.width/2, s.height/2));
    draw->drawCardinalSpline(array, 0.5, 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    auto array2 = PointArray::create(20);
    array2->addControlPoint(Vec2(s.width / 2, 30));
    array2->addControlPoint(Vec2(s.width  -80, 30));
    array2->addControlPoint(Vec2(s.width - 80, s.height - 80));
    array2->addControlPoint(Vec2(s.width / 2, s.height - 80));
    array2->addControlPoint(Vec2(s.width / 2, 30));
    draw->drawCatmullRom(array2, 50, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    // open random color poly
    Vec2 vertices[] = { Vec2(0,0), Vec2(50,50), Vec2(100,50), Vec2(100,100), Vec2(50,100) };
    draw->drawPoly( vertices, 5, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    // closed random color poly
    Vec2 vertices2[] = { Vec2(30,130), Vec2(30,230), Vec2(50,200) };
    draw->drawPoly( vertices2, 3, true, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    // Draw 10 circles
    for( int i=0; i < 10; i++)
    {
        draw->drawDot(Vec2(s.width/2, s.height/2), 10*(10-i), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    }
    
    // Draw polygons
    Vec2 points[] = { Vec2(s.height/4,0), Vec2(s.width,s.height/5), Vec2(s.width/3*2,s.height) };
    draw->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(1,0,0,0.5), 4, Color4F(0,0,1,0.5));
    
    // star poly (triggers buggs)
    {
        const float o=80;
        const float w=20;
        const float h=50;
        Vec2 star[] = {
            Vec2(o+w,o-h), Vec2(o+w*2, o),                        // lower spike
            Vec2(o + w*2 + h, o+w ), Vec2(o + w*2, o+w*2),        // right spike
            //              {o +w, o+w*2+h}, {o,o+w*2},                 // top spike
            //              {o -h, o+w}, {o,o},                         // left spike
        };
        
        draw->drawPolygon(star, sizeof(star)/sizeof(star[0]), Color4F(1,0,0,0.5), 1, Color4F(0,0,1,1));
    }
    
    // star poly (doesn't trigger bug... order is important un tesselation is supported.
    {
        const float o=180;
        const float w=20;
        const float h=50;
        Vec2 star[] = {
            Vec2(o,o), Vec2(o+w,o-h), Vec2(o+w*2, o),        // lower spike
            Vec2(o + w*2 + h, o+w ), Vec2(o + w*2, o+w*2),    // right spike
            Vec2(o +w, o+w*2+h), Vec2(o,o+w*2),               // top spike
            Vec2(o -h, o+w),                                     // left spike
        };
        
        draw->drawPolygon(star, sizeof(star)/sizeof(star[0]), Color4F(1,0,0,0.5), 1, Color4F(0,0,1,1));
    }
    
    //draw a solid polygon
    Vec2 vertices3[] = {Vec2(60,160), Vec2(70,190), Vec2(100,190), Vec2(90,160)};
    draw->drawSolidPoly( vertices3, 4, Color4F(1,1,0,1) );
    
    //draw a solid rectangle
    draw->drawSolidRect(Vec2(10,10), Vec2(20,20), Color4F(1,1,0,1));
    
    //draw a solid circle
    draw->drawSolidCircle( VisibleRect::center() + Vec2(140,0), 40, CC_DEGREES_TO_RADIANS(90), 50, 2.0f, 2.0f, Color4F(0.0, 1.0, 0.0, 1.0));
    
    // Draw segment
    draw->drawSegment(Vec2(20,s.height), Vec2(20,s.height/2), 10, Color4F(0, 1, 0, 1));
    
    draw->drawSegment(Vec2(10,s.height/2), Vec2(s.width/2, s.height/2), 40, Color4F(1, 0, 1, 0.5));

	// Draw triangle
    draw->drawTriangle(Vec2(10, 10), Vec2(70, 30), Vec2(100, 140), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 0.5));
    
    for (int i = 0; i < 100; i++) {
        draw->drawPoint(Vec2(i*7, 5), (float)i/5+1, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    }

    auto draw1 = DrawNode::create();
    this->addChild(draw1, 10);
    draw1->setLineWidth(4);
    draw1->drawLine(Vec2(0, s.height), Vec2(s.width, s.height - 20), Color4F::YELLOW);
    draw1->drawLine(Vec2(0, 0), Vec2(s.width, s.height - 20), Color4F::YELLOW);
}

string DrawNodeTest::title() const
{
    return "Test DrawNode";
}

string DrawNodeTest::subtitle() const
{
    return "Testing DrawNode - batched draws. Concave polygons are BROKEN";
}

// PrimitivesCommandTest
PrimitivesCommandTest::PrimitivesCommandTest()
{
    // draws a quad
    V3F_C4B_T2F data[] = {
        {{0,    0,0}, {255,  0,  0,255}, {0,1}},
        {{200,  0,0}, {0,  255,255,255}, {1,1}},
        {{200,200,0}, {255,255,  0,255}, {1,0}},
        {{0,  200,0}, {255,255,255,255}, {0,0}},
    };

    uint16_t indices[] = {
        0,1,2,
        2,0,3
    };

    static const int TOTAL_VERTS = sizeof(data) / sizeof(data[0]);
    static const int TOTAL_INDICES = TOTAL_VERTS*6/4;

    auto vertexBuffer = VertexBuffer::create(sizeof(V3F_C4B_T2F), TOTAL_VERTS);
    vertexBuffer->updateVertices(data, TOTAL_VERTS,0);

    auto vertsData = VertexData::create();
    vertsData->setStream(vertexBuffer, VertexStreamAttribute(0, GLProgram::VERTEX_ATTRIB_POSITION, GL_FLOAT, 3));
    vertsData->setStream(vertexBuffer, VertexStreamAttribute(offsetof(V3F_C4B_T2F, colors), GLProgram::VERTEX_ATTRIB_COLOR, GL_UNSIGNED_BYTE, 4, true));
    vertsData->setStream(vertexBuffer, VertexStreamAttribute(offsetof(V3F_C4B_T2F, texCoords), GLProgram::VERTEX_ATTRIB_TEX_COORD, GL_FLOAT, 2));


    auto indexBuffer = IndexBuffer::create(IndexBuffer::IndexType::INDEX_TYPE_SHORT_16, TOTAL_INDICES);
    indexBuffer->updateIndices(indices, TOTAL_INDICES, 0);

    _primitive = Primitive::create(vertsData, indexBuffer, GL_TRIANGLES);
    _primitive->setCount(TOTAL_INDICES);
    _primitive->setStart(0);

    auto cache = Director::getInstance()->getTextureCache();
    _texture = cache->addImage("Images/grossini.png");
    _programState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);

    _primitive->retain();
    _texture->retain();
    _programState->retain();
}

PrimitivesCommandTest::~PrimitivesCommandTest()
{
    CC_SAFE_RELEASE(_primitive);
    CC_SAFE_RELEASE(_texture);
    CC_SAFE_RELEASE(_programState);
}


void PrimitivesCommandTest::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
    _primitiveCommand.init(_globalZOrder,
                           _texture->getName(),
                           _programState,
                           BlendFunc::ALPHA_NON_PREMULTIPLIED,
                           _primitive,
                           transform,
                           flags);
    renderer->addCommand(&_primitiveCommand);
}

string PrimitivesCommandTest::title() const
{
    return "PrimitiveCommand test";
}

string PrimitivesCommandTest::subtitle() const
{
    return "Drawing Primitives using PrimitiveCommand";
}

//
// Issue11942Test
//
Issue11942Test::Issue11942Test()
{
    auto draw = DrawNode::create();
    addChild(draw, 10);

    // draw a circle
    draw->setLineWidth(1);
    draw->drawCircle(VisibleRect::center() - Vec2(140,0), 50, CC_DEGREES_TO_RADIANS(90), 30, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    draw->setLineWidth(10);
    draw->drawCircle(VisibleRect::center() + Vec2(140,0), 50, CC_DEGREES_TO_RADIANS(90), 30, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
}

string Issue11942Test::title() const
{
    return "Gihub Issue #11942";
}

string Issue11942Test::subtitle() const
{
    return "drawCircle() with width";
}


#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (pop)
#endif
