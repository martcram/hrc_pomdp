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
        return adjacency_list.at(u);
    }

    std::vector<T> get_nodes() const
    {
        std::vector<T> nodes{};
        for (auto i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
        {
            nodes.push_back(i->first);
            nodes.insert(nodes.end(), i->second.begin(), i->second.end());
        }
        std::sort(nodes.begin(), nodes.end());
        nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
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
};

#endif // DIGRAPH_HPP