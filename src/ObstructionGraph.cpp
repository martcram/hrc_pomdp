#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "ObstructionGraph.hpp"

ObstructionGraph::ObstructionGraph()
    : adjacency_list{}
{
}

ObstructionGraph::ObstructionGraph(const std::vector<std::pair<std::string, std::string>> &edges)
    : adjacency_list{}
{
    this->add_edges_from(edges);
}

void ObstructionGraph::add_edge(const std::pair<std::string, std::string> &edge)
{
    if (adjacency_list.find(edge.first) == adjacency_list.end())
        adjacency_list.insert({edge.first, std::vector<std::string>{edge.second}});
    else
    {
        std::vector<std::string> adjacents = adjacency_list.at(edge.first);
        if (std::find(adjacents.begin(), adjacents.end(), edge.second) == adjacents.end())
            adjacency_list.at(edge.first).push_back(edge.second);
    }
}

void ObstructionGraph::add_edges_from(const std::vector<std::pair<std::string, std::string>> &edges)
{
    for (const auto &edge : edges)
    {
        this->add_edge(edge);
    }
}

ObstructionGraph ObstructionGraph::reverse()
{
    ObstructionGraph reversed_graph{};

    for (const auto &it : adjacency_list)
    {
        for (const auto &vertex : it.second)
            reversed_graph.add_edge(std::pair<std::string, std::string>(vertex, it.first));
    }

    return reversed_graph;
}