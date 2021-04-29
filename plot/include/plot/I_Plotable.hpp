#ifndef I_PLOTABLE_HPP
#define I_PLOTABLE_HPP

#include <sstream> // std::stringstream
#include <string>  // std::string

class I_Plotable
{
public:
    virtual ~I_Plotable() = default;

    virtual std::string get_name() const = 0;
    virtual std::stringstream generate_dot() const = 0;
};

#endif // I_PLOTABLE_HPP