#include <algorithm>     // std::copy_if, std::find, std::find_if, std::for_each, std::sort, std::transform
#include <iterator>      // std::back_inserter
#include <tuple>         // std::get, std::make_tuple, std::tuple
#include <unordered_map> // std::unordered_map
#include <utility>       // std::make_pair
#include <vector>        // std::vector

template <typename T>
AndOrGraph<T>::AndOrGraph()
    : adjacency_list{}, node_ids{}
{
}

template <typename T>
AndOrGraph<T>::AndOrGraph(const std::vector<std::tuple<T, std::vector<T>, int>> &edges)
    : AndOrGraph<T>()
{
    this->add_edges(edges);
}

// VERTEX - BEGIN
template <typename T>
AndOrGraph<T>::Node::Node(const T &data)
    : data{data}
{
}

template <typename T>
bool AndOrGraph<T>::Node::operator==(const Node &rhs) const
{
    return (this->data == rhs.data);
}

template <typename T>
bool AndOrGraph<T>::Node::operator<(const Node &rhs) const
{
    return (this->data < rhs.data);
}
// VERTEX - END

// EDGE - BEGIN
template <typename T>
AndOrGraph<T>::AndEdge::AndEdge(const std::vector<Node> &child_nodes, int id)
    : child_nodes{}, id{id}
{
    std::copy_if(child_nodes.begin(), child_nodes.end(), std::back_inserter(this->child_nodes),
                 [this](const Node child_node) {
                     return (std::find(this->child_nodes.begin(), this->child_nodes.end(), child_node) == this->child_nodes.end());
                 });

    std::sort(this->child_nodes.begin(), this->child_nodes.end());
}

template <typename T>
bool AndOrGraph<T>::AndEdge::operator==(const AndEdge &rhs) const
{
    return (this->child_nodes == rhs.child_nodes);
}

template <typename T>
bool AndOrGraph<T>::AndEdge::operator<(const AndEdge &rhs) const
{
    return (this->child_nodes < rhs.child_nodes);
}
// EDGE - END

template <typename T>
bool AndOrGraph<T>::get_id(const Node &node, int &out) const
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
void AndOrGraph<T>::add_node(const Node &node, int &out_id)
{
    if (!this->get_id(node, out_id))
    {
        this->node_ids.emplace(std::make_pair(out_id, node));
        this->adjacency_list.emplace(std::make_pair(out_id, std::vector<AndEdge>{}));
    }
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_nodes() const
{
    std::vector<T> nodes_data{};
    std::transform(node_ids.begin(), node_ids.end(), std::back_inserter(nodes_data),
                   [](const auto &m) {
                       return m.second.data;
                   });
    return nodes_data;
}

template <typename T>
std::vector<std::tuple<T, std::vector<T>, int>> AndOrGraph<T>::get_edges(const T &data) const
{
    std::vector<std::tuple<T, std::vector<T>, int>> edges_data{};

    Node node{data}; // cannot construct a parameterless node
    int node_id{};
    if (this->get_id(node, node_id))
    {
        for (const AndEdge &edge : this->adjacency_list.at(node_id))
        {               
            std::vector<T> child_data{};
            std::transform(edge.child_nodes.begin(), edge.child_nodes.end(), std::back_inserter(child_data),
                        [](const Node child_node) {
                            return child_node.data;
                        });
            edges_data.push_back(std::make_tuple(data, child_data, edge.id));
        }
    }

    return edges_data;
}

template <typename T>
std::vector<std::tuple<T, std::vector<T>, int>> AndOrGraph<T>::get_edges() const
{
    std::vector<std::tuple<T, std::vector<T>, int>> edges_data{};

    for (const auto m : this->node_ids)
    {
        int parent_node_id{};
        if (this->get_id(m.second, parent_node_id))
        {
            T parent_node{m.second.data};
            for (const AndEdge &edge : this->adjacency_list.at(parent_node_id))
            {               
                std::vector<T> child_data{};
                std::transform(edge.child_nodes.begin(), edge.child_nodes.end(), std::back_inserter(child_data),
                            [](const Node child_node) {
                                return child_node.data;
                            });
                edges_data.push_back(std::make_tuple(parent_node, child_data, edge.id));
            }
        }
    }

    return edges_data;
}

template <typename T>
std::vector<std::vector<T>> AndOrGraph<T>::get_successors(const T &data) const
{
    std::vector<std::vector<T>> successors{};

    int data_id{};
    if (this->get_id(Node{data}, data_id))
    {
        for (const AndEdge &edge : this->adjacency_list.at(data_id))
        {
            std::vector<T> child_data{};
            std::transform(edge.child_nodes.begin(), edge.child_nodes.end(), std::back_inserter(child_data),
                           [](const Node child_node) {
                               return child_node.data;
                           });
            successors.push_back(child_data);
        }
    }

    return successors;
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_predecessors(const T &data) const
{
    std::vector<T> predecessors{};

    int data_id{};
    if (this->get_id(Node{data}, data_id))
    {
        for (const auto m : this->node_ids)
        {
            std::vector<T> successors{};
            for (const std::vector<T> &edge_successors : this->get_successors(m.second.data))
                successors.insert(successors.end(), edge_successors.begin(), edge_successors.end());

            if (std::find(successors.begin(), successors.end(), data) != successors.end() &&
                std::find(predecessors.begin(), predecessors.end(), data) == predecessors.end())
                predecessors.push_back(m.second.data);
        }
    }

    return predecessors;
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_root_nodes() const
{
    std::vector<T> nodes_data{this->get_nodes()};

    std::vector<T> root_nodes{};
    std::copy_if(nodes_data.begin(), nodes_data.end(), std::back_inserter(root_nodes),
                 [this](const T &data) { return this->get_predecessors(data).empty(); });
    return root_nodes;
}

template <typename T>
std::vector<T> AndOrGraph<T>::get_leaf_nodes() const
{
    std::vector<T> nodes_data{this->get_nodes()};

    std::vector<T> leaf_nodes{};
    std::copy_if(nodes_data.begin(), nodes_data.end(), std::back_inserter(leaf_nodes),
                 [this](const T &data) { return this->get_successors(data).empty(); });
    return leaf_nodes;
}

template <typename T>
void AndOrGraph<T>::add_edge(const T &parent_data, const std::vector<T> &child_data, int id)
{
    Node parent_node{parent_data};
    int parent_id{};
    this->add_node(parent_node, parent_id);

    std::vector<Node> child_nodes{};
    for (const T &data : child_data)
    {
        Node child_node{data};
        int child_id{};
        this->add_node(child_node, child_id);

        child_nodes.push_back(child_node);
    }

    AndEdge edge{child_nodes, id};
    auto it = this->adjacency_list.find(parent_id);
    if (std::find(it->second.begin(), it->second.end(), edge) == it->second.end())
        it->second.push_back(edge);
}

template <typename T>
void AndOrGraph<T>::add_edges(const std::vector<std::tuple<T, std::vector<T>, int>> &edges)
{
    std::for_each(edges.begin(), edges.end(),
                  [this](const std::tuple<T, std::vector<T>, int> &edge) {
                      this->add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
                  });
}