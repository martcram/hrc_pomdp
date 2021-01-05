#include <algorithm>
#include <vector>

#include "AndEdge.hpp"
#include "AndOrGraph.hpp"
#include "Node.hpp"

AndOrGraph::AndOrGraph()
    : nodes{}, edges{}
{
}

AndOrGraph::AndOrGraph(const std::vector<Node> &nodes, const std::vector<AndEdge> &edges)
    : nodes{nodes}, edges{edges}
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