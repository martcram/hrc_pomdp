#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include <graph/I_Plotable.hpp>

template <typename T>
class DiGraph : public I_Plotable
{
protected:
    std::unordered_map<T, std::vector<T>> adjacency_list;
    std::unordered_map<T, int> node_indices;

    void add_node(const T &node)
    {
        if (node_indices.find(node) == node_indices.end())
            node_indices.emplace(std::make_pair(node, node_indices.size()));
    }

public:
    DiGraph()
        : adjacency_list{}, node_indices{}
    {
    }

    explicit DiGraph(const std::vector<std::pair<T, T>> &edges)
        : DiGraph<T>{}
    {
        this->add_edges(edges);
    }

    ~DiGraph() = default;

    void add_edge(const T &u, const T &v)
    {
        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end())
            adjacency_list.emplace(std::make_pair(u, std::vector<T>{v}));
        else if (std::find(it->second.begin(), it->second.end(), v) == it->second.end())
            it->second.push_back(v);

        this->add_node(u);
        this->add_node(v);
    }

    void add_edges(const std::vector<std::pair<T, T>> &edges)
    {
        for (const auto &edge : edges)
            this->add_edge(edge.first, edge.second);
    }

    DiGraph reverse() const
    {
        DiGraph reversed_graph{};
        for (const auto &it : adjacency_list)
        {
            for (const auto &node : it.second)
                reversed_graph.add_edge(node, it.first);
        }
        return reversed_graph;
    }

    std::vector<T> get_successors(const T &u) const
    {
        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end())
            return std::vector<T>{};
        else
            return it->second;
    }

    std::vector<T> get_nodes() const
    {
        std::vector<T> nodes{};
        std::transform(node_indices.begin(), node_indices.end(), std::back_inserter(nodes),
                       [](const auto &node_index_map) { return node_index_map.first; });
        return nodes;
    }

    std::vector<std::pair<T, T>> get_edges() const
    {
        std::vector<std::pair<T, T>> edges{};
        for (auto i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
        {
            for (auto j = i->second.begin(); j != i->second.end(); ++j)
                edges.emplace_back(i->first, *j);
        }
        return edges;
    }

    virtual std::stringstream plot() const override
    {
        std::stringstream ss{};
        for (const auto &edge : this->get_edges())
            ss << node_indices.at(edge.first) << "->" << node_indices.at(edge.second)
               << '\n';
        return ss;
    }
};

#endif // DIGRAPH_HPP