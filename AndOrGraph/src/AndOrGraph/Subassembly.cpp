#include <algorithm>
#include <AndOrGraph/Subassembly.hpp>

Subassembly::Subassembly(const std::set<std::string> &components)
    : components{components}
{
}

bool Subassembly::operator==(const Subassembly &rhs) const
{
    return (this->components == rhs.components);
}