#include <string>

#include <main/Component.hpp>

int Component::s_id{0};

Component::Component(const std::string &name)
    : name{name}, id{s_id++}
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
    return name;
}