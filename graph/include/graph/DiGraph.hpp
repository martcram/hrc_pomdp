#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

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
};

#endif // DIGRAPH_HPP