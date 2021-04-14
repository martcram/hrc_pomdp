#ifndef AND_EDGE_HPP
#define AND_EDGE_HPP

#include <vector>

#include <graph/Node.hpp>

template <typename T>
class AndEdge
{
private:
    static constexpr double def_cost = 0.0;

    Node<T> parent_node;
    std::vector<Node<T>> child_nodes;
    double cost;

public:
    explicit AndEdge(const Node<T> &parent_node, const std::vector<Node<T>> &child_nodes, double cost = def_cost);
    ~AndEdge() = default;

    bool operator==(const AndEdge &rhs) const;

    Node<T> get_parent_node() const;
    std::vector<Node<T>> get_child_nodes() const;
};

#include <graph/AndEdge.tpp>

#endif // AND_EDGE_HPP