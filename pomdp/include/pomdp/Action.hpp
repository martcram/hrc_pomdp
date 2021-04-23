#ifndef ACTION_HPP
#define ACTION_HPP

#include <vector> // std::vector

#include <main/Component.hpp>

using Subassembly = std::vector<Component>;

class Action
{
private:
    std::vector<Subassembly> preconditions;
    Subassembly effect;

public:
    Action();
    explicit Action(const std::vector<Subassembly> &preconditions, const Subassembly &effect);
    ~Action() = default;

    std::vector<Subassembly> get_preconditions() const;
    Subassembly get_effect() const;

    bool operator==(const Action &rhs) const;
    bool operator<(const Action &rhs) const;
};

#endif // ACTION_HPP