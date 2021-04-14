#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

template <typename T>
class Node
{
private:
    static constexpr double def_cost = 0.0;

    T data;
    double cost;

public:
    explicit Node(const T &data, double cost = def_cost);
    ~Node() = default;

    bool operator==(const Node<T> &rhs) const;
    bool operator<(const Node<T> &rhs) const;

    T get_data() const;
    double get_cost() const;
};

#include <graph/Node.tpp>

#endif // NODE_HPP