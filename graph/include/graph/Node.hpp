#ifndef NODE_HPP
#define NODE_HPP

#include <ostream>
#include <string>
#include <vector>

#include <boost/functional/hash.hpp>
#include <graph/I_Printable.hpp>

class Node : public I_Printable
{
private:
    static constexpr double def_cost = 0.0;

    std::vector<std::string> subassembly;
    double cost;

public:
    explicit Node(const std::vector<std::string> &subassembly, double cost = def_cost);
    ~Node() = default;

    bool operator==(const Node &rhs) const;
    bool operator<(const Node &rhs) const;
    void print(std::ostream &os) const override;

    std::vector<std::string> get_subassembly() const;
    double get_cost() const;
};

// Custom specialization of std::hash injected in namespace std to deal with key types: Node, std::vector<Node>.
namespace std
{
    template <>
    struct hash<Node>
    {
        std::size_t operator()(Node const &n) const noexcept
        {
            std::size_t seed{0};
            boost::hash_combine(seed, boost::hash_value(n.get_subassembly()));
            // For now, the node cost isn't used for calculating the hash value.
            //boost::hash_combine(seed, boost::hash_value(n.get_cost()));
            return seed;
        }
    };

    template <>
    struct hash<std::vector<Node>>
    {
        std::size_t operator()(std::vector<Node> const &nodes) const noexcept
        {
            size_t seed{0};
            for (const auto &node : nodes)
                boost::hash_combine(seed, std::hash<Node>{}(node));
            return seed;
        }
    };
} // namespace std

#endif // NODE_HPP