#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T>
class Graph
{
private:
    std::unordered_map<T, std::vector<T>> adjacency_list;

    void add_directed_edge(const T &u, const T &v)
    {
        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end())
            adjacency_list.emplace(std::make_pair(u, std::vector<T>{v}));
        else if (std::find(it->second.begin(), it->second.end(), v) == it->second.end())
            it->second.push_back(v);
    }

public:
    Graph()
        : adjacency_list{}
    {
    }

    explicit Graph(const std::vector<std::pair<T, T>> &edges)
        : Graph()
    {
        this->add_edges(edges);
    }

    ~Graph() = default;

    void add_edge(const T &u, const T &v)
    {
        this->add_directed_edge(u, v);
        this->add_directed_edge(v, u);
    }

    void add_edges(const std::vector<std::pair<T, T>> &edges)
    {
        for (const auto &edge : edges)
            this->add_edge(edge.first, edge.second);
    }

    std::vector<T> get_neighbors(const T &u) const
    {
        return adjacency_list.at(u);
    }
};

#endif // GRAPH_HPP