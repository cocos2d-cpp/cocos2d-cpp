/****************************************************************************
CopyRight (c) 2017      Iakov Sergeev <yahont@github>

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

#ifndef __ACTION_CCACTION_MANAGER_H__
#define __ACTION_CCACTION_MANAGER_H__

#include "2d/CCAction.h"

#include <memory> // unique_ptr
#include <vector>

namespace cocos2d {

class Action;
class Node;

class CC_DLL ActionManager final
{
public:
    using tag_t   = Action::tag_t;
    using flags_t = Action::flags_t;

    ActionManager() = default;
    ActionManager(ActionManager const&) = delete;
    ActionManager& operator=(ActionManager const&) = delete;

    ~ActionManager();

   
    void runAction(std::unique_ptr<Action> action);

    // getters

    Action* getFirstActionForTargetWithTag(const Node *, tag_t) const;

    size_t nOfActionsForTarget(const Node *target) const;
    size_t nOfActionsForTargetWithTag(const Node *, tag_t) const;
    size_t nOfActionsForTargetWithFlags(const Node *, flags_t) const;

    // stoppers

    size_t stopAllActions();

    size_t stopActionsForTarget(const Node *target);
    size_t stopActionsForTargetWithTag(const Node *target, tag_t);
    size_t stopActionsForTargetWithFlags(const Node*, flags_t);

    // do not use it

    void update(float dt);

 private:

    std::vector<std::unique_ptr<Action>> _actions;
    std::vector<std::unique_ptr<Action>> _actionsToAdd;
    // TODO Consider having _actionsPaused
};

// end of actions group
/// @}

} // namespace cocos2d

#endif // __ACTION_CCACTION_MANAGER_H__
