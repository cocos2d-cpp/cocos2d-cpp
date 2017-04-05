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

#include <algorithm>
#include <cassert>

namespace cocos2d {

namespace {

inline bool _compare(Node const* target1, Action::tag_t const tag1,
                     Node const* target2, Action::tag_t const tag2)
{
    return target1 < target2
        || (target1 == target2 && tag1 < tag2);
}

inline bool _compare(std::unique_ptr<Action> const& a, std::pair<Node const*,Action::tag_t> const& p)
{
    return _compare(a->getTarget(), a->getTag(), p.first, p.second);
}

inline bool _compare(std::pair<Node const*,Action::tag_t> const& p, std::unique_ptr<Action> const& a)
{
    return _compare(p.first, p.second, a->getTarget(), a->getTag());
}

inline bool _compare(std::unique_ptr<Action> const& a, std::unique_ptr<Action> const& b)
{
    return _compare(a->getTarget(), a->getTag(), b->getTarget(), b->getTag());
}

inline bool _compare(std::unique_ptr<Action> const& a, Node const* t)
{
    return a->getTarget() < t;
}

inline bool _compare(Node const* t, std::unique_ptr<Action> const& a)
{
    return t < a->getTarget();
}

size_t count_matching(auto const& vec, auto const& v, auto match_cmp)
{
    size_t rv = 0;

    auto lb = std::lower_bound(vec.begin(), vec.end(), v,
                               [](auto & a, auto & b) { return _compare(a, b); });

    for ( ; lb != vec.end() && !_compare(v, *lb); lb++)
    {
        rv += match_cmp(*lb);
    }

    return rv;
}

auto stop_matching(auto const& vec, auto const& v, auto match_cmp)
{
    size_t rv = 0;
    auto lb = std::lower_bound(vec.begin(), vec.end(), v,
                               [](auto & a, auto & b) { return _compare(a, b); });

    for ( ; lb != vec.end() && !_compare(v, *lb); lb++)
    {
        if (match_cmp(*lb))
        {
            (*lb)->stop();
            rv++;
        }
    }

    return rv;
}

} // unnamed namespace

ActionManager::~ActionManager()
{}

void ActionManager::runAction(std::unique_ptr<Action> action)
{
    assert(action);
    assert(action->getTarget() != nullptr);

    auto ub = std::upper_bound(_actionsToAdd.begin(), _actionsToAdd.end(), action,
                               [](auto & a, auto & b) { return _compare(a, b); });

    _actionsToAdd.insert(ub, std::move(action));
}

Action* ActionManager::getFirstActionForTargetWithTag(const Node *target, Action::tag_t tag) const
{
    auto find = [target,tag](auto & vec) -> Action* {
        auto lb = std::lower_bound
            (
                vec.begin(),
                vec.end(),
                std::make_pair(target, tag),
                [](auto & a, auto & b) { return _compare(a, b); }
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

    if (rv != nullptr)
    {
        return rv;
    }

    return find(_actionsToAdd);
}

size_t ActionManager::nOfActionsForTarget(const Node *target) const
{
    auto match_cmp = [](auto const&) -> bool { return true; };

    return count_matching(_actions,      target, match_cmp) 
        +  count_matching(_actionsToAdd, target, match_cmp);
}

size_t ActionManager::nOfActionsForTargetWithTag(const Node *target, tag_t tag) const
{
    auto pair = std::make_pair(target, tag);
    auto match_cmp = [](auto const&) -> bool { return true; };

    return count_matching(_actions     , pair, match_cmp) 
        +  count_matching(_actionsToAdd, pair, match_cmp);
}

size_t ActionManager::nOfActionsForTargetWithFlags(const Node* target, flags_t flags) const
{
    auto match_cmp = [flags](auto const& a) -> bool { return flags | a->getFlags(); };

    return count_matching(_actions,      target, match_cmp) 
        +  count_matching(_actionsToAdd, target, match_cmp);
}

size_t ActionManager::stopAllActions()
{
    for (auto& a : _actions)
        a->stop();
    for (auto& a : _actionsToAdd)
        a->stop();
    return _actions.size() + _actionsToAdd.size();
}

size_t ActionManager::stopActionsForTarget(const Node *target)
{
    auto match_cmp = [](auto const&) -> bool { return true; };

    return stop_matching(_actions,      target, match_cmp)
        +  stop_matching(_actionsToAdd, target, match_cmp);
}

size_t ActionManager::stopActionsForTargetWithTag(const Node *target, tag_t tag)
{
    auto pair = std::make_pair(target, tag);
    auto match_cmp = [](auto const&) -> bool { return true; };

    return stop_matching(_actions,      pair, match_cmp)
        +  stop_matching(_actionsToAdd, pair, match_cmp);
}

size_t ActionManager::stopActionsForTargetWithFlags(const Node* target, flags_t flags)
{
    auto match_cmp = [flags](auto const& a) -> bool { return flags | a->getFlags(); };

    return stop_matching(_actions,      target, match_cmp)
        +  stop_matching(_actionsToAdd, target, match_cmp);
}

void ActionManager::update(float dt)
{
    bool some_stopped = false;

#ifndef NDEBUG
    auto min_target = static_cast<decltype(std::declval<Action>().getTarget())>(nullptr);
    auto min_tag    = std::numeric_limits<decltype(std::declval<Action>().getTag())>::min();
    auto prev = std::make_pair(min_target, min_tag);
#endif

    for (auto & a : _actions)
    {
#ifndef NDEBUG
        assert( !_compare(a, prev) );
        prev = std::make_pair(a->getTarget(), a->getTag());
#endif
        some_stopped |= (a->hasStopped() || (!a->getTarget()->isPaused() && a->last_update(dt)));
    }

    if (some_stopped)
    {
        auto first_to_remove = std::remove_if(_actions.begin(), _actions.end(),
                                              [](auto & a) { return a->hasStopped(); });

        std::vector<std::unique_ptr<Action>> to_delete(std::make_move_iterator(first_to_remove),
                                                       std::make_move_iterator(_actions.end()));
        _actions.erase(first_to_remove, _actions.end());
    }

#ifndef NDEBUG
    prev = std::make_pair(min_target, min_tag);
#endif

    auto begin = _actions.begin();

    for (auto & a : _actionsToAdd)
    {
#ifndef NDEBUG
        assert( !_compare(a, prev) );
        auto curr = std::make_pair(a->getTarget(), a->getTag());
        prev = curr;
#endif
        if (!a->hasStopped())
        {
            auto ub = std::upper_bound(begin, _actions.end(), a,
                                       [](auto & a, auto & b) { return _compare(a, b); });

            assert(ub == _actions.end() || _compare(curr, *ub));

            begin = _actions.insert(ub, std::move(a));

            assert(begin == _actions.begin() || !_compare(curr, *(begin - 1)));
            assert(!_compare(*begin, curr) && !_compare(curr, *begin));

            begin++;

            assert(begin == _actions.end() || _compare(curr, *begin));
        }
        else
        {
            a.reset();
        }
    }

    _actionsToAdd.clear();

#ifndef NDEBUG
    assert(_actions.end() == std::find_if(_actions.begin(), _actions.end(),
                                          [](auto const& a) { return a->hasStopped(); }));
#endif
}

} // namespace cocos2d
