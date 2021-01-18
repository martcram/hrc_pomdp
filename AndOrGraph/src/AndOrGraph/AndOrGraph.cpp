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
    : AndOrGraph{}
{
    this->add_edges_from(edges);
}

AndOrGraph::AndOrGraph(const std::vector<std::vector<std::vector<std::string>>> &cutsets)
{
    for (const auto &cutset : cutsets)
    {
        Node parent_node{cutset.at(2)};
        std::vector<Node> child_nodes{Node{cutset.at(0)}, Node{cutset.at(1)}};
        AndEdge edge{parent_node, child_nodes};
        this->add_edge(edge);
    }
}

void AndOrGraph::add_edge(const AndEdge &edge)
{
    if (std::find(edges.begin(), edges.end(), edge) == edges.end())
        edges.push_back(edge);

    std::vector<Node> edge_nodes{edge.get_child_nodes()};
    edge_nodes.push_back(edge.get_parent_node());
    this->add_nodes_from(edge_nodes);
}

void AndOrGraph::add_edges_from(const std::vector<AndEdge> &edges)
{
    for (const auto &edge : edges)
        this->add_edge(edge);
}

void AndOrGraph::add_node(const Node &node)
{
    if (std::find(nodes.begin(), nodes.end(), node) == nodes.end())
        nodes.push_back(node);
}

void AndOrGraph::add_nodes_from(const std::vector<Node> &nodes)
{
    for (const auto &node : nodes)
        this->add_node(node);
}

std::vector<AndEdge> AndOrGraph::get_edges() const
{
    return edges;
}

std::vector<Node> AndOrGraph::get_nodes() const
{
    return nodes;
}