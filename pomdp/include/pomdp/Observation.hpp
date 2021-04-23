#ifndef OBSERVATION_HPP
#define OBSERVATION_HPP

#include <vector> // std::vector

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

    bool operator==(const Observation &rhs) const;
    bool operator<(const Observation &rhs) const;
};

#endif // OBSERVATION_HPP