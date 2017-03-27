/*
 * Copyright © 2017 Iakov Sergeev <yahont@github>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace cocos2d {

class NodeRegister;

class NodeId {

    friend class NodeRegister;

    using index_type     = uint16_t;
    using signature_type = uint16_t;

    bool operator==(NodeId const& r) const
    {
        return index == r.index && signature == r.signature;
    }

    index_type     index;
    signature_type signature;
};

class Node {
public:
    const NodeId & getNodeId() const { return nodeId; }
    NodeId & getNodeId() { return nodeId; }

private:
    NodeId nodeId;
};

class NodeRegister {
private:

    using index_type     = NodeId::index_type;
    using signature_type = NodeId::signature_type;

    static constexpr index_type MAX_NUMBER_OF_NODES = static_cast<index_type>(-1);
    static constexpr index_type INVALID_INDEX       = static_cast<index_type>(-1);

    static constexpr signature_type FIRST_SIGNATURE = 0xAAAA;

    static_assert(0u < MAX_NUMBER_OF_NODES, "MAX_NUMBER_OF_NODES is not positive");
    static_assert(0u < static_cast<signature_type>(-1), "signature_type must be unsigned");

public:

    NodeRegister() = default;

    NodeRegister(const NodeRegister &) = delete;
    NodeRegister& operator=(const NodeRegister &) = delete;

    void addNode(Node* node)
    {
        assert(node != nullptr);

        auto & id = node->getNodeId();

        if (first_free != INVALID_INDEX)
        {
            id.index = first_free;
            m_nodeHandlers[ id.index ].node      = node;
            m_nodeHandlers[ id.index ].prev_free = INVALID_INDEX;
            first_free = m_nodeHandlers[ id.index ].next_free;
            m_nodeHandlers[ id.index ].next_free = INVALID_INDEX;
        }
        else if (m_nodeHandlers.size() != MAX_NUMBER_OF_NODES)
        {
            m_nodeHandlers.emplace_back(node, index_type(INVALID_INDEX), index_type(INVALID_INDEX));
            id.index = m_nodeHandlers.size() - 1;
        }
        else
        {
            throw std::runtime_error("Maximum number of nodes has been exeeded");
        }

        id.signature = next_signature++;
    }
    
    void delNode(Node* node)
    {
        assert(node != nullptr);

        auto & id = node->getNodeId();
        const auto size = m_nodeHandlers.size();

        assert(id.index < size);
        assert(m_nodeHandlers[id.index].node != nullptr);
        assert(id == m_nodeHandlers[id.index].node->getNodeId());

        m_nodeHandlers[id.index].node = nullptr;

        if (first_free == INVALID_INDEX)
        {
            first_free = id.index;
            return;
        }

        if (id.index < first_free)
        {
            m_nodeHandlers[ id.index ].next_free = first_free;
            first_free = id.index;
            return;
        }

        auto prev_free = first_free;

        while (m_nodeHandlers[prev_free].next_free < id.index)
            prev_free = m_nodeHandlers[prev_free].next_free;

        m_nodeHandlers[ id.index ].next_free  = m_nodeHandlers[ prev_free ].next_free;
        m_nodeHandlers[ id.index ].prev_free  = prev_free;
        m_nodeHandlers[ prev_free ].next_free = id.index;
    }

    Node const* getNode(NodeId id) const noexcept
    {
        if (id.index < m_nodeHandlers.size())
        {
            if (auto node = m_nodeHandlers[id.index].node)
            {
                if (node->getNodeId() == id)
                {
                    return node;
                }
            }
        }

        return nullptr;
    }

    Node* getNode(NodeId id) noexcept
    {
        return const_cast<Node*>(const_cast<const NodeRegister*>(this)->getNode(id));
    }

    template<typename T>
        T const* get(NodeId id) const
        {
            static_assert(std::is_base_of<Node, T>::value,
                          "Node cannot be converted into T");
            return dynamic_cast<const T*>(getNode(id));
        }

    template<typename T>
        T* get(NodeId id)
        {
            return const_cast<T*>(const_cast<const NodeRegister*>(this)->get<T>(id));
        }

private:

    struct NodeHandler {
        NodeHandler(Node* node, index_type prev_free, index_type next_free)
            : node(node)
            , prev_free(prev_free)
            , next_free(next_free)
            {}

        Node*      node      = nullptr;
        index_type prev_free = INVALID_INDEX;
        index_type next_free = INVALID_INDEX;
    };

private:
    index_type               first_free     = INVALID_INDEX;
    signature_type           next_signature = FIRST_SIGNATURE;
    std::vector<NodeHandler> m_nodeHandlers;
};

} // namespace cocos2d
