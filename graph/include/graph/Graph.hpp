#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <utility>
#include <vector>

template <typename T>
class Graph
{
private:
    std::unordered_map<T, std::vector<T>> adjacency_list;

public:
    Graph()
        : adjacency_list{}
    {
    }

    explicit Graph(const std::vector<std::pair<T, T>> &edges)
        : adjacency_list{}
    {
    }

    ~Graph() = default;
};

#endif // GRAPH_HPP