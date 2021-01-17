#include <algorithm>
#include <vector>

#include <AndOrGraph/AndEdge.hpp>
#include <AndOrGraph/AndOrGraph.hpp>
#include <AndOrGraph/Node.hpp>

AndOrGraph::AndOrGraph()
    : nodes{}, edges{}
{
}

AndOrGraph::AndOrGraph(const std::vector<AndEdge> &edges)
    : nodes{}, edges{edges}
{
}

void AndOrGraph::add_edge(const AndEdge &edge)
{
    if (std::find(edges.begin(), edges.end(), edge) == edges.end())
        edges.push_back(edge);
}

void AndOrGraph::add_edges_from(const std::vector<AndEdge> &edges)
{
    for (const auto &edge : edges)
        this->add_edge(edge);
}

std::vector<AndEdge> AndOrGraph::get_edges() const
{
    return edges;
}

std::vector<Node> AndOrGraph::get_nodes() const
{
    return nodes;
}