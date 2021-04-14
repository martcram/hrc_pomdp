#include <algorithm> // std::copy_if, std::find, std::transform
#include <sstream>   // std::stringstream
#include <unordered_map>
#include <utility>   // std::back_inserter, std::pair
#include <vector>

#include <graph/I_Plotable.hpp>

template <typename T>
DiGraph<T>::DiGraph()
    : adjacency_list{}, node_indices{}
{
}

template <typename T>
DiGraph<T>::DiGraph(const std::vector<std::pair<T, T>> &edges)
    : DiGraph<T>{}
{
    this->add_edges(edges);
}

template <typename T>
void DiGraph<T>::add_node(const T &node)
{
    if (node_indices.find(node) == node_indices.end())
        node_indices.emplace(std::make_pair(node, node_indices.size()));
}

template <typename T>
void DiGraph<T>::add_edge(const T &u, const T &v)
{
    auto it = adjacency_list.find(u);
    if (it == adjacency_list.end())
        adjacency_list.emplace(std::make_pair(u, std::vector<T>{v}));
    else if (std::find(it->second.begin(), it->second.end(), v) == it->second.end())
        it->second.push_back(v);

    this->add_node(u);
    this->add_node(v);
}

template <typename T>
void DiGraph<T>::add_edges(const std::vector<std::pair<T, T>> &edges)
{
    for (const auto &edge : edges)
        this->add_edge(edge.first, edge.second);
}

template <typename T>
DiGraph<T> DiGraph<T>::reverse() const
{
    DiGraph<T> reversed_graph{};
    for (const auto &it : adjacency_list)
    {
        for (const auto &node : it.second)
            reversed_graph.add_edge(node, it.first);
    }
    return reversed_graph;
}

template <typename T>
std::vector<T> DiGraph<T>::get_successors(const T &u) const
{
    auto it = adjacency_list.find(u);
    if (it == adjacency_list.end())
        return std::vector<T>{};
    else
        return it->second;
}

template <typename T>
std::vector<T> DiGraph<T>::get_predecessors(const T &u) const
{
    std::vector<T> predecessors{};
    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); ++it)
    {
        std::vector<T> successors{this->get_successors(it->first)};
        if (std::find(successors.begin(), successors.end(), u) != successors.end())
            predecessors.push_back(it->first);
    }
    return predecessors;
}

template <typename T>
std::vector<T> DiGraph<T>::get_nodes() const
{
    std::vector<T> nodes{};
    std::transform(node_indices.begin(), node_indices.end(), std::back_inserter(nodes),
                   [](const auto &node_index_map) { return node_index_map.first; });
    return nodes;
}

template <typename T>
std::vector<std::pair<T, T>> DiGraph<T>::get_edges() const
{
    std::vector<std::pair<T, T>> edges{};
    for (auto i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
    {
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
            edges.emplace_back(i->first, *j);
    }
    return edges;
}

template <typename T>
std::vector<T> DiGraph<T>::get_root_nodes() const
{
    std::vector<T> root_nodes{};
    std::vector<T> nodes = this->get_nodes();
    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(root_nodes),
                 [this](const auto &node) {
                     return this->get_predecessors(node).empty();
                 });
    return root_nodes;
}

template <typename T>
std::stringstream DiGraph<T>::plot() const
{
    std::stringstream ss{};
    for (const auto &edge : this->get_edges())
        ss << node_indices.at(edge.first) << "->" << node_indices.at(edge.second)
           << '\n';
    return ss;
}