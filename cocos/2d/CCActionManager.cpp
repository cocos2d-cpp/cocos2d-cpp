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

#include "2d/CCAction.h"
#include "2d/CCActionManager.h"
#include "2d/CCNode.h"
#include "base/ccMacros.h" // CC_ASSERT

#include <algorithm>

namespace cocos2d {

static auto cmp_tt1_tt2 = [](auto const target1, auto const tag1,
                             auto const target2, auto const tag2) {
    return (target1 < target2
            || (target1 == target2
                || tag1 < tag2));
};

static auto cmp_action_pair = [](auto const& a, auto const& p) {
    return cmp_tt1_tt2( a->getTarget(), a->getTag(), p.first, p.second);
};

static auto cmp_pair_action = [](auto const& p, auto const& a) {
    return cmp_tt1_tt2( p.first, p.second, a->getTarget(), a->getTag());
};

static auto cmp_action_action = [](auto const& a, auto const& b) {
    return cmp_tt1_tt2( a->getTarget(), a->getTag(),  b->getTarget(), b->getTag());
};

static auto cmp_action_target = [](auto const& a, auto const t) {
    return a->getTarget() < t;
};

static auto cmp_target_action = [](auto const t, auto const& a) {
    return t < a->getTarget();
};

ActionManager::~ActionManager()
{}

void ActionManager::runAction(std::unique_ptr<Action> action)
{
    CC_ASSERT(action);
    CC_ASSERT(action->getTarget());

    _actionsToAdd.insert
        (
            std::upper_bound
            (
                _actionsToAdd.begin(),
                _actionsToAdd.end(),
                action,
                cmp_action_action
            ),
            std::move(action)
        );
}

Action* ActionManager::getFirstActionForTargetWithTag(const Node *target, Action::tag_t tag) const
{
    auto find = [target,tag](auto & vec) -> Action* {
        auto lb = std::lower_bound
            (
                vec.begin(),
                vec.end(),
                std::make_pair(target, tag),
                cmp_action_pair
            );

        if (lb != vec.end()
            && *lb
            && (*lb)->getTarget() == target
            && (*lb)->getTag() == tag)
        {
            return lb->get();
        }

        return nullptr;
    };

    auto rv = find(_actions);

    if (rv)
    {
        return rv;
    }

    return find(_actionsToAdd);
}

static auto get_n_for = [](auto const& vec, auto const& v,
                           auto lb_cmp, auto ub_cmp,
                           auto match_cmp)
{
    size_t rv = 0;

    auto lb = std::lower_bound( vec.begin(), vec.end(), v, lb_cmp);

    for ( ; lb != vec.end() && !ub_cmp(v, *lb); lb++)
    {
        rv += match_cmp(*lb);
    }

    return rv;
};

size_t ActionManager::nOfActionsForTarget(const Node *target) const
{
    auto match_cmp = [](auto const&) -> bool { return true; };

    return get_n_for(_actions,      target, cmp_action_target, cmp_target_action, match_cmp) 
        +  get_n_for(_actionsToAdd, target, cmp_action_target, cmp_target_action, match_cmp);
}

size_t ActionManager::nOfActionsForTargetWithTag(const Node *target, tag_t tag) const
{
    auto pair = std::make_pair(target, tag);
    auto match_cmp = [](auto const&) -> bool { return true; };

    return get_n_for(_actions     , pair, cmp_action_pair, cmp_pair_action, match_cmp) 
        +  get_n_for(_actionsToAdd, pair, cmp_action_pair, cmp_pair_action, match_cmp);
}

size_t ActionManager::nOfActionsForTargetWithFlags(const Node* target, flags_t flags) const
{
    auto match_cmp = [flags](auto const& a) -> bool { return flags | a->getFlags(); };

    return get_n_for(_actions,      target, cmp_action_target, cmp_target_action, match_cmp) 
        +  get_n_for(_actionsToAdd, target, cmp_action_target, cmp_target_action, match_cmp);
}

size_t ActionManager::stopAllActions()
{
    for (auto& a : _actions)
        a->stop();
    for (auto& a : _actionsToAdd)
        a->stop();
    return _actions.size() + _actionsToAdd.size();
}

static auto stop = [](auto const& vec, auto const& v,
                      auto lb_cmp, auto ub_cmp,
                      auto match_cmp)
{
    size_t rv = 0;
    auto lb = std::lower_bound( vec.begin(), vec.end(), v, lb_cmp);

    for ( ; lb != vec.end() && !ub_cmp(v, *lb); lb++)
    {
        if (match_cmp(*lb))
        {
            (*lb)->stop();
            rv++;
        }
    }

    return rv;
};

size_t ActionManager::stopActionsForTarget(const Node *target)
{
    auto match_cmp = [](auto const&) -> bool { return true; };

    return stop(_actions,      target, cmp_action_target, cmp_target_action, match_cmp)
        +  stop(_actionsToAdd, target, cmp_action_target, cmp_target_action, match_cmp);
}

size_t ActionManager::stopActionsForTargetWithTag(const Node *target, tag_t tag)
{
    auto pair = std::make_pair(target, tag);
    auto match_cmp = [](auto const&) -> bool { return true; };

    return stop(_actions,      pair, cmp_action_pair, cmp_pair_action, match_cmp)
        +  stop(_actionsToAdd, pair, cmp_action_pair, cmp_pair_action, match_cmp);
}

size_t ActionManager::stopActionsForTargetWithFlags(const Node* target, flags_t flags)
{
    auto match_cmp = [flags](auto const& a) -> bool { return flags | a->getFlags(); };

    return stop(_actions,      target, cmp_action_target, cmp_target_action, match_cmp)
        +  stop(_actionsToAdd, target, cmp_action_target, cmp_target_action, match_cmp);
}

void ActionManager::update(float dt)
{
    const size_t size = _actions.size();
    size_t n_stopped = 0;

    for (size_t i = 0; i < size; i++)
    {
        auto & a = _actions[i];

        if (a->hasStopped() || (!a->getTarget()->isPaused() && a->last_update(dt)))
        {
            n_stopped = true;
        }
    }

    if (n_stopped)
    {
        auto first_to_remove = std::remove_if(_actions.begin(), _actions.end(),
                                              [](auto & a) { return a->hasStopped(); });

        std::vector<std::unique_ptr<Action>> to_delete(std::make_move_iterator(first_to_remove),
                                                       std::make_move_iterator(_actions.end()));
        _actions.erase(first_to_remove, _actions.end());
    }

    auto begin = _actions.begin();

    for (auto& a : _actionsToAdd)
    {
        if (!a->hasStopped())
        {
            begin = ++_actions.insert
                (
                    std::upper_bound
                    (
                        begin,
                        _actions.end(),
                        a,
                        cmp_action_action
                    ),
                    std::move(a)
                );
        }
    }

    _actionsToAdd.clear();
}

} // namespace cocos2d
