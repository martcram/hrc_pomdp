#ifndef AND_OR_GRAPH_HPP
#define AND_OR_GRAPH_HPP

#include <unordered_map>
#include <vector>

#include <graph/AndEdge.hpp>
#include <graph/Node.hpp>

class AndOrGraph
{
private:
    std::vector<Node> nodes;
    std::vector<AndEdge> edges;
    
    std::unordered_map<Node, std::vector<AndEdge>, NodeHasher> incoming_edges{};
    std::unordered_map<Node, std::vector<AndEdge>, NodeHasher> outgoing_edges{};

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
    std::vector<AndEdge> get_incoming_edges(const Node &node) const;
    std::vector<AndEdge> get_outgoing_edges(const Node &node) const;
};

#endif // AND_OR_GRAPH_HPP