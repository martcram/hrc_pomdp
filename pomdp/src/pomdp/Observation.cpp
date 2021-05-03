#include <ostream> // std::ostream
#include <vector>  // std::vector

#include <main/Component.hpp>

#include <pomdp/Observation.hpp>

Observation::Observation()
    : Observation(std::vector<Component>{})
{
}

Observation::Observation(const std::vector<Component> &manip_components, bool manip_tool)
    : manip_components{manip_components}, manip_tool{manip_tool}
{
}

std::vector<Component> Observation::get_manip_components() const
{
    return this->manip_components;
}

bool Observation::is_tool_manipulated() const
{
    return this->manip_tool;
}

bool Observation::operator==(const Observation &rhs) const
{
    return ((this->manip_components == rhs.manip_components) && (this->manip_tool == rhs.manip_tool));
}

bool Observation::operator<(const Observation &rhs) const
{
    return ((this->manip_components < rhs.manip_components) && (this->manip_tool < rhs.manip_tool));
}

std::ostream &operator<<(std::ostream &os, const Observation &observation)
{
    os << observation.get_manip_components() << " - "
       << (observation.is_tool_manipulated() ? "True" : "False");
    return os;
}