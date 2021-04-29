#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <sstream>       // std::stringstream
#include <string>        // std::string
#include <unordered_map> // std::unordered_map
#include <utility>       // std::pair
#include <vector>        // std::vector

#include <plot/I_Plotable.hpp>

template <typename N, typename E = int>
class DiGraph : public I_Plotable
{
private:
    static constexpr char def_name[] = "directional_graph";
    std::string name;

    std::unordered_map<int, std::vector<int>> adjacency_list;
    std::unordered_map<int, N> node_ids;
    std::unordered_map<int, std::pair<N, N>> edge_ids;
    std::unordered_map<int, E> edge_attrs;
    std::unordered_map<int, E> edge_attr_ids;

    std::vector<N> nodes;

    bool get_id(const N &node, int &out) const;
    bool get_id(const std::pair<N, N> &edge, int &out) const;
    bool get_id(const E &edge_attr, int &out) const;

    void add_node(const N &node, int &out_id);

public:
    DiGraph(const std::string &name = def_name);
    explicit DiGraph(const std::vector<std::pair<N, N>> &edges, const std::string &name = def_name);
    ~DiGraph() = default;

    std::vector<N> get_nodes() const;
    std::vector<std::pair<N, N>> get_edges() const;

    std::vector<N> get_successors(const N &node) const;
    std::vector<N> get_predecessors(const N &node) const;

    std::vector<N> get_root_nodes() const;
    std::vector<N> get_leaf_nodes() const;

    E get_edge_attr(const std::pair<N, N> &edge) const;

    void add_edge(const N &u, const N &v, const E &edge_attr = E{});
    void add_edges(const std::vector<std::pair<N, N>> &edges);

    DiGraph<N, E> reverse() const;

    std::string get_name() const override;
    void set_name(const std::string &name);

    std::stringstream generate_dot() const override;
    void plot(const std::string &output_loc) const;
};

#include <graph/DiGraph.tpp>

#endif // DIRGRAPH_HPP