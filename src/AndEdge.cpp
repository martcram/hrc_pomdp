#include <vector>

#include "AndEdge.hpp"
#include "Node.hpp"

AndEdge::AndEdge(const Node &parent_node, const std::vector<Node> &child_nodes, double cost)
    : parent_node{parent_node}, child_nodes{child_nodes}, cost{cost}
{
}