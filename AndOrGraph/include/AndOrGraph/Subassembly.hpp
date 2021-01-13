#ifndef SUBASSEMBLY_HPP
#define SUBASSEMBLY_HPP

#include <set>
#include <string>

class Subassembly
{
private:
    std::set<std::string> components;

public:
    explicit Subassembly(const std::set<std::string> &components);
    ~Subassembly() = default;

    bool operator==(const Subassembly &rhs) const;
};

#endif // SUBASSEMBLY_HPP