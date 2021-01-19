#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T>
class DiGraph
{
private:
    std::unordered_map<T, std::vector<T>> adjacency_list;

public:
    DiGraph()
        : adjacency_list{}
    {
    }

    explicit DiGraph(const std::vector<std::pair<T, T>> &edges)
        : DiGraph()
    {
    }

    ~DiGraph() = default;

    void add_edge(const T &u, const T &v)
    {
        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end())
            adjacency_list.emplace(std::make_pair(u, std::vector<T>{v}));
        else if (std::find(it->second.begin(), it->second.end(), v) == it->second.end())
            it->second.push_back(v);
    }

    void add_edges(const std::vector<std::pair<T, T>> &edges)
    {
        for (const auto &edge : edges)
            this->add_edge(edge.first, edge.second);
    }
};

#endif // DIGRAPH_HPP