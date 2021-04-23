#include <vector> // std::vector

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

bool Observation::operator==(const Observation &rhs) const
{
    return ((this->manip_components == rhs.manip_components) && (this->manip_tool == rhs.manip_tool));
}

bool Observation::operator<(const Observation &rhs) const
{
    return ((this->manip_components < rhs.manip_components) && (this->manip_tool < rhs.manip_tool));
}