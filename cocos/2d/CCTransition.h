/****************************************************************************
Copyright (c) 2009-2010 Ricardo Quesada
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

#ifndef __CCTRANSITION_H__
#define __CCTRANSITION_H__

#include "2d/CCScene.h"

namespace cocos2d {

/**
 * @addtogroup _2d
 * @{
 */

//static creation function macro
//c/c++ don't support object creation of using class name
//so, all classes need creation method.

class ActionInterval;
class Node;
class NodeGrid;

/** @class TransitionEaseScene
 * @brief TransitionEaseScene can ease the actions of the scene protocol.
@since v0.8.2
@js NA
*/
class CC_DLL TransitionEaseScene
{
public:
    /** Constructor.
     */
    virtual ~TransitionEaseScene() {}

    /** Returns the Ease action that will be performed on a linear action.
    @since v0.8.2
     *
     * @param action A given interval action.
     * @return The Ease action that will be performed on a linear action.
     */
    virtual ActionInterval * easeActionWithAction(ActionInterval * action) = 0;
};

/** @class TransitionScene
 * @brief Base class for Transition scenes.
*/
class CC_DLL TransitionScene : public Scene
{
public:
    /** Orientation Type used by some transitions.
     */
    enum class Orientation
    {
        /// An horizontal orientation where the Left is nearer
        LEFT_OVER = 0,
        /// An horizontal orientation where the Right is nearer
        RIGHT_OVER = 1,
        /// A vertical orientation where the Up is nearer
        UP_OVER = 0,
        /// A vertical orientation where the Bottom is nearer
        DOWN_OVER = 1,
    };
    
    /** Creates a base transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionScene object.
     */
    static TransitionScene * create(float t, Scene *scene);

    /** Called after the transition finishes.
     */
    void finish(void);

    /** Used by some transitions to hide the outer scene.
     */
    void hideOutShowIn(void);

    Scene* getInScene() const{ return _inScene; }
    float getDuration() const { return _duration; }
    //
    // Overrides
    //
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void cleanup() override;
    
protected:
    TransitionScene();
    virtual ~TransitionScene();

    /** initializes a transition with duration and incoming scene */
    bool initWithDuration(float t,Scene* scene);
    
protected:
    virtual void sceneOrder();
    void setNewScene(float dt);

    Scene *_inScene;
    Scene *_outScene;
    float _duration;
    bool _isInSceneOnTop;
    bool _isSendCleanupToScene;

private:
    TransitionScene(const TransitionScene &) = delete;
    const TransitionScene & operator=(const TransitionScene &) = delete;
};

/** @class TransitionSceneOriented
 * @brief A Transition that supports orientation like.
 * Possible orientation: LeftOver, RightOver, UpOver, DownOver
 */
class CC_DLL TransitionSceneOriented : public TransitionScene
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     * 
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @param orientation A given orientation: LeftOver, RightOver, UpOver, DownOver.
     * @return A autoreleased TransitionSceneOriented object.
     */
    static TransitionSceneOriented * create(float t,Scene* scene, Orientation orientation);
    
protected:
    TransitionSceneOriented();
    virtual ~TransitionSceneOriented();

    /** initializes a transition with duration and incoming scene */
    bool initWithDuration(float t,Scene* scene,Orientation orientation);

protected:
    Orientation _orientation;

private:
    TransitionSceneOriented(const TransitionSceneOriented &) = delete;
    const TransitionSceneOriented & operator=(const TransitionSceneOriented &) = delete;
};

/** @class TransitionRotoZoom
 * @brief TransitionRotoZoom:
Rotate and zoom out the outgoing scene, and then rotate and zoom in the incoming 
*/
class CC_DLL TransitionRotoZoom : public TransitionScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionRotoZoom object.
     */
    static TransitionRotoZoom* create(float t, Scene* scene);

    //
    // Overrides
    //
    virtual void onEnter() override;

protected:
    TransitionRotoZoom();
    virtual ~TransitionRotoZoom();

private:
    TransitionRotoZoom(const TransitionRotoZoom &) = delete;
    const TransitionRotoZoom & operator=(const TransitionRotoZoom &) = delete;

};

/** @class TransitionJumpZoom
 * @brief TransitionJumpZoom:
Zoom out and jump the outgoing scene, and then jump and zoom in the incoming 
*/
class CC_DLL TransitionJumpZoom : public TransitionScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionJumpZoom object.
     */
    static TransitionJumpZoom* create(float t, Scene* scene);

    //
    // Overrides
    //
    virtual void onEnter() override;

protected:
    TransitionJumpZoom();
    virtual ~TransitionJumpZoom();

private:
    TransitionJumpZoom(const TransitionJumpZoom &) = delete;
    const TransitionJumpZoom & operator=(const TransitionJumpZoom &) = delete;
};

/** @class TransitionMoveInL
 * @brief TransitionMoveInL:
Move in from to the left the incoming scene.
*/
class CC_DLL TransitionMoveInL : public TransitionScene, public TransitionEaseScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionMoveInL object.
     */
    static TransitionMoveInL* create(float t, Scene* scene);

    /** Returns the action that will be performed. 
     * 
     * @return The action that will be performed.
     */
    virtual ActionInterval* action(void);

    virtual ActionInterval* easeActionWithAction(ActionInterval * action) override;

    //
    // Overrides
    //
    virtual void onEnter() override;

protected:
    TransitionMoveInL();
    virtual ~TransitionMoveInL();

protected:
    /** initializes the scenes */
    virtual void initScenes();

private:
    TransitionMoveInL(const TransitionMoveInL &) = delete;
    const TransitionMoveInL & operator=(const TransitionMoveInL &) = delete;
};

/** @class TransitionMoveInR
 * @brief TransitionMoveInR:
Move in from to the right the incoming scene.
*/
class CC_DLL TransitionMoveInR : public TransitionMoveInL
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionMoveInR object.
     */
    static TransitionMoveInR* create(float t, Scene* scene);

protected:
    TransitionMoveInR();
    virtual ~TransitionMoveInR();

protected:
    virtual void initScenes();

private:
    TransitionMoveInR(const TransitionMoveInR &) = delete;
    const TransitionMoveInR & operator=(const TransitionMoveInR &) = delete;
};

/** @class TransitionMoveInT
 * @brief TransitionMoveInT:
Move in from to the top the incoming scene.
*/
class CC_DLL TransitionMoveInT : public TransitionMoveInL 
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionMoveInT object.
     */
    static TransitionMoveInT* create(float t, Scene* scene);

protected:
    TransitionMoveInT();
    virtual ~TransitionMoveInT();

protected:
    virtual void initScenes();

private:
    TransitionMoveInT(const TransitionMoveInT &) = delete;
    const TransitionMoveInT & operator=(const TransitionMoveInT &) = delete;
};

/** @class TransitionMoveInB
 * @brief TransitionMoveInB:
Move in from to the bottom the incoming scene.
*/
class CC_DLL TransitionMoveInB : public TransitionMoveInL
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionMoveInB object.
     */
    static TransitionMoveInB* create(float t, Scene* scene);

protected:
    TransitionMoveInB();
    virtual ~TransitionMoveInB();

protected:
    virtual void initScenes();

private:
    TransitionMoveInB(const TransitionMoveInB &) = delete;
    const TransitionMoveInB & operator=(const TransitionMoveInB &) = delete;
};

/** @class TransitionSlideInL
 * @brief TransitionSlideInL:
Slide in the incoming scene from the left border.
*/
class CC_DLL TransitionSlideInL : public TransitionScene, public TransitionEaseScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionSlideInL object.
     */
    static TransitionSlideInL* create(float t, Scene* scene);

    virtual ActionInterval* easeActionWithAction(ActionInterval * action) override;

    /** Returns the action that will be performed by the incoming and outgoing scene.
     *
     * @return The action that will be performed by the incoming and outgoing scene.
     */
    virtual ActionInterval* action(void);

    //
    // Overrides
    //
    virtual void onEnter() override;

protected:
    TransitionSlideInL();
    virtual ~TransitionSlideInL();

protected:
    /** initializes the scenes */
    virtual void initScenes(void);

    virtual void sceneOrder() override;

private:
    TransitionSlideInL(const TransitionSlideInL &) = delete;
    const TransitionSlideInL & operator=(const TransitionSlideInL &) = delete;
};

/** @class TransitionSlideInR
 *@brief TransitionSlideInR:
Slide in the incoming scene from the right border.
*/
class CC_DLL TransitionSlideInR : public TransitionSlideInL 
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionSlideInR object.
     */
    static TransitionSlideInR* create(float t, Scene* scene);

    /** Returns the action that will be performed by the incoming and outgoing scene. */
    virtual ActionInterval* action(void) override;

protected:
    TransitionSlideInR();
    virtual ~TransitionSlideInR();

protected:
    /** initializes the scenes */
    virtual void initScenes(void) override;

    virtual void sceneOrder() override;

private:
    TransitionSlideInR(const TransitionSlideInR &) = delete;
    const TransitionSlideInR & operator=(const TransitionSlideInR &) = delete;
};

/** @class TransitionSlideInB
 * @brief TransitionSlideInB:
Slide in the incoming scene from the bottom border.
*/
class CC_DLL TransitionSlideInB : public TransitionSlideInL
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionSlideInB object.
     */
    static TransitionSlideInB* create(float t, Scene* scene);

    /** returns the action that will be performed by the incoming and outgoing scene */
    virtual ActionInterval* action(void) override;

protected:
    TransitionSlideInB();
    virtual ~TransitionSlideInB();

protected:
    /** initializes the scenes */
    virtual void initScenes() override;

    virtual void sceneOrder() override;

private:
    TransitionSlideInB(const TransitionSlideInB &) = delete;
    const TransitionSlideInB & operator=(const TransitionSlideInB &) = delete;
};

/** @class TransitionSlideInT
 * @brief TransitionSlideInT:
Slide in the incoming scene from the top border.
*/
class CC_DLL TransitionSlideInT : public TransitionSlideInL
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionSlideInT object.
     */
    static TransitionSlideInT* create(float t, Scene* scene);

    /** returns the action that will be performed by the incoming and outgoing scene */
    virtual ActionInterval* action(void) override;

protected:
    TransitionSlideInT();
    virtual ~TransitionSlideInT();

protected:
    /** initializes the scenes */
    virtual void initScenes(void) override;

    virtual void sceneOrder() override;

private:
    TransitionSlideInT(const TransitionSlideInT &) = delete;
    const TransitionSlideInT & operator=(const TransitionSlideInT &) = delete;
};

/** @class TransitionShrinkGrow
* @brief Shrink the outgoing scene while grow the incoming scene
*/
class CC_DLL TransitionShrinkGrow : public TransitionScene , public TransitionEaseScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionShrinkGrow object.
     */
    static TransitionShrinkGrow* create(float t, Scene* scene);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;
    virtual ActionInterval* easeActionWithAction(ActionInterval * action) override;

protected:
    TransitionShrinkGrow();
    virtual ~TransitionShrinkGrow();

private:
    TransitionShrinkGrow(const TransitionShrinkGrow &) = delete;
    const TransitionShrinkGrow & operator=(const TransitionShrinkGrow &) = delete;
};

/** @class TransitionFlipX
 * @brief TransitionFlipX:
Flips the screen horizontally.
The front face is the outgoing scene and the back face is the incoming scene.
*/
class CC_DLL TransitionFlipX : public TransitionSceneOriented
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @param o A given orientation.
     * @return A autoreleased TransitionFlipX object.
     */
    static TransitionFlipX* create(float t, Scene* s, Orientation o);
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @return A autoreleased TransitionFlipX object.
     */
    static TransitionFlipX* create(float t, Scene* s);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;

protected:
    TransitionFlipX();
    virtual ~TransitionFlipX();

private:
    TransitionFlipX(const TransitionFlipX &) = delete;
    const TransitionFlipX & operator=(const TransitionFlipX &) = delete;
};

/** @class TransitionFlipY
 * @brief TransitionFlipY:
Flips the screen vertically.
The front face is the outgoing scene and the back face is the incoming scene.
*/
class CC_DLL TransitionFlipY : public TransitionSceneOriented
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @param o A given orientation.
     * @return A autoreleased TransitionFlipY object.
     */
    static TransitionFlipY* create(float t, Scene* s, Orientation o);
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @return A autoreleased TransitionFlipY object.
     */
    static TransitionFlipY* create(float t, Scene* s);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;

protected:
    TransitionFlipY();
    virtual ~TransitionFlipY();

private:
    TransitionFlipY(const TransitionFlipY &) = delete;
    const TransitionFlipY & operator=(const TransitionFlipY &) = delete;
};

/** @class TransitionFlipAngular
 * @brief TransitionFlipAngular:
Flips the screen half horizontally and half vertically.
The front face is the outgoing scene and the back face is the incoming scene.
*/
class CC_DLL TransitionFlipAngular : public TransitionSceneOriented
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @param o A given orientation.
     * @return A autoreleased TransitionFlipAngular object.
     */
    static TransitionFlipAngular* create(float t, Scene* s, Orientation o);
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @return A autoreleased TransitionFlipAngular object.
     */
    static TransitionFlipAngular* create(float t, Scene* s);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;

protected:
    TransitionFlipAngular();
    virtual ~TransitionFlipAngular();

private:
    TransitionFlipAngular(const TransitionFlipAngular &) = delete;
    const TransitionFlipAngular & operator=(const TransitionFlipAngular &) = delete;
};

/** @class TransitionZoomFlipX
 * @brief TransitionZoomFlipX:
Flips the screen horizontally doing a zoom out/in
The front face is the outgoing scene and the back face is the incoming scene.
*/
class CC_DLL TransitionZoomFlipX : public TransitionSceneOriented
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @param o A given orientation.
     * @return A autoreleased TransitionZoomFlipX object.
     */
    static TransitionZoomFlipX* create(float t, Scene* s, Orientation o);
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @return A autoreleased TransitionZoomFlipX object.
     */
    static TransitionZoomFlipX* create(float t, Scene* s);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;

protected:
    TransitionZoomFlipX();
    virtual ~TransitionZoomFlipX();

private:
    TransitionZoomFlipX(const TransitionZoomFlipX &) = delete;
    const TransitionZoomFlipX & operator=(const TransitionZoomFlipX &) = delete;
};

/** @class TransitionZoomFlipY
 * @brief TransitionZoomFlipY:
Flips the screen vertically doing a little zooming out/in
The front face is the outgoing scene and the back face is the incoming scene.
*/
class CC_DLL TransitionZoomFlipY : public TransitionSceneOriented
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @param o A given orientation.
     * @return A autoreleased TransitionZoomFlipY object.
     */
    static TransitionZoomFlipY* create(float t, Scene* s, Orientation o);
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @return A autoreleased TransitionZoomFlipY object.
     */
    static TransitionZoomFlipY* create(float t, Scene* s);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;

protected:
    TransitionZoomFlipY();
    virtual ~TransitionZoomFlipY();

private:
    TransitionZoomFlipY(const TransitionZoomFlipY &) = delete;
    const TransitionZoomFlipY & operator=(const TransitionZoomFlipY &) = delete;
};

/** @class TransitionZoomFlipAngular
 * @brief TransitionZoomFlipAngular:
Flips the screen half horizontally and half vertically doing a little zooming out/in.
The front face is the outgoing scene and the back face is the incoming scene.
*/
class CC_DLL TransitionZoomFlipAngular : public TransitionSceneOriented
{
public:
    /** Creates a transition with duration, incoming scene and orientation.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @param o A given orientation.
     * @return A autoreleased TransitionZoomFlipAngular object.
     */
    static TransitionZoomFlipAngular* create(float t, Scene* s, Orientation o);
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param s A given scene.
     * @return A autoreleased TransitionZoomFlipAngular object.
     */
    static TransitionZoomFlipAngular* create(float t, Scene* s);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;

protected:
    TransitionZoomFlipAngular();
    virtual ~TransitionZoomFlipAngular();

private:
    TransitionZoomFlipAngular(const TransitionZoomFlipAngular &) = delete;
    const TransitionZoomFlipAngular & operator=(const TransitionZoomFlipAngular &) = delete;
};

/** @class TransitionFade
 * @brief TransitionFade:
Fade out the outgoing scene and then fade in the incoming scene.'''
*/
class CC_DLL TransitionFade : public TransitionScene
{
public:
    /** Creates the transition with a duration and with an RGB color
     * Example: FadeTransition::create(2, scene, Color3B(255,0,0); // red color
     *
     * @param duration Duration time, in seconds.
     * @param scene A given scene.
     * @param color A given transition color.
     * @return A autoreleased TransitionFade object.
     */
    static TransitionFade* create(float duration, Scene* scene, const Color3B& color);
    /** Creates the transition with a duration.
     *
     * @param duration Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionFade object.
     */
    static TransitionFade* create(float duration, Scene* scene);

    /**
     * @lua NA
     */
    virtual void onEnter() override;
    /**
     * @lua NA
     */
    virtual void onExit() override;
    
protected:
    TransitionFade();
    virtual ~TransitionFade();

    /** initializes the transition with a duration and with an RGB color */
    bool initWithDuration(float t, Scene*scene, const Color3B& color);
    bool initWithDuration(float t, Scene* scene);

protected:
    Color4B _color;

private:
    TransitionFade(const TransitionFade &) = delete;
    const TransitionFade & operator=(const TransitionFade &) = delete;

};

class RenderTexture;
/** @class TransitionCrossFade
@brief TransitionCrossFade:
Cross fades two scenes using the RenderTexture object.
*/
class CC_DLL TransitionCrossFade : public TransitionScene
{
public :
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionCrossFade object.
     */
    static TransitionCrossFade* create(float t, Scene* scene);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    /**
     * @lua NA
     */
    virtual void onEnter() override;
    /**
     * @lua NA
     */
    virtual void onExit() override;

protected:
    TransitionCrossFade();
    virtual ~TransitionCrossFade();

private:
    TransitionCrossFade(const TransitionCrossFade &) = delete;
    const TransitionCrossFade & operator=(const TransitionCrossFade &) = delete;
};

/** @class TransitionTurnOffTiles
 * @brief TransitionTurnOffTiles:
Turn off the tiles of the outgoing scene in random order
*/
class CC_DLL TransitionTurnOffTiles : public TransitionScene ,public TransitionEaseScene
{
public :
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionTurnOffTiles object.
     */
    static TransitionTurnOffTiles* create(float t, Scene* scene);

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;
    /**
     * @js NA
     */
    virtual void onExit() override;
	virtual ActionInterval * easeActionWithAction(ActionInterval * action) override;
	/**
	* @js NA
	*/
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

protected:
    TransitionTurnOffTiles();
    virtual ~TransitionTurnOffTiles();

protected:
    virtual void sceneOrder() override;
    NodeGrid* _outSceneProxy;

private:
    TransitionTurnOffTiles(const TransitionTurnOffTiles &) = delete;
    const TransitionTurnOffTiles & operator=(const TransitionTurnOffTiles &) = delete;
};

/** @class TransitionSplitCols
 * @brief TransitionSplitCols:
The odd columns goes upwards while the even columns goes downwards.
*/
class CC_DLL TransitionSplitCols : public TransitionScene , public TransitionEaseScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionSplitCols object.
     */
    static TransitionSplitCols* create(float t, Scene* scene);

    /** Returns the action that will be performed.
     *
     * @return The action that will be performed.
     */
    virtual ActionInterval* action();

    //
    // Overrides
    //
    /**
     * @lua NA
     */
    virtual void onEnter() override;
    virtual ActionInterval * easeActionWithAction(ActionInterval * action) override;
    virtual void onExit() override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

protected:
    TransitionSplitCols();
    virtual ~TransitionSplitCols();

protected:
    void switchTargetToInscene();
    NodeGrid* _gridProxy;
private:
    TransitionSplitCols(const TransitionSplitCols &) = delete;
    const TransitionSplitCols & operator=(const TransitionSplitCols &) = delete;
};

/** @class TransitionSplitRows
 * @brief TransitionSplitRows:
The odd rows goes to the left while the even rows goes to the right.
*/
class CC_DLL TransitionSplitRows : public TransitionSplitCols
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionSplitRows object.
     */
    static TransitionSplitRows* create(float t, Scene* scene);

    //
    // Overrides
    //
    virtual ActionInterval* action(void) override;

protected:
    TransitionSplitRows();
    virtual ~TransitionSplitRows();

private:
    TransitionSplitRows(const TransitionSplitRows &) = delete;
    const TransitionSplitRows & operator=(const TransitionSplitRows &) = delete;
};

/** @class TransitionFadeTR
 * @brief TransitionFadeTR:
Fade the tiles of the outgoing scene from the left-bottom corner the to top-right corner.
*/
class CC_DLL TransitionFadeTR : public TransitionScene , public TransitionEaseScene
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionFadeTR object.
     */
    static TransitionFadeTR* create(float t, Scene* scene);

    /** Returns the action that will be performed with size.
     *
     * @param size A given size.
     * @return The action that will be performed.
     */
    virtual ActionInterval* actionWithSize(const Size& size);

    //
    // Overrides
    //
    /**
     * @js NA
     * @lua NA
     */
    virtual void onEnter() override;
    virtual ActionInterval* easeActionWithAction(ActionInterval * action) override;
    virtual void onExit() override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

protected:
    TransitionFadeTR();
    virtual ~TransitionFadeTR();

protected:
    virtual void sceneOrder() override;

    NodeGrid* _outSceneProxy;

private:
    TransitionFadeTR(const TransitionFadeTR &) = delete;
    const TransitionFadeTR & operator=(const TransitionFadeTR &) = delete;
};

/** @class TransitionFadeBL
 * @brief TransitionFadeBL:
Fade the tiles of the outgoing scene from the top-right corner to the bottom-left corner.
*/
class CC_DLL TransitionFadeBL : public TransitionFadeTR
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionFadeBL object.
     */
    static TransitionFadeBL* create(float t, Scene* scene);

    //
    // Overrides
    //
    virtual ActionInterval* actionWithSize(const Size& size) override;

protected:
    TransitionFadeBL();
    virtual ~TransitionFadeBL();

private:
    TransitionFadeBL(const TransitionFadeBL &) = delete;
    const TransitionFadeBL & operator=(const TransitionFadeBL &) = delete;
};

/** @class TransitionFadeUp
 * @brief TransitionFadeUp:
* Fade the tiles of the outgoing scene from the bottom to the top.
*/
class CC_DLL TransitionFadeUp : public TransitionFadeTR
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionFadeUp object.
     */
    static TransitionFadeUp* create(float t, Scene* scene);

    //
    // Overrides
    //
    virtual ActionInterval* actionWithSize(const Size& size) override;

protected:
    TransitionFadeUp();
    virtual ~TransitionFadeUp();

private:
    TransitionFadeUp(const TransitionFadeUp &) = delete;
    const TransitionFadeUp & operator=(const TransitionFadeUp &) = delete;
};

/** @class TransitionFadeDown
 * @brief TransitionFadeDown:
* Fade the tiles of the outgoing scene from the top to the bottom.
*/
class CC_DLL TransitionFadeDown : public TransitionFadeTR
{
public:
    /** Creates a transition with duration and incoming scene.
     *
     * @param t Duration time, in seconds.
     * @param scene A given scene.
     * @return A autoreleased TransitionFadeDown object.
     */
    static TransitionFadeDown* create(float t, Scene* scene);

    //
    // Overrides
    //
    virtual ActionInterval* actionWithSize(const Size& size) override;

protected:
    TransitionFadeDown();
    virtual ~TransitionFadeDown();

private:
    TransitionFadeDown(const TransitionFadeDown &) = delete;
    const TransitionFadeDown & operator=(const TransitionFadeDown &) = delete;

};

// end of _2d group
/// @}

} // namespace cocos2d

#endif // __CCTRANSITION_H__

