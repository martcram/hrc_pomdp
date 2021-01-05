#ifndef AND_OR_GRAPH_HPP
#define AND_OR_GRAPH_HPP

#include <vector>

#include "AndEdge.hpp"
#include "Node.hpp"

class AndOrGraph
{
private:
    std::vector<Node> nodes;
    std::vector<AndEdge> edges;

public:
    AndOrGraph();
    AndOrGraph(const std::vector<Node> &nodes, const std::vector<AndEdge> &edges);
    ~AndOrGraph() = default;
};

#endif // AND_OR_GRAPH_HPP