#include <algorithm> // std::find
#include <vector>

#include <graph/Node.hpp>

template <typename T>
AndEdge<T>::AndEdge(const Node<T> &parent_node, const std::vector<Node<T>> &child_nodes, double cost)
    : parent_node{parent_node}, child_nodes{child_nodes}, cost{cost}
{
}

template <typename T>
bool AndEdge<T>::operator==(const AndEdge &rhs) const
{
    for (const auto &node : this->child_nodes)
    {
        if (std::find(rhs.child_nodes.begin(), rhs.child_nodes.end(), node) == rhs.child_nodes.end())
        {
            return false;
        }
    }
    return (this->parent_node == rhs.parent_node);
}

template <typename T>
Node<T> AndEdge<T>::get_parent_node() const
{
    return parent_node;
}

template <typename T>
std::vector<Node<T>> AndEdge<T>::get_child_nodes() const
{
    return child_nodes;
}