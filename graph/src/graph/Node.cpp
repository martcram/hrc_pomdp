#include <iomanip>
#include <ios>
#include <ostream>
#include <string>
#include <vector>

#include <boost/functional/hash.hpp>
#include <graph/Node.hpp>

Node::Node(const std::vector<std::string> &subassembly, double cost)
    : subassembly{subassembly}, cost{cost}
{
}

bool Node::operator==(const Node &rhs) const
{
    return (this->subassembly == rhs.subassembly);
}

bool Node::operator<(const Node &rhs) const
{
    return (this->subassembly < rhs.subassembly);
}

void Node::print(std::ostream &os) const
{
    // Keep track of the initial precision
    std::streamsize ss{os.precision()};

    os << std::setiosflags(std::ios::fixed) << std::setprecision(2)
       << "[ ";

    for (const auto &part : subassembly)
        os << part << ' ';

    //os << "| " << cost;
    os << "]"
       << std::resetiosflags(std::ios::fixed) << std::setprecision(ss);
}

std::vector<std::string> Node::get_subassembly() const
{
    return subassembly;
}

double Node::get_cost() const
{
    return cost;
}