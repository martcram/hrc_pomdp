#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <AndOrGraph/ConnectionGraph.hpp>

ConnectionGraph::ConnectionGraph()
    : adjacency_list{}
{
}

ConnectionGraph::ConnectionGraph(const std::vector<std::pair<std::string, std::string>> &edges)
    : adjacency_list{}
{
    this->add_edges_from(edges);
}

void ConnectionGraph::add_directed_edge(const std::pair<std::string, std::string> &edge)
{
    if (adjacency_list.find(edge.first) == adjacency_list.end())
    {
        adjacency_list.insert({edge.first, std::vector<std::string>{edge.second}});
    }
    else
    {
        std::vector<std::string> adjacents = adjacency_list.at(edge.first);
        if (std::find(adjacents.begin(), adjacents.end(), edge.second) == adjacents.end())
            adjacency_list.at(edge.first).push_back(edge.second);
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

std::vector<std::string> ConnectionGraph::get_nodes() const
{
    std::vector<std::string> nodes{};
    for (const auto &it : adjacency_list)
    {
        nodes.push_back(it.first);
    }

    return nodes;
}

std::vector<std::string> ConnectionGraph::get_neighbors(const std::string &node) const
{
    return adjacency_list.at(node);
}

std::vector<std::vector<std::string>> ConnectionGraph::get_edges() const
{
    std::vector<std::vector<std::string>> edges{};
    for (auto i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
    {
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            std::vector<std::string> edge{i->first, *j};
            std::sort(edge.begin(), edge.end());
            if (std::find(edges.begin(), edges.end(), edge) == edges.end())
                edges.push_back(edge);
        }
    }
    return edges;
}