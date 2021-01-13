#include "Node.hpp"

Node::Node(const Subassembly &subassembly, double cost)
    : subassembly{subassembly}, cost{cost}
{
}

bool Node::operator==(const Node &rhs) const
{
    return (this->subassembly == rhs.subassembly);
}