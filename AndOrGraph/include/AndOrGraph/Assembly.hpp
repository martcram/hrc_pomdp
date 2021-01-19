#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <AndOrGraph/AndOrGraph.hpp>
#include <AndOrGraph/ConnectionGraph.hpp>
#include <AndOrGraph/ObstructionGraph.hpp>

#include <graph/Graph.hpp>
#include <graph/DiGraph.hpp>

class Assembly
{
private:
    std::vector<std::string> parts;
    std::vector<DiGraph<std::string>> obstruction_graphs;
    Graph<std::string> connection_graph;
    AndOrGraph ao_graph;
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_rules;

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> compute_blocking_rules() const;
    bool check_geom_feasibility(const std::vector<std::string> &subassembly) const;
    bool validate_triplet(const std::vector<std::vector<std::string>> &triplet) const;
    std::vector<std::vector<std::vector<std::string>>> reversed_cutset() const;
    std::vector<std::string> get_neighbors(const std::vector<std::string> &subassembly) const;

public:
    explicit Assembly(const std::vector<DiGraph<std::string>> &obstr_graphs, const Graph<std::string> &connect_graph);
    ~Assembly() = default;  
};

#endif // ASSEMBLY_HPP