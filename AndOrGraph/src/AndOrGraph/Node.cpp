#include <string>
#include <vector>

#include <AndOrGraph/Node.hpp>

Node::Node(const std::vector<std::string> &subassembly, double cost)
    : subassembly{subassembly}, cost{cost}
{
}

bool Node::operator==(const Node &rhs) const
{
    return (this->subassembly == rhs.subassembly);
}