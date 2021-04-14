#include <algorithm> // std::copy_if, std::find_if, std::for_each, std::transform
#include <iterator>  // std::back_inserter
#include <unordered_map>
#include <utility> // std::pair
#include <vector>

#include <graph/AndEdge.hpp>
#include <graph/Node.hpp>

template <typename T>
AndOrGraph<T>::AndOrGraph()
    : nodes{}, edges{}
{
}

template <typename T>
AndOrGraph<T>::AndOrGraph(const std::vector<std::pair<T, std::vector<T>>> &edges)
    : AndOrGraph<T>{}
{
    this->add_edges_from(edges);
}

template <typename T>
void AndOrGraph<T>::_add_node(const Node<T> &node)
{
    if (std::find(nodes.begin(), nodes.end(), node) == nodes.end())
        nodes.push_back(node);
}

template <typename T>
void AndOrGraph<T>::_add_nodes_from(const std::vector<Node<T>> &nodes)
{
    for (const auto &node : nodes)
        this->_add_node(node);
}

template <typename T>
void AndOrGraph<T>::_add_edge(const AndEdge<T> &edge)
{
    // Add edge to edges
    if (std::find(edges.begin(), edges.end(), edge) == edges.end())
        edges.push_back(edge);

    // Add edge nodes to nodes
    std::vector<Node<T>> edge_nodes{edge.get_child_nodes()};
    edge_nodes.push_back(edge.get_parent_node());
    this->_add_nodes_from(edge_nodes);

    // Add edge to parent node's outgoing edges.
    Node<T> parent_node{edge.get_parent_node()};
    auto i = outgoing_edges.find(parent_node);
    if (i == outgoing_edges.end())
        outgoing_edges.insert({parent_node, std::vector<AndEdge<T>>{edge}});
    else
        i->second.push_back(edge);

    // Add edge to child nodes' incoming edges.
    for (const auto &child_node : edge.get_child_nodes())
    {
        auto j = incoming_edges.find(child_node);
        if (j == incoming_edges.end())
            incoming_edges.insert({child_node, std::vector<AndEdge<T>>{edge}});
        else
            j->second.push_back(edge);
    }
}

template <typename T>
Node<T> AndOrGraph<T>::_get_node(const T &data) const
{
    auto it = std::find_if(nodes.begin(), nodes.end(), [&data](const Node<T> &node) { return (data == node.get_data()); });
    return (it == nodes.end() ? Node<T>{T{}} : *it);
}

template <typename T>
std::vector<AndEdge<T>> AndOrGraph<T>::_get_incoming_edges(const Node<T> &node) const
{
    auto it = incoming_edges.find(node);
    return (it == incoming_edges.end() ? std::vector<AndEdge<T>>{} : it->second);
}

template <typename T>
std::vector<AndEdge<T>> AndOrGraph<T>::_get_outgoing_edges(const Node<T> &node) const
{
    auto it = outgoing_edges.find(node);
    return (it == outgoing_edges.end() ? std::vector<AndEdge<T>>{} : it->second);
}

template <typename T>
std::vector<std::vector<Node<T>>> AndOrGraph<T>::_get_successors(const Node<T> &node) const
{
    std::vector<std::vector<Node<T>>> successors{};
    for (const AndEdge<T> &outgoing_edge : this->_get_outgoing_edges(node))
    {
        successors.push_back(outgoing_edge.get_child_nodes());
    }
    return successors;
}

template <typename T>
std::vector<Node<T>> AndOrGraph<T>::_get_root_nodes() const
{
    std::vector<Node<T>> root_nodes{};
    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(root_nodes),
                 [this](const auto &node) { return this->_get_incoming_edges(node).empty(); });
    return root_nodes;
}

template <typename T>
std::vector<Node<T>> AndOrGraph<T>::_get_leaf_nodes() const
{
    std::vector<Node<T>> leaf_nodes{};
    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(leaf_nodes),
                 [this](const auto &node) { return this->_get_outgoing_edges(node).empty(); });
    return leaf_nodes;
}

template <typename T>
void AndOrGraph<T>::add_edge(const T &parent_node, const std::vector<T> &child_nodes)
{
    Node<T> p_node{parent_node};
    std::vector<Node<T>> ch_nodes{};
    std::transform(child_nodes.begin(), child_nodes.end(), std::back_inserter(ch_nodes), [](const T &child_node) { return Node<T>{child_node}; });
    this->_add_edge(AndEdge<T>{p_node, ch_nodes});
}

template <typename T>
void AndOrGraph<T>::add_edges_from(const std::vector<std::pair<T, std::vector<T>>> &edges)
{
    std::for_each(edges.begin(), edges.end(), [this](const std::pair<T, std::vector<T>> &edge) { this->add_edge(edge.first, edge.second); });
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_nodes() const
{
    std::vector<T> nodes_data{};
    std::transform(nodes.begin(), nodes.end(), std::back_inserter(nodes_data),
                   [](const Node<T> &node) { return node.get_data(); });
    return nodes_data;
}

template <typename T>
std::vector<std::pair<T, std::vector<T>>> AndOrGraph<T>::get_edges() const
{
    std::vector<std::pair<T, std::vector<T>>> edges_data{};
    std::transform(edges.begin(), edges.end(), std::back_inserter(edges_data),
                   [](const AndEdge<T> &edge) {
                       T parent_node{edge.get_parent_node().get_data()};
                       
                       std::vector<T> child_nodes{};
                       for (const Node<T> &child_node : edge.get_child_nodes())
                           child_nodes.push_back(child_node.get_data());

                       return std::make_pair(parent_node, child_nodes);
                   });
    return edges_data;
}

template <typename T>
std::vector<std::vector<T>> AndOrGraph<T>::get_successors(const T &data) const
{
    std::vector<std::vector<Node<T>>> successors{this->_get_successors(this->_get_node(data))};
    std::vector<std::vector<T>> successors_data{};

    std::transform(successors.begin(), successors.end(), std::back_inserter(successors_data),
                   [](std::vector<Node<T>> vec_successors) {
                       std::vector<T> successor_data{};
                       std::transform(vec_successors.begin(), vec_successors.end(), std::back_inserter(successor_data),
                                      [](const Node<T> &successor) {
                                          return successor.get_data();
                                      });
                       return successor_data;
                   });

    return successors_data;
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_root_nodes() const
{
    std::vector<Node<T>> root_nodes{this->_get_root_nodes()};
    std::vector<T> root_nodes_data{};

    std::transform(root_nodes.begin(), root_nodes.end(), std::back_inserter(root_nodes_data),
                   [](const auto &root_node) {
                       return root_node.get_data();
                   });
    return root_nodes_data;
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_leaf_nodes() const
{
    std::vector<Node<T>> leaf_nodes{this->_get_leaf_nodes()};
    std::vector<T> leaf_nodes_data{};

    std::transform(leaf_nodes.begin(), leaf_nodes.end(), std::back_inserter(leaf_nodes_data),
                   [](const auto &leaf_node) {
                       return leaf_node.get_data();
                   });
    return leaf_nodes_data;
}