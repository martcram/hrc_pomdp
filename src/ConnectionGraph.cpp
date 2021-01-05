#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "ConnectionGraph.hpp"

ConnectionGraph::ConnectionGraph()
    : vertices{}
{
}

ConnectionGraph::ConnectionGraph(const std::vector<std::pair<std::string, std::string>> &edges)
    : vertices{}
{
    this->add_edges_from(edges);
}

void ConnectionGraph::add_directed_edge(const std::pair<std::string, std::string> &edge)
{
    if (vertices.find(edge.first) == vertices.end())
    {
        vertices.insert({edge.first, std::vector<std::string>{edge.second}});
    }
    else
    {
        std::vector<std::string> adjacents = vertices.at(edge.first);
        if (std::find(adjacents.begin(), adjacents.end(), edge.second) == adjacents.end())
            vertices.at(edge.first).push_back(edge.second);
    }
}

void ConnectionGraph::add_edge(const std::pair<std::string, std::string> &edge)
{
    this->add_directed_edge(edge);
    this->add_directed_edge(std::pair<std::string, std::string>(edge.second, edge.first));
}

void ConnectionGraph::add_edges_from(const std::vector<std::pair<std::string, std::string>> &edges)
{
    for (const auto &edge : edges)
    {
        this->add_edge(edge);
    }
}
