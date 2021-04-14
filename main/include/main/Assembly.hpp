#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <unordered_map>
#include <vector>

#include <graph/AndOrGraph.hpp>
#include <graph/DiGraph.hpp>
#include <graph/Graph.hpp>

#include <main/Component.hpp>

using Subassembly = std::vector<Component>;
using State       = std::vector<Subassembly>;
using Intention   = std::vector<State>;

class Assembly
{
private:
    std::vector<Component> components;
    std::vector<DiGraph<Component>> obstruction_graphs;
    Graph<Component> connection_graph;
    
    AndOrGraph<Subassembly> ao_graph;
    DiGraph<State> state_graph;
    DiGraph<Intention> intention_graph;

    std::unordered_map<Component, std::vector<Subassembly>> blocking_rules;
    std::unordered_map<Component, std::vector<Subassembly>> technical_constraints;

    std::unordered_map<Component, std::vector<Subassembly>> compute_blocking_rules() const;
    bool check_tech_feasibility(const Subassembly &subassembly) const;
    bool check_geom_feasibility(const Subassembly &subassembly) const;
    bool validate_triplet(const std::vector<Subassembly> &triplet) const;
    std::vector<Component> get_neighbors(const Subassembly &subassembly) const;
    
    AndOrGraph<Subassembly> generate_ao_graph() const;
    DiGraph<State> generate_state_graph() const;
    DiGraph<Intention> generate_intention_graph() const;

public:
    explicit Assembly(const std::vector<DiGraph<Component>> &obstr_graphs, const Graph<Component> &connect_graph, const std::unordered_map<Component, std::vector<Subassembly>> &tech_constraints);
    ~Assembly() = default;
};

#endif // ASSEMBLY_HPP