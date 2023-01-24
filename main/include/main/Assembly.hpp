#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <graph/AndOrGraph.hpp>
#include <graph/DiGraph.hpp>
#include <graph/Graph.hpp>

#include <main/Component.hpp>

#include <nlohmann/json.hpp>

using Subassembly = std::vector<Component>;

class Assembly
{
private:
    std::vector<Component> components;
    std::vector<DiGraph<Component>> obstruction_graphs;
    Graph<Component> connection_graph;
    AndOrGraph<Subassembly> ao_graph;

    std::unordered_map<Component, std::vector<Subassembly>> blocking_rules;
    std::unordered_map<Component, std::vector<Subassembly>> technical_constraints;

    std::unordered_map<Component, std::vector<Subassembly>> compute_blocking_rules() const;
    bool check_tech_feasibility(const Subassembly &subassembly) const;
    bool check_geom_feasibility(const Subassembly &subassembly) const;
    bool validate_triplet(const std::vector<Subassembly> &triplet) const;
    std::vector<Component> get_neighbors(const Subassembly &subassembly) const;
    AndOrGraph<Subassembly> generate_ao_graph() const;

public:
    Assembly();
    explicit Assembly(const std::vector<DiGraph<Component>> &obstr_graphs, const Graph<Component> &connect_graph, const std::unordered_map<Component, std::vector<Subassembly>> &tech_constraints);
    ~Assembly() = default;

    std::vector<Component> get_components() const;
    AndOrGraph<Subassembly> get_ao_graph() const;

    void import_ao_graph(const std::string &file_path);
};

#endif // ASSEMBLY_HPP