/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.

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

#ifndef __SPRITE_NODE_CCSPRITE_H__
#define __SPRITE_NODE_CCSPRITE_H__

#include <string>
#include "2d/CCNode.h"
#include "2d/CCDrawNode.h"
#include "renderer/CCTextureAtlas.h"
#include "renderer/CCTrianglesCommand.h"
#include "renderer/CCCustomCommand.h"
#include "2d/CCAutoPolygon.h"

namespace cocos2d {

class SpriteBatchNode;
class SpriteFrame;
class Animation;
class Rect;
class Size;
class Texture2D;
struct transformValues_;

#ifdef SPRITE_RENDER_IN_SUBPIXEL
#undef SPRITE_RENDER_IN_SUBPIXEL
#endif

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define SPRITE_RENDER_IN_SUBPIXEL
#else
#define SPRITE_RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

/**
 * @addtogroup _2d
 * @{
 */

/**
 * Sprite is a 2d image ( http://en.wikipedia.org/wiki/Sprite_(computer_graphics) ).
 *
 * Sprite can be created with an image, or with a sub-rectangle of an image.
 *
 * To optimize the Sprite rendering, please follow the following best practices:
 *  - Put all your sprites in the same spritesheet (http://www.codeandweb.com/what-is-a-sprite-sheet).
 *  - Use the same blending function for all your sprites.
 *  - ...and the Renderer will automatically "batch" your sprites (will draw all of them in one OpenGL call).
 *
 *  To gain an additional 5% ~ 10% more in the rendering, you can parent your sprites into a `SpriteBatchNode`.
 *  But doing so carries the following limitations:
 *
 *  - The Alias/Antialias property belongs to `SpriteBatchNode`, so you can't individually set the aliased property.
 *  - The Blending function property belongs to `SpriteBatchNode`, so you can't individually set the blending function property.
 *  - `ParallaxNode` is not supported, but can be simulated with a "proxy" sprite.
 *  - Sprites can only have other Sprites (or subclasses of Sprite) as children.
 *
 * The default anchorPoint in Sprite is (0.5, 0.5).
 */
class CC_DLL Sprite : public Node
{
public:
     /** Sprite invalid index on the SpriteBatchNode. */
    static const int INDEX_NOT_INITIALIZED = -1;

    /// @name Creators
    /// @{

    /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @memberof Sprite
     * @return An autoreleased sprite object.
     */
    static Sprite* create();

    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   filename A path to image file, e.g., "scene1/monster.png".
     * @return  An autoreleased sprite object.
     */
    static Sprite* create(const std::string& filename);
    
    /**
     * Creates a polygon sprite with a polygon info.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   polygonInfo A path to image file, e.g., "scene1/monster.png".
     * @return  An autoreleased sprite object.
     */
    static Sprite* create(const PolygonInfo& info);

    /**
     * Creates a sprite with an image filename and a rect.
     *
     * @param   filename A path to image file, e.g., "scene1/monster.png".
     * @param   rect     A subrect of the image file.
     * @return  An autoreleased sprite object.
     */
    static Sprite* create(const std::string& filename, const Rect& rect);

    /**
     * Creates a sprite with a Texture2D object.
     *
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   texture    A pointer to a Texture2D object.
     * @return  An autoreleased sprite object.
     */
    static Sprite* create(const Texture2D *texture);

    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   texture     A pointer to an existing Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @param   rotated     Whether or not the rect is rotated.
     * @return  An autoreleased sprite object.
     */
    static Sprite* create(const Texture2D *texture, const Rect& rect, bool rotated=false);

    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   spriteFrame    A sprite frame which involves a texture and a rect.
     * @return  An autoreleased sprite object.
     */
    static Sprite* create(SpriteFrame *spriteFrame);

    //  end of creators group
    /// @}

    /// @{
    /// @name BatchNode methods

    /**
     * Updates the quad according the rotation, position, scale values.
     */
    virtual void updateTransform() override;

    /**
     * Returns the batch node object if this sprite is rendered by SpriteBatchNode.
     *
     * @return The SpriteBatchNode object if this sprite is rendered by SpriteBatchNode,
     *         nullptr if the sprite isn't used batch node.
     */
    virtual SpriteBatchNode* getBatchNode() const;
    /**
     * Sets the batch node to sprite.
     * @warning This method is not recommended for game developers. Sample code for using batch node
     * @code
     * SpriteBatchNode *batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
     * Sprite *sprite = Sprite::create(batch->getTexture(), Rect(0, 0, 57, 57));
     * batch->addChild(sprite);
     * layer->addChild(batch);
     * @endcode
     */
    virtual void setBatchNode(SpriteBatchNode *spriteBatchNode);

    /// @} end of BatchNode methods


    /// @{
    /// @name Texture / Frame methods

    /** Sets a new texture (from a filename) to the sprite.
     *
     *  @memberof Sprite
     *  It will call `setTextureRect()` with the texture's content size.
     */
    virtual void setTexture(const std::string &filename );

    /**
     *  The Texture's rect is not changed.
     */
    void setTexture(const Texture2D *texture);

    /** Returns the Texture2D object used by the sprite. */
    const Texture2D* getTexture() const;

    /**
     * Updates the texture rect of the Sprite in points.
     *
     * It will call setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize) with \p rotated = false, and \p utrimmedSize = rect.size.
     */
    virtual void setTextureRect(const Rect& rect);

    /** @overload
     *
     * It will update the texture coordinates and the vertex rectangle.
     */
    virtual void setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize);

    /**
     * Sets the vertex rect.
     *
     * It will be called internally by setTextureRect.
     * Useful if you want to create 2x images from SD images in Retina Display.
     * Do not call it manually. Use setTextureRect instead.
     */
    virtual void setVertexRect(const Rect& rect);

    /**
     * setCenterRectNormalized
     *
     * Useful to implement "9 sliced" sprites.
     * The default value is (0,0) - (1,1), which means that only one "slice" will be used: From top-left (0,0) to bottom-right (1,1).
     * If the value is different than (0,0), (1,1), then the sprite will be sliced into a 3 x 3 grid. The four corners of this grid are applied without
     * performing any scaling. The upper- and lower-middle parts are scaled horizontally, and the left- and right-middle parts are scaled vertically.
     * The center is scaled in both directions.
     * Important: The scaling is based the Sprite's trimmed size.
     *
     * Limitations: Does not work when the sprite is part of `SpriteBatchNode`.
     */
    virtual void setCenterRectNormalized(const Rect& rect);

    /**
     * getCenterRectNormalized
     *
     * Returns the CenterRect in normalized coordinates
     */
    virtual Rect getCenterRectNormalized() const;

    /* setCenterRect
     *
     * Like `setCenterRectNormalized`, but instead of being in normalized coordinates, it is in points coordinates
     */
    virtual void setCenterRect(const Rect& rect);

    /**
     * @brief Returns the Cap Insets rect
     *
     * @return Scale9Sprite's cap inset.
     */
    virtual Rect getCenterRect() const;


    /** @{
     * Sets a new SpriteFrame to the Sprite.
     */
    virtual void setSpriteFrame(SpriteFrame* newFrame);
    /** @} */

    /**
     * Returns whether or not a SpriteFrame is being displayed.
     */
    virtual bool isFrameDisplayed(SpriteFrame *frame) const;

    /**
     * Returns the current displayed frame.
     */
    virtual SpriteFrame* getSpriteFrame() const;

    /// @} End of frames methods

    /// @{
    /// @name Sprite Properties' setter/getters.

    /**
     * Whether or not the Sprite needs to be updated in the Atlas.
     *
     * @return True if the sprite needs to be updated in the Atlas, false otherwise.
     */
    virtual bool isDirty() const { return _dirty; }

    /**
     * Makes the Sprite to be updated in the Atlas.
     */
    virtual void setDirty(bool dirty) { _dirty = dirty; }

    /**
     * Returns the quad (tex coords, vertex coords and color) information.
     * @js  NA
     * @lua NA
     */
    V3F_C4B_T2F_Quad getQuad() const { return _quad; }

    /**
     * Returns whether or not the texture rectangle is rotated.
     */
    bool isTextureRectRotated() const { return _rectRotated; }

    /**
     * Returns the index used on the TextureAtlas.
     */
    ssize_t getAtlasIndex() const { return _atlasIndex; }

    /**
     * Sets the index used on the TextureAtlas.
     *
     * @warning Don't modify this value unless you know what you are doing.
     */
    void setAtlasIndex(ssize_t atlasIndex) { _atlasIndex = atlasIndex; }

    /**
     * Returns the rect of the Sprite in points.
     */
    const Rect& getTextureRect() const { return _rect; }

    /**
     * Gets the weak reference of the TextureAtlas when the sprite is rendered using via SpriteBatchNode.
     */
    TextureAtlas* getTextureAtlas() const { return _textureAtlas; }

    /**
     * Sets the weak reference of the TextureAtlas when the sprite is rendered using via SpriteBatchNode.
     */
    void setTextureAtlas(TextureAtlas *textureAtlas) { _textureAtlas = textureAtlas; }

    /**
     * Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
     */
    const Vec2& getOffsetPosition() const { return _offsetPosition; }


    /**
     * Returns the flag which indicates whether the sprite is flipped horizontally or not.
     *
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleX(sprite->getScaleX() * -1);
     *
     * @return true if the sprite is flipped horizontally, false otherwise.
     */
    bool isFlippedX() const;
    /**
     * Sets whether the sprite should be flipped horizontally or not.
     *
     * @param flippedX true if the sprite should be flipped horizontally, false otherwise.
     */
    void setFlippedX(bool flippedX);

    /**
     * Return the flag which indicates whether the sprite is flipped vertically or not.
     *
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleY(sprite->getScaleY() * -1);
     *
     * @return true if the sprite is flipped vertically, false otherwise.
     */
    bool isFlippedY() const;
    /**
     * Sets whether the sprite should be flipped vertically or not.
     *
     * @param flippedY true if the sprite should be flipped vertically, false otherwise.
     */
    void setFlippedY(bool flippedY);

    /// @} End of Sprite properties getter/setters

    /**
     * returns a reference of the polygon information associated with this sprite
     *
     * @return a reference of PolygonInfo
     */
    const PolygonInfo& getPolygonInfo() const;

    /**
     * set the sprite to use this new PolygonInfo
     *
     * @param PolygonInfo the polygon information object
     */
    void setPolygonInfo(const PolygonInfo& info);

    /** whether or not contentSize streches the sprite's texture */
    void setStrechEnabled(bool enabled);

    /** returns whether or not contentSize streches the sprite's texture */
    bool isStrechEnabled() const;

    void setBlendFunc(const BlendFunc &blendFunc)
    {
        _blendFunc = blendFunc;
    }

    const BlendFunc& getBlendFunc() const
    {
        return _blendFunc;
    }

    virtual std::string getDescription() const override;

    /// @{
    /// @name Functions inherited from Node.
    virtual void setScaleX(float scaleX) override;
    virtual void setScaleY(float scaleY) override;
    virtual void setScale(float scaleX, float scaleY) override;

    virtual void setPosition(const Vec2& pos) override;
    virtual void setPosition(float x, float y) override;
    virtual void setRotation(float rotation) override;
    virtual void setRotationSkewX(float rotationX) override;
    virtual void setRotationSkewY(float rotationY) override;
    virtual void setSkewX(float sx) override;
    virtual void setSkewY(float sy) override;
    CC_DEPRECATED_ATTRIBUTE node_ptr<Node> removeChild(Node* child, bool cleanup = true) override
    {
        if (child != nullptr)
            return removeChild(child->getNodeId(), cleanup);
        return node_ptr<Node>();
    }
    virtual node_ptr<Node> removeChild(NodeId child, bool cleanup) override;
    virtual void removeAllChildrenWithCleanup(bool cleanup) override;
    virtual void reorderChild(Node *child, int zOrder) override;

    using Node::addChild;
    virtual NodeId addChild(node_ptr<Node> child, int zOrder, int tag) override;
    virtual NodeId addChild(node_ptr<Node> child, int zOrder, const std::string &name) override;

    virtual void sortAllChildren() override;
    virtual void setScale(float scale) override;
    virtual void setPositionZ(float positionZ) override;
    virtual void setAnchorPoint(const Vec2& anchor) override;
    virtual void setContentSize(const Size& size) override;
    
    virtual void setIgnoreAnchorPointForPosition(bool value) override;
    
    virtual void setVisible(bool bVisible) override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    virtual void setOpacityModifyRGB(bool modify) override;
    virtual bool isOpacityModifyRGB() const override;
    /// @}

protected :

    Sprite();
    virtual ~Sprite();

    /* Initializes an empty sprite with no parameters. */
    virtual bool init() override;

    /**
     * Initializes a sprite with a texture.
     *
     * After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
     *
     * @param   texture    A pointer to an existing Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @return  True if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(const Texture2D *texture);
    
    
    /**
     * Initializes a sprite with a PolygonInfo.
     *
     * After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
     *
     * @param   PolygonInfo    a Polygon info contains the structure of the polygon.
     * @return  True if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithPolygon(const PolygonInfo& info);

    /**
     * Initializes a sprite with a texture and a rect.
     *
     * After initialization, the offset will be (0,0).
     *
     * @param   texture    A pointer to an existing Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @param   rect        Only the contents inside rect of this texture will be applied for this sprite.
     * @return  True if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(const Texture2D *texture, const Rect& rect);

    /**
     * Initializes a sprite with a texture and a rect in points, optionally rotated.
     *
     * After initialization, the offset will be (0,0).
     * @note    This is the designated initializer.
     *
     * @param   texture    A Texture2D object whose texture will be applied to this sprite.
     * @param   rect        A rectangle assigned the contents of texture.
     * @param   rotated     Whether or not the texture rectangle is rotated.
     * @return  True if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(const Texture2D *texture, const Rect& rect, bool rotated);

    /**
     * Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite.
     *
     * @param   spriteFrame  A SpriteFrame object. It should includes a valid texture and a rect.
     * @return  True if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithSpriteFrame(SpriteFrame *spriteFrame);

    /**
     * Initializes a sprite with an image filename.
     *
     * This method will find filename from local file system, load its content to Texture2D,
     * then use Texture2D to create a sprite.
     * After initialization, the rect used will be the size of the image. The offset will be (0,0).
     *
     * @param   filename The path to an image file in local file system.
     * @return  True if the sprite is initialized properly, false otherwise.
     * @lua     init
     */
    virtual bool initWithFile(const std::string& filename);

    /**
     * Initializes a sprite with an image filename, and a rect.
     *
     * This method will find filename from local file system, load its content to Texture2D,
     * then use Texture2D to create a sprite.
     * After initialization, the offset will be (0,0).
     *
     * @param   filename The path to an image file in local file system.
     * @param   rect        The rectangle assigned the content area from texture.
     * @return  True if the sprite is initialized properly, false otherwise.
     * @lua     init
     */
    virtual bool initWithFile(const std::string& filename, const Rect& rect);
    
protected:

    void updateColor() override;
    virtual void setTextureCoords(const Rect& rect);
    virtual void setTextureCoords(const Rect& rect, V3F_C4B_T2F_Quad* outQuad);
    virtual void setVertexCoords(const Rect& rect, V3F_C4B_T2F_Quad* outQuad);
    virtual void updateBlendFunc();
    virtual void setReorderChildDirtyRecursively();
    virtual void setDirtyRecursively(bool value);

    void updatePoly();
    void updateStretchFactor();

protected:
    //
    // Data used when the sprite is rendered using a SpriteSheet
    //
    TextureAtlas*       _textureAtlas;      /// SpriteBatchNode texture atlas (weak reference)
    ssize_t             _atlasIndex;        /// Absolute (real) Index on the SpriteSheet
    SpriteBatchNode*    _batchNode;         /// Used batch node (weak reference)

    bool                _dirty;             /// Whether the sprite needs to be updated
    bool                _recursiveDirty;    /// Whether all of the sprite's children needs to be updated
    bool                _shouldBeHidden;    /// should not be drawn because one of the ancestors is not visible
    Mat4              _transformToBatch;

    //
    // Data used when the sprite is self-rendered
    //
    BlendFunc        _blendFunc;





    SpriteFrame*     _spriteFrame;
    const Texture2D* _texture;              /// Texture2D object that is used to render the sprite
    Vec2 _unflippedOffsetPositionFromCenter; // SpriteFrame._offset
    bool _rectRotated;                      /// Whether the texture is rotated
    Size _originalContentSize;              /// original content size
    Rect _rect;                             /// Rectangle of Texture2D
    PolygonInfo  _polyInfo;

    TrianglesCommand _trianglesCommand;     ///
#if CC_SPRITE_DEBUG_DRAW
    NodeId _debugDrawNodeId;
#endif //CC_SPRITE_DEBUG_DRAW
    //
    // Shared data
    //

    Rect _centerRectNormalized;             /// Rectangle to implement "slice 9"
    int _numberOfSlices;                    /// how many sprite slices: 1 or 9
    Vec2 _strechFactor;                     /// strech factor to match the contentSize. for 1- and 9- slice sprites
    bool _strechEnabled;

    // Offset Position (used by Zwoptex)
    Vec2 _offsetPosition;

    // vertex coords, texture coords and color info
    V3F_C4B_T2F_Quad _quad;
    V3F_C4B_T2F_Quad* _quads;

    // opacity and RGB protocol
    bool _opacityModifyRGB;

    // image is flipped
    bool _flippedX;                         /// Whether the sprite is flipped horizontally or not
    bool _flippedY;                         /// Whether the sprite is flipped vertically or not

    bool _insideBounds;                     /// whether or not the sprite was inside bounds the previous frame

private:
    Sprite(const Sprite &) = delete;
    const Sprite & operator=(const Sprite &) = delete;
};


// end of sprite_nodes group
/// @}

} // namespace cocos2d

#endif // __SPRITE_NODE_CCSPRITE_H__
