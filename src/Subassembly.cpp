#include <algorithm>
#include "Subassembly.hpp"

Subassembly::Subassembly(const std::set<std::string> &components)
    : components{components}
{
}
