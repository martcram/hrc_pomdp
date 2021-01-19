#ifndef AND_OR_GRAPH_HPP
#define AND_OR_GRAPH_HPP

#include <vector>

#include <graph/AndEdge.hpp>
#include <graph/Node.hpp>

class AndOrGraph
{
private:
    std::vector<Node> nodes;
    std::vector<AndEdge> edges;
    
    void add_node(const Node &node);
    void add_nodes_from(const std::vector<Node> &nodes);

public:
    AndOrGraph();
    explicit AndOrGraph(const std::vector<AndEdge> &edges);
    explicit AndOrGraph(const std::vector<std::vector<std::vector<std::string>>> &cutsets);
    ~AndOrGraph() = default;

    void add_edge(const AndEdge &edge);
    void add_edges_from(const std::vector<AndEdge> &edges);

    std::vector<AndEdge> get_edges() const;
    std::vector<Node> get_nodes() const;
};

#endif // AND_OR_GRAPH_HPP