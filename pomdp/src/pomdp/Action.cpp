#include <algorithm> // std::for_each, std::sort
#include <ostream>   // std::ostream
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

std::ostream &operator<<(std::ostream &os, const Action &action)
{
    std::vector<Subassembly> preconditions{action.get_preconditions()};
    std::for_each(preconditions.begin(), preconditions.end(),
                  [&os](const Subassembly &precond) {
                      os << precond << " ";
                  });
    os << "--> " << action.get_effect();
    return os;
}