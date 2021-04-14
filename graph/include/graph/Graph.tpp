#include <algorithm> // std::find, std::transform
#include <sstream>   // std::stringstream
#include <unordered_map>
#include <utility>   // std::back_inserter, std::pair
#include <vector>

#include <graph/I_Plotable.hpp>

template <typename T>
Graph<T>::Graph()
    : adjacency_list{}, node_indices{}
{
}

template <typename T>
Graph<T>::Graph(const std::vector<std::pair<T, T>> &edges)
    : Graph()
{
    this->add_edges(edges);
}

template <typename T>
void Graph<T>::add_directed_edge(const T &u, const T &v)
{
    auto it = adjacency_list.find(u);
    if (it == adjacency_list.end())
        adjacency_list.emplace(std::make_pair(u, std::vector<T>{v}));
    else if (std::find(it->second.begin(), it->second.end(), v) == it->second.end())
        it->second.push_back(v);
}

template <typename T>
void Graph<T>::add_node(const T &node)
{
    if (node_indices.find(node) == node_indices.end())
        node_indices.emplace(std::make_pair(node, node_indices.size()));
}

template <typename T>
void Graph<T>::add_edge(const T &u, const T &v)
{
    this->add_directed_edge(u, v);
    this->add_directed_edge(v, u);

    this->add_node(u);
    this->add_node(v);
}

template <typename T>
void Graph<T>::add_edges(const std::vector<std::pair<T, T>> &edges)
{
    for (const auto &edge : edges)
        this->add_edge(edge.first, edge.second);
}

template <typename T>
std::vector<T> Graph<T>::get_neighbors(const T &u) const
{
    auto it = adjacency_list.find(u);
    if (it == adjacency_list.end())
        return std::vector<T>{};
    else
        return it->second;
}

template <typename T>
std::vector<T> Graph<T>::get_nodes() const
{
    std::vector<T> nodes{};
    std::transform(node_indices.begin(), node_indices.end(), std::back_inserter(nodes),
                   [](const auto &node_index_map) { return node_index_map.first; });
    return nodes;
}

template <typename T>
std::vector<std::pair<T, T>> Graph<T>::get_edges() const
{
    std::vector<std::pair<T, T>> edges{};
    for (auto i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
    {
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            std::pair<T, T> edge = ((i->first < *j) ? std::make_pair(i->first, *j) : std::make_pair(*j, i->first));
            if (std::find(edges.begin(), edges.end(), edge) == edges.end())
                edges.push_back(edge);
        }
    }
    return edges;
}

template <typename T>
std::stringstream Graph<T>::plot() const
{
    std::stringstream ss{};
    for (const auto &edge : this->get_edges())
        ss << node_indices.at(edge.first) << "->" << node_indices.at(edge.second)
           << '\n';
    return ss;
}