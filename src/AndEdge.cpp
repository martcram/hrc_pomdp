#include <vector>

#include "AndEdge.hpp"
#include "Node.hpp"

AndEdge::AndEdge(const std::vector<Node> &child_nodes, double cost)
    : child_nodes{child_nodes}, cost{cost}
{
}