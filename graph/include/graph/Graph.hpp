#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <sstream> // std::stringstream
#include <unordered_map>
#include <utility> // std::pair
#include <vector>

#include <graph/I_Plotable.hpp>

template <typename T>
class Graph : public I_Plotable
{
private:
    std::unordered_map<T, std::vector<T>> adjacency_list;
    std::unordered_map<T, int> node_indices;

    void add_directed_edge(const T &u, const T &v);
    void add_node(const T &node);

public:
    Graph();
    explicit Graph(const std::vector<std::pair<T, T>> &edges);
    ~Graph() = default;

    void add_edge(const T &u, const T &v);
    void add_edges(const std::vector<std::pair<T, T>> &edges);

    std::vector<T> get_neighbors(const T &u) const;
    std::vector<T> get_nodes() const;
    std::vector<std::pair<T, T>> get_edges() const;

    virtual std::stringstream plot() const override;
};

#include <graph/Graph.tpp>

#endif // GRAPH_HPP