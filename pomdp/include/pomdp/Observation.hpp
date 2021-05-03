#ifndef OBSERVATION_HPP
#define OBSERVATION_HPP

#include <ostream> // std::ostream
#include <vector>  // std::vector

#include <main/Component.hpp>

class Observation
{
private:
    std::vector<Component> manip_components;
    bool manip_tool;

public:
    Observation();
    explicit Observation(const std::vector<Component> &manip_components, bool manip_tool = false);
    ~Observation() = default;

    std::vector<Component> get_manip_components() const;
    bool is_tool_manipulated() const;

    bool operator==(const Observation &rhs) const;
    bool operator<(const Observation &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Observation &observation);
};

#endif // OBSERVATION_HPP