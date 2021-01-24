#ifndef I_PLOTABLE_HPP
#define I_PLOTABLE_HPP

#include <sstream>

class I_Plotable
{
public:
    virtual ~I_Plotable() = default;
    virtual std::stringstream plot() const = 0;
};

#endif // I_PLOTABLE_HPP