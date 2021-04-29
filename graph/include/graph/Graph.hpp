#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <sstream>       // std::stringstream
#include <string>        // std::string
#include <unordered_map> // std::unordered_map
#include <utility>       // std::pair
#include <vector>        // std::vector

#include <plot/I_Plotable.hpp>

template <typename T>
class Graph : public I_Plotable
{
private:
    static constexpr char def_name[] = "undirectional_graph";
    std::string name;

    std::unordered_map<int, std::vector<int>> adjacency_list;
    std::unordered_map<int, T> node_ids;

    std::vector<T> nodes;
    std::vector<std::pair<T, T>> edges;

    bool get_id(const T &node, int &out) const;

    void add_node(const T &node, int &out_id);

public:
    Graph(const std::string &name = def_name);
    explicit Graph(const std::vector<std::pair<T, T>> &edges, const std::string &name = def_name);
    ~Graph() = default;

    std::vector<T> get_nodes() const;
    std::vector<std::pair<T, T>> get_edges() const;

    std::vector<T> get_neighbors(const T &node) const;

    void add_edge(const T &u, const T &v);
    void add_edges(const std::vector<std::pair<T, T>> &edges);

    std::string get_name() const override;
    void set_name(const std::string &name);
    
    std::stringstream generate_dot() const override;
    void plot(const std::string &output_loc) const;
};

#include <graph/Graph.tpp>

#endif // GRAPH_HPP