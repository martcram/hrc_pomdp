#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <AndOrGraph/ObstructionGraph.hpp>

class Assembly
{
private:
    std::vector<ObstructionGraph> obstruction_graphs;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_rules;

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> compute_blocking_rules() const;

public:
    explicit Assembly(const std::vector<ObstructionGraph> &obstr_graphs);
    ~Assembly() = default;
};

#endif // ASSEMBLY_HPP