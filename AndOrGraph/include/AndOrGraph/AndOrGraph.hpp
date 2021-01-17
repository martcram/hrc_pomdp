#ifndef AND_OR_GRAPH_HPP
#define AND_OR_GRAPH_HPP

#include <vector>

#include <AndOrGraph/AndEdge.hpp>
#include <AndOrGraph/Node.hpp>

class AndOrGraph
{
private:
    std::vector<Node> nodes;
    std::vector<AndEdge> edges;

public:
    AndOrGraph();
    explicit AndOrGraph(const std::vector<AndEdge> &edges);
    ~AndOrGraph() = default;

    void add_edge(const AndEdge &edge);
    void add_edges_from(const std::vector<AndEdge> &edges);

    std::vector<AndEdge> get_edges() const;
    std::vector<Node> get_nodes() const;
};

#endif // AND_OR_GRAPH_HPP