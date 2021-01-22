#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>

class Node
{
private:
    static constexpr double def_cost = 0.0;

    std::vector<std::string> subassembly;
    double cost;

public:
    explicit Node(const std::vector<std::string> &subassembly, double cost = def_cost);
    ~Node() = default;

    bool operator==(const Node &rhs) const;

    std::vector<std::string> get_subassembly() const;
    double get_cost() const;
};

struct NodeHasher
{
    std::size_t operator()(const Node &n) const;
};

#endif // NODE_HPP