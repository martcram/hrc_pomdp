#include <algorithm>     // std::copy_if, std::find, std::find_if, std::for_each, std::transform
#include <iterator>      // std::back_inserter
#include <unordered_map> // std::unordered_map
#include <utility>       // std::make_pair, std::pair
#include <vector>        // std::vector

template <typename N, typename E>
DiGraph<N, E>::DiGraph()
    : adjacency_list{}, node_ids{}, edge_ids{}, edge_attrs{}, nodes{}
{
}

template <typename N, typename E>
DiGraph<N, E>::DiGraph(const std::vector<std::pair<N, N>> &edges)
    : DiGraph<N, E>()
{
    this->add_edges(edges);
}

template <typename N, typename E>
bool DiGraph<N, E>::get_id(const N &node, int &out) const
{
    auto it = std::find_if(this->node_ids.begin(), this->node_ids.end(),
                           [&node](const auto &n) {
                               return (n.second == node);
                           });

    if (it != node_ids.end())
    {
        out = it->first;
        return true;
    }
    else
    {
        out = this->node_ids.size();
        return false;
    }
}

template <typename N, typename E>
bool DiGraph<N, E>::get_id(const std::pair<N, N> &edge, int &out) const
{
    auto it = std::find_if(this->edge_ids.begin(), this->edge_ids.end(),
                           [&edge](const auto &e) {
                               return (e.second == edge);
                           });

    if (it != edge_ids.end())
    {
        out = it->first;
        return true;
    }
    else
    {
        out = this->edge_ids.size();
        return false;
    }
}

template <typename N, typename E>
void DiGraph<N, E>::add_node(const N &node, int &out_id)
{
    if (!this->get_id(node, out_id))
    {
        this->node_ids.emplace(std::make_pair(out_id, node));
        this->adjacency_list.emplace(std::make_pair(out_id, std::vector<int>{}));

        this->nodes.push_back(node);
    }
}

template <typename N, typename E>
std::vector<N> DiGraph<N, E>::get_nodes() const
{
    std::vector<N> nodes{};
    std::transform(this->node_ids.begin(), this->node_ids.end(), std::back_inserter(nodes),
                   [](const auto &node_id) { return node_id.second; });
    return nodes;
}

template <typename N, typename E>
std::vector<std::pair<N, N>> DiGraph<N, E>::get_edges() const
{
    std::vector<std::pair<N, N>> edges{};
    std::transform(this->edge_ids.begin(), this->edge_ids.end(), std::back_inserter(edges),
                   [](const auto &edge_id) { return edge_id.second; });
    return edges;
}

template <typename N, typename E>
std::vector<N> DiGraph<N, E>::get_successors(const N &node) const
{
    std::vector<N> successors{};

    int node_id{};
    if (this->get_id(node, node_id))
    {
        for (int successor_id : this->adjacency_list.at(node_id))
        {
            auto it = node_ids.find(successor_id);
            if (it != node_ids.end())
                successors.push_back(it->second);
        }
    }

    return successors;
}

template <typename N, typename E>
std::vector<N> DiGraph<N, E>::get_predecessors(const N &node) const
{
    std::vector<N> predecessors{};

    int node_id{};
    if (this->get_id(node, node_id))
    {
        for (const auto &adj : this->adjacency_list)
        {
            if (std::find(adj.second.begin(), adj.second.end(), node_id) != adj.second.end())
            {
                auto it = node_ids.find(adj.first);
                if (it != node_ids.end())
                    predecessors.push_back(it->second);
            }
        }
    }

    return predecessors;
}

template <typename N, typename E>
std::vector<N> DiGraph<N, E>::get_root_nodes() const
{
    std::vector<N> root_nodes{};
    std::copy_if(this->nodes.begin(), this->nodes.end(), std::back_inserter(root_nodes),
                 [this](const N &node) { return this->get_predecessors(node).empty(); });
    return root_nodes;
}

template <typename N, typename E>
std::vector<N> DiGraph<N, E>::get_leaf_nodes() const
{
    std::vector<N> leaf_nodes{};
    std::copy_if(this->nodes.begin(), this->nodes.end(), std::back_inserter(leaf_nodes),
                 [this](const N &node) { return this->get_successors(node).empty(); });
    return leaf_nodes;
}

template <typename N, typename E>
E DiGraph<N, E>::get_edge_attr(const std::pair<N, N> &edge) const
{
    E edge_attr{};
    int edge_id{};
    if (this->get_id(edge, edge_id))
    {
        edge_attr = this->edge_attrs.at(edge_id);
    }
    return edge_attr;
}

template <typename N, typename E>
void DiGraph<N, E>::add_edge(const N &u, const N &v, const E &edge_attr)
{
    int u_id{};
    this->add_node(u, u_id);

    int v_id{};
    this->add_node(v, v_id);

    auto it = this->adjacency_list.find(u_id);
    if (std::find(it->second.begin(), it->second.end(), v_id) == it->second.end())
    {
        it->second.push_back(v_id);

        std::pair<N, N> edge{std::make_pair(u, v)};
        int edge_id{};
        if (!this->get_id(edge, edge_id))
        {
            this->edge_ids.emplace(std::make_pair(edge_id, edge));
            this->edge_attrs.emplace(std::make_pair(edge_id, edge_attr));
        }
    }
}

template <typename N, typename E>
void DiGraph<N, E>::add_edges(const std::vector<std::pair<N, N>> &edges)
{
    std::for_each(edges.begin(), edges.end(), [this](const auto &edge) {
        this->add_edge(edge.first, edge.second);
    });
}

template <typename N, typename E>
DiGraph<N, E> DiGraph<N, E>::reverse() const
{
    DiGraph<N, E> reversed_graph{};

    std::for_each(this->edge_ids.begin(), this->edge_ids.end(),
                  [this, &reversed_graph](const auto &e) {
                      std::pair<N, N> edge{e.second};
                      reversed_graph.add_edge(edge.second, edge.first, this->edge_attrs.at(e.first));
                  });

    return reversed_graph;
}