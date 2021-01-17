#include <algorithm>
#include <vector>

#include <AndOrGraph/AndEdge.hpp>
#include <AndOrGraph/Node.hpp>

AndEdge::AndEdge(const Node &parent_node, const std::vector<Node> &child_nodes, double cost)
    : parent_node{parent_node}, child_nodes{child_nodes}, cost{cost}
{
}

bool AndEdge::operator==(const AndEdge &rhs) const
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