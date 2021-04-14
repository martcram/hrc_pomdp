#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <algorithm> // std::for_each
#include <string>
#include <vector>

#include <boost/functional/hash.hpp>

class Component
{
private:
    std::string name;
    int id;

public:
    static int s_id;

    Component(const std::string &name);
    ~Component() = default;

    bool operator==(const Component &rhs) const;
    bool operator<(const Component &rhs) const;
    std::string get_name() const;
};

// Custom specialization of std::hash injected in namespace std to deal with key types: Component, std::vector<Component>, std::vector<std::vector<Component>>, std::vector<std::vector<std::vector<Component>>>.
namespace std
{
    template <>
    struct hash<Component>
    {
        std::size_t operator()(Component const &component) const noexcept
        {
            std::size_t seed{0};
            boost::hash_combine(seed, boost::hash_value(component.get_name()));
            return seed;
        }
    };

    template <>
    struct hash<std::vector<Component>>
    {
        std::size_t operator()(std::vector<Component> const &components) const noexcept
        {
            size_t seed{0};
            std::for_each(components.begin(), components.end(),
                          [&seed](const auto &component) {
                              boost::hash_combine(seed, std::hash<Component>{}(component));
                          });
            return seed;
        }
    };

    template <>
    struct hash<std::vector<std::vector<Component>>>
    {
        std::size_t operator()(std::vector<std::vector<Component>> const &vec_components) const noexcept
        {
            size_t seed{0};
            std::for_each(vec_components.begin(), vec_components.end(),
                          [&seed](const auto &components) {
                              boost::hash_combine(seed, std::hash<std::vector<Component>>{}(components));
                          });
            return seed;
        }
    };

    template <>
    struct hash<std::vector<std::vector<std::vector<Component>>>>
    {
        std::size_t operator()(std::vector<std::vector<std::vector<Component>>> const &vec_vec_components) const noexcept
        {
            size_t seed{0};
            std::for_each(vec_vec_components.begin(), vec_vec_components.end(),
                          [&seed](const auto &vec_components) {
                              boost::hash_combine(seed, std::hash<std::vector<std::vector<Component>>>{}(vec_components));
                          });
            return seed;
        }
    };
} // namespace std

#endif // COMPONENT_HPP