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

    void add_edge(const AndEdge &edge);
    void add_edges_from(const std::vector<AndEdge> &edges);
};

#endif // AND_OR_GRAPH_HPP