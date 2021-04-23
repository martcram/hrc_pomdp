#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map> // std::unordered_map
#include <utility>       // std::pair
#include <vector>        // std::vector

template <typename T>
class Graph
{
private:
    std::unordered_map<int, std::vector<int>> adjacency_list;
    std::unordered_map<int, T> node_ids;

    std::vector<T> nodes;
    std::vector<std::pair<T, T>> edges;

    bool get_id(const T &node, int &out) const;

    void add_node(const T &node, int &out_id);

public:
    Graph();
    explicit Graph(const std::vector<std::pair<T, T>> &edges);
    ~Graph() = default;

    std::vector<T> get_nodes() const;
    std::vector<std::pair<T, T>> get_edges() const;

    std::vector<T> get_neighbors(const T &node) const;

    void add_edge(const T &u, const T &v);
    void add_edges(const std::vector<std::pair<T, T>> &edges);
};

#include <graph/Graph.tpp>

#endif // GRAPH_HPP