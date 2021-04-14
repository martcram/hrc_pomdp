#include <algorithm> // std::for_each
#include <vector>

#include <boost/functional/hash.hpp>

template <typename T>
Node<T>::Node(const T &data, double cost)
    : data{data}, cost{cost}
{
}

template <typename T>
bool Node<T>::operator==(const Node<T> &rhs) const
{
    return (this->data == rhs.data);
}

template <typename T>
bool Node<T>::operator<(const Node<T> &rhs) const
{
    return (this->data < rhs.data);
}

template <typename T>
T Node<T>::get_data() const
{
    return data;
}

template <typename T>
double Node<T>::get_cost() const
{
    return cost;
}

// Custom specialization of std::hash injected in namespace std to deal with key types: Node, std::vector<Node>.
namespace std
{
    template <typename T>
    struct hash<Node<T>>
    {
        std::size_t operator()(Node<T> const &n) const noexcept
        {
            std::size_t seed{0};
            boost::hash_combine(seed, std::hash<T>{}(n.get_data()));
            // For now, the node cost isn't used for calculating the hash value.
            //boost::hash_combine(seed, boost::hash_value(n.get_cost()));
            return seed;
        }
    };

    template <typename T>
    struct hash<std::vector<Node<T>>>
    {
        std::size_t operator()(std::vector<Node<T>> const &nodes) const noexcept
        {
            size_t seed{0};
            std::for_each(nodes.begin(), nodes.end(),
                          [&seed](const auto &node) {
                              boost::hash_combine(seed, std::hash<Node<T>>{}(node));
                          });
            return seed;
        }
    };
} // namespace std