#ifndef AND_EDGE_HPP
#define AND_EDGE_HPP

#include <vector>
#include "Node.hpp"

class AndEdge
{
private:
    static constexpr double def_cost = 0.0;

    Node parent_node;
    std::vector<Node> child_nodes;
    double cost;

public:
    explicit AndEdge(const Node &parent_node, const std::vector<Node> &child_nodes, double cost = def_cost);
    ~AndEdge() = default;

    bool operator==(const AndEdge &rhs) const;
};

#endif // AND_EDGE_HPP