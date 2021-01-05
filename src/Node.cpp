#include "Node.hpp"

Node::Node(const Subassembly &subassembly, double cost)
    : subassembly{subassembly}, cost{cost}
{
}