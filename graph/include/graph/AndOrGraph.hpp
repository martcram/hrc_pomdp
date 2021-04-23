#ifndef AND_OR_GRAPH_HPP
#define AND_OR_GRAPH_HPP

#include <unordered_map> // std::unordered_map
#include <utility>       // std::pair
#include <vector>        // std::vector

template <typename T>
class AndOrGraph
{
private:
    struct Node
    {
        T data;

        explicit Node(const T &data);
        ~Node() = default;

        bool operator==(const Node &rhs) const;
        bool operator<(const Node &rhs) const;
    };

    struct AndEdge
    {
        std::vector<Node> child_nodes;

        explicit AndEdge(const std::vector<Node> &child_nodes);
        ~AndEdge() = default;

        bool operator==(const AndEdge &rhs) const;
        bool operator<(const AndEdge &rhs) const;
    };

    std::unordered_map<int, std::vector<AndEdge>> adjacency_list;
    std::unordered_map<int, Node> node_ids;

    bool get_id(const Node &node, int &out) const;

    void add_node(const Node &node, int &out_id);

public:
    AndOrGraph();
    explicit AndOrGraph(const std::vector<std::pair<T, std::vector<T>>> &edges);
    ~AndOrGraph() = default;

    std::vector<T> get_nodes() const;
    std::vector<std::pair<T, std::vector<T>>> get_edges() const;

    std::vector<std::vector<T>> get_successors(const T &data) const;
    std::vector<T> get_predecessors(const T &data) const;

    std::vector<T> get_root_nodes() const;
    std::vector<T> get_leaf_nodes() const;

    void add_edge(const T &parent_data, const std::vector<T> &child_data);
    void add_edges(const std::vector<std::pair<T, std::vector<T>>> &edges);
};

#include <graph/AndOrGraph.tpp>

#endif // AND_OR_GRAPH_HPP