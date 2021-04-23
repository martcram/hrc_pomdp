#include <algorithm> // std::sort
#include <vector>    // std::vector

#include <main/Component.hpp>

#include <pomdp/Action.hpp>

using Subassembly = std::vector<Component>;

Action::Action()
    : Action(std::vector<Subassembly>{}, Subassembly{})
{
}

Action::Action(const std::vector<Subassembly> &preconditions, const Subassembly &effect)
    : preconditions{preconditions}, effect{effect}
{
}

std::vector<Subassembly> Action::get_preconditions() const
{
    return this->preconditions;
}

Subassembly Action::get_effect() const
{
    return this->effect;
}

bool Action::operator==(const Action &rhs) const
{
    return ((this->preconditions == rhs.preconditions) && (this->effect == rhs.effect));
}

bool Action::operator<(const Action &rhs) const
{
    return ((this->preconditions < rhs.preconditions) && (this->effect < rhs.effect));
}
