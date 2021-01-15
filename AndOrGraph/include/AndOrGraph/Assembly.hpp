#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <AndOrGraph/ConnectionGraph.hpp>
#include <AndOrGraph/ObstructionGraph.hpp>

class Assembly
{
private:
    std::vector<ObstructionGraph> obstruction_graphs;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_rules;

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> compute_blocking_rules() const;

public:
    explicit Assembly(const std::vector<ObstructionGraph> &obstr_graphs, const ConnectionGraph &connect_graph);
    ~Assembly() = default;
    
    bool check_geom_feasibility(std::vector<std::string> subassembly) const;
};

#endif // ASSEMBLY_HPP