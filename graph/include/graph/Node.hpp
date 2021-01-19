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
};

#endif // NODE_HPP