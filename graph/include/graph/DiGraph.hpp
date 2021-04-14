#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <sstream> // std::stringstream
#include <unordered_map>
#include <utility> // std::pair
#include <vector>

#include <graph/I_Plotable.hpp>

template <typename T>
class DiGraph : public I_Plotable
{
protected:
    std::unordered_map<T, std::vector<T>> adjacency_list;
    std::unordered_map<T, int> node_indices;

    void add_node(const T &node);

public:
    DiGraph();
    explicit DiGraph(const std::vector<std::pair<T, T>> &edges);
    ~DiGraph() = default;

    void add_edge(const T &u, const T &v);
    void add_edges(const std::vector<std::pair<T, T>> &edges);

    DiGraph reverse() const;

    std::vector<T> get_successors(const T &u) const;
    std::vector<T> get_predecessors(const T &u) const;
    std::vector<T> get_nodes() const;
    std::vector<std::pair<T, T>> get_edges() const;
    std::vector<T> get_root_nodes() const;

    virtual std::stringstream plot() const override;
};

#include <graph/DiGraph.tpp>

#endif // DIGRAPH_HPP