#ifndef NODE_HPP
#define NODE_HPP

#include "Subassembly.hpp"

class Node
{
private:
    static constexpr double def_cost = 0.0;

    Subassembly subassembly;
    double cost;

public:
    explicit Node(const Subassembly &subassembly, double cost = def_cost);
    ~Node() = default;
};

#endif // NODE_HPP