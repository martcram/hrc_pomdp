#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>

template <typename T>
class Graph
{
private:
    std::unordered_map<T, std::vector<T>> adjacency_list;

public:
    Graph();
    ~Graph() = default;
};

#endif // GRAPH_HPP