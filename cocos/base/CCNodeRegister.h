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

class NodeRegister {
private:

    using index_type     = NodeId::index_type;
    using signature_type = NodeId::signature_type;

    static constexpr signature_type FIRST_SIGNATURE = 0xAAAA;

public:

    NodeRegister() = default;

    NodeRegister(const NodeRegister &) = delete;
    NodeRegister& operator=(const NodeRegister &) = delete;

    void addNode(Node* node)
    {
        assert(node != nullptr);

        index_type     index;

        if (first_free < m_nodes.size())
        {
            index                 = first_free;
            m_nodes[ first_free ] = node;

            first_free++;

            for (const size_t size = m_nodes.size(); first_free < size && nullptr != m_nodes[first_free]; first_free++)
                ;
        }
        else if (m_nodes.size() != NodeId::MAX_NUMBER_OF_NODES)
        {
            m_nodes.push_back(node);
            index      = m_nodes.size() - 1;
            first_free = m_nodes.size();
        }
        else
        {
            throw std::runtime_error("Maximum number of nodes has been exeeded");
        }

        node->getNodeId().data = { index, next_signature++ };
    }
    
    void delNode(Node* node)
    {
        assert(node != nullptr);

        const auto index = node->getNodeId().index();

        assert(index < m_nodes.size());
        assert(m_nodes[index] != nullptr);
        assert(node->getNodeId() == m_nodes[index]->getNodeId());

        m_nodes[index] = nullptr;

        first_free = std::min(index, first_free);

        node->getNodeId() = NodeId();

        CC_ASSERT(!node->getNodeId());
    }

    Node const* getNode(NodeId id) const noexcept
    {
        const auto index = id.index();

        if (index < m_nodes.size())
        {
            if (auto node = m_nodes[index])
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

    index_type         first_free     = 0;
    signature_type     next_signature = FIRST_SIGNATURE;
    std::vector<Node*> m_nodes;
};

} // namespace cocos2d
