#include <algorithm> // std::for_each
#include <ostream>   // std::ostream
#include <string>    // std::string
#include <vector>    // std::vector

#include <main/Component.hpp>

Component::Component(const std::string &name)
    : name{name}
{
}

std::string Component::get_name() const
{
    return this->name;
}

bool Component::operator==(const Component &rhs) const
{
    return (this->name == rhs.name);
}

bool Component::operator<(const Component &rhs) const
{
    return (this->name < rhs.name);
}

std::ostream &operator<<(std::ostream &os, const Component &component)
{
    os << component.get_name();
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Component> &v_component)
{
    os << "( ";
    std::for_each(v_component.begin(), v_component.end(),
                  [&os](const Component &component) {
                      os << component << " ";
                  });
    os << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::vector<Component>> &vv_component)
{
    os << "[ ";
    std::for_each(vv_component.begin(), vv_component.end(),
                  [&os](const std::vector<Component> &v_component) {
                      os << v_component << " ";
                  });
    os << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::vector<std::vector<Component>>> &vvv_component)
{
    os << "---\n";
    std::for_each(vvv_component.begin(), vvv_component.end(),
                  [&os](const std::vector<std::vector<Component>> &vv_component) {
                      os << vv_component << "\n";
                  });
    os << "---";
    return os;
}