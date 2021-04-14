#ifndef AND_OR_GRAPH_HPP
#define AND_OR_GRAPH_HPP

#include <unordered_map>
#include <utility> // std::pair
#include <vector>

#include <graph/AndEdge.hpp>
#include <graph/Node.hpp>

template <typename T>
class AndOrGraph
{
private:
    std::vector<Node<T>> nodes;
    std::vector<AndEdge<T>> edges;

    std::unordered_map<Node<T>, std::vector<AndEdge<T>>> incoming_edges{};
    std::unordered_map<Node<T>, std::vector<AndEdge<T>>> outgoing_edges{};

    void _add_node(const Node<T> &node);
    void _add_nodes_from(const std::vector<Node<T>> &nodes);
    void _add_edge(const AndEdge<T> &edge);

    Node<T> _get_node(const T &data) const;
    std::vector<AndEdge<T>> _get_incoming_edges(const Node<T> &node) const;
    std::vector<AndEdge<T>> _get_outgoing_edges(const Node<T> &node) const;
    std::vector<std::vector<Node<T>>> _get_successors(const Node<T> &node) const;
    std::vector<Node<T>> _get_root_nodes() const;
    std::vector<Node<T>> _get_leaf_nodes() const;

public:
    AndOrGraph();
    explicit AndOrGraph(const std::vector<std::pair<T, std::vector<T>>> &edges);
    ~AndOrGraph() = default;

    void add_edge(const T &parent_node, const std::vector<T> &child_nodes);
    void add_edges_from(const std::vector<std::pair<T, std::vector<T>>> &edges);

    std::vector<T> get_nodes() const;
    std::vector<std::pair<T, std::vector<T>>> get_edges() const;
    std::vector<std::vector<T>> get_successors(const T &data) const;
    std::vector<T> get_root_nodes() const;
    std::vector<T> get_leaf_nodes() const;
};

#include <graph/AndOrGraph.tpp>

#endif // AND_OR_GRAPH_HPP