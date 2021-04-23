#include <string> // std::string

#include <main/Component.hpp>

Component::Component(const std::string &name)
    : name{name}
{
}

bool Component::operator==(const Component &rhs) const
{
    return (this->name == rhs.name);
}

bool Component::operator<(const Component &rhs) const
{
    return (this->name < rhs.name);
}

std::string Component::get_name() const
{
    return this->name;
}