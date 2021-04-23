#include <algorithm>     // std::find, std::find_if, std::for_each
#include <unordered_map> // std::unordered_map
#include <utility>       // std::make_pair, std::pair
#include <vector>        // std::vector

template <typename T>
Graph<T>::Graph()
    : adjacency_list{}, node_ids{}, nodes{}, edges{}
{
}

template <typename T>
Graph<T>::Graph(const std::vector<std::pair<T, T>> &edges)
    : Graph<T>()
{
    this->add_edges(edges);
}

template <typename T>
bool Graph<T>::get_id(const T &node, int &out) const
{
    auto it = std::find_if(this->node_ids.begin(), this->node_ids.end(),
                           [&node](const auto &m) {
                               return (m.second == node);
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

template <typename T>
void Graph<T>::add_node(const T &node, int &out_id)
{
    if (!this->get_id(node, out_id))
    {
        this->node_ids.emplace(std::make_pair(out_id, node));
        this->adjacency_list.emplace(std::make_pair(out_id, std::vector<int>{}));

        this->nodes.push_back(node);
    }
}

template <typename T>
std::vector<T> Graph<T>::get_nodes() const
{
    return this->nodes;
}

template <typename T>
std::vector<std::pair<T, T>> Graph<T>::get_edges() const
{
    return this->edges;
}

template <typename T>
std::vector<T> Graph<T>::get_neighbors(const T &node) const
{
    std::vector<T> neighbors{};

    int node_id{};
    if (this->get_id(node, node_id))
    {
        for (int neighbor_id : this->adjacency_list.at(node_id))
        {
            auto it = node_ids.find(neighbor_id);
            if (it != node_ids.end())
                neighbors.push_back(it->second);
        }
    }

    return neighbors;
}

template <typename T>
void Graph<T>::add_edge(const T &u, const T &v)
{
    int u_id{};
    this->add_node(u, u_id);

    int v_id{};
    this->add_node(v, v_id);

    auto u_it = this->adjacency_list.find(u_id);
    if (std::find(u_it->second.begin(), u_it->second.end(), v_id) == u_it->second.end())
    {
        u_it->second.push_back(v_id);
        this->edges.push_back(std::make_pair(u, v));
    }

    auto v_it = this->adjacency_list.find(v_id);
    if (std::find(v_it->second.begin(), v_it->second.end(), u_id) == v_it->second.end())
        v_it->second.push_back(u_id);
}

template <typename T>
void Graph<T>::add_edges(const std::vector<std::pair<T, T>> &edges)
{
    std::for_each(edges.begin(), edges.end(), [this](const auto &edge) {
        this->add_edge(edge.first, edge.second);
    });
}