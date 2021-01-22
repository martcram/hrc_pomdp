#include <algorithm>
#include <vector>

#include <graph/AndEdge.hpp>
#include <graph/AndOrGraph.hpp>
#include <graph/Node.hpp>

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
    // Add edge to edges
    if (std::find(edges.begin(), edges.end(), edge) == edges.end())
        edges.push_back(edge);

    // Add edge nodes to nodes
    std::vector<Node> edge_nodes{edge.get_child_nodes()};
    edge_nodes.push_back(edge.get_parent_node());
    this->add_nodes_from(edge_nodes);

    // Add edge to parent node's outgoing edges.
    Node parent_node{edge.get_parent_node()};
    auto it = outgoing_edges.find(parent_node);
    if (it == outgoing_edges.end())
        outgoing_edges.insert({parent_node, std::vector<AndEdge>{edge}});
    else
        it->second.push_back(edge);

    // Add edge to child nodes' incoming edges.
    for (const auto &child_node : edge.get_child_nodes())
    {
        auto it = incoming_edges.find(child_node);
        if (it == incoming_edges.end())
            incoming_edges.insert({child_node, std::vector<AndEdge>{edge}});
        else
            it->second.push_back(edge);
    }
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

std::vector<AndEdge> AndOrGraph::get_incoming_edges(const Node &node) const
{
    auto it = incoming_edges.find(node);
    if (it == incoming_edges.end())
        return std::vector<AndEdge>{};
    else
        return it->second;
}

std::vector<AndEdge> AndOrGraph::get_outgoing_edges(const Node &node) const
{
    auto it = outgoing_edges.find(node);
    if (it == outgoing_edges.end())
        return std::vector<AndEdge>{};
    else
        return it->second;
}