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

std::vector<std::string> Node::get_subassembly() const
{
    return subassembly;
}

double Node::get_cost() const
{
    return cost;
}

std::size_t NodeHasher::operator()(const Node &n) const
{
    std::size_t seed{0};
    boost::hash_combine(seed, boost::hash_value(n.get_subassembly()));
    // For now, the node cost isn't used for calculating the hash value.
    //boost::hash_combine(seed, boost::hash_value(n.get_cost()));
    return seed;
}