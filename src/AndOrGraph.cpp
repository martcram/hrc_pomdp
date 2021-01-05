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