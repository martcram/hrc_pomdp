#include <algorithm>     // std::copy, std::copy_if, std::find, std::includes, std::set_intersection, std::set_union, std::sort, std::transform, std::unique
#include <cmath>         // std::ceil
#include <fstream>       // std::ifstream
#include <iterator>      // std::back_inserter, std::inserter
#include <map>           // std::map
#include <set>           // std::set
#include <string>        // std::string 
#include <unordered_map> // std::unordered_map
#include <utility>       // std::pair
#include <vector>        // std::vector

#include <graph/AndOrGraph.hpp>
#include <graph/DiGraph.hpp>
#include <graph/Graph.hpp>

#include <main/Assembly.hpp>
#include <main/Component.hpp>

#include <utils/utils.hpp> // utils::cartesian_product

#include <nlohmann/json.hpp>

using Subassembly = std::vector<Component>;

Assembly::Assembly()
    : components{}, obstruction_graphs{}, connection_graph{}, ao_graph{},
      blocking_rules{}, technical_constraints{}
{
}

Assembly::Assembly(const std::vector<DiGraph<Component>> &obstr_graphs, const Graph<Component> &connect_graph, const std::unordered_map<Component, std::vector<Subassembly>> &tech_constraints)
    : components{}, obstruction_graphs{obstr_graphs}, connection_graph{connect_graph}, ao_graph{},
      blocking_rules{}, technical_constraints{tech_constraints}
{
    components = connection_graph.get_nodes();
    blocking_rules = this->compute_blocking_rules();
    ao_graph = this->generate_ao_graph();
}

std::unordered_map<Component, std::vector<Subassembly>> Assembly::compute_blocking_rules() const
{
    std::unordered_map<Component, std::vector<Subassembly>> blocking_parts{};
    for (const auto &component : components)
        blocking_parts.insert({component, std::vector<Subassembly>{}});

    for (const auto &obstr_graph : obstruction_graphs)
    {
        for (const auto &component : components)
        {
            std::vector<Component> successors{obstr_graph.get_successors(component)};
            std::sort(successors.begin(), successors.end());
            blocking_parts.at(component).push_back(successors);
        }
    }

    std::unordered_map<Component, std::vector<Subassembly>> blocking_rules{};
    for (const auto &component : components)
    {
        std::vector<Subassembly> subassemblies{utils::cartesian_product(blocking_parts.at(component))};
        for (auto &subassembly : subassemblies)
        {
            std::sort(subassembly.begin(), subassembly.end());
            subassembly.erase(std::unique(subassembly.begin(), subassembly.end()), subassembly.end());
        }
        std::sort(subassemblies.begin(), subassemblies.end());
        subassemblies.erase(std::unique(subassemblies.begin(), subassemblies.end()), subassemblies.end());

        blocking_rules.insert({component, subassemblies});
    }

    return blocking_rules;
}

bool Assembly::check_tech_feasibility(const Subassembly &subassembly) const
{
    for (auto it = technical_constraints.begin(); it != technical_constraints.end(); ++it)
    {
        for (auto rule : it->second)
        {
            // 'rule' needs to be sorted
            std::sort(rule.begin(), rule.end());
            bool is_subset = std::includes(subassembly.begin(), subassembly.end(), rule.begin(), rule.end());
            if (is_subset && (std::find(subassembly.begin(), subassembly.end(), it->first) == subassembly.end()))
                return false;
        }
    }
    return true;
}

bool Assembly::check_geom_feasibility(const Subassembly &subassembly) const
{
    for (auto it = blocking_rules.begin(); it != blocking_rules.end(); ++it)
    {
        for (const auto &rule : it->second)
        {
            // 'rule' needs to be sorted, but was already sorted when computing blocking rules
            bool is_subset = std::includes(subassembly.begin(), subassembly.end(), rule.begin(), rule.end());
            if (is_subset && (std::find(subassembly.begin(), subassembly.end(), it->first) == subassembly.end()))
                return false;
        }
    }
    return true;
}

bool Assembly::validate_triplet(const std::vector<Subassembly> &triplet) const
{
    Subassembly subasm_union{};
    std::set_union(triplet.at(0).begin(), triplet.at(0).end(),
                   triplet.at(1).begin(), triplet.at(1).end(),
                   std::back_inserter(subasm_union));

    Subassembly subasm_intersect{};
    std::set_intersection(triplet.at(0).begin(), triplet.at(0).end(),
                          triplet.at(1).begin(), triplet.at(1).end(),
                          std::back_inserter(subasm_intersect));

    return (subasm_union == triplet.at(2) && subasm_intersect.empty());
}

std::vector<Component> Assembly::get_neighbors(const Subassembly &subassembly) const
{
    std::vector<Component> asm_neighbors{};
    for (const auto &component : subassembly)
    {
        std::vector<Component> component_neighbors{connection_graph.get_neighbors(component)};
        std::copy_if(component_neighbors.begin(), component_neighbors.end(), std::back_inserter(asm_neighbors),
                     [&asm_neighbors, &subassembly](Component &component) {
                         return (std::find(asm_neighbors.begin(), asm_neighbors.end(), component) == asm_neighbors.end()) &&
                                (std::find(subassembly.begin(), subassembly.end(), component) == subassembly.end());
                     });
    }
    return asm_neighbors;
}

AndOrGraph<Subassembly> Assembly::generate_ao_graph() const
{
    size_t num_components{components.size()};
    std::map<size_t, std::vector<Subassembly>> subasm_length_map{};

    std::vector<Subassembly> one_component_asms{};
    std::transform(components.begin(), components.end(), std::back_inserter(one_component_asms),
                   [](const Component &component) { return Subassembly{component}; });
    subasm_length_map.insert({1, one_component_asms});

    std::vector<Subassembly> connections{};
    std::vector<std::pair<Component, Component>> pair_connections{connection_graph.get_edges()};
    std::transform(pair_connections.begin(), pair_connections.end(), std::back_inserter(connections),
                   [](const auto &connection) {
                       return Subassembly{connection.first, connection.second};
                   });

    std::vector<Subassembly> two_component_asms{};
    std::copy_if(connections.begin(), connections.end(), std::back_inserter(two_component_asms),
                 [this](Subassembly &two_component_asm) {
                     std::sort(two_component_asm.begin(), two_component_asm.end());
                     return (this->check_geom_feasibility(two_component_asm) && this->check_tech_feasibility(two_component_asm));
                 });
    subasm_length_map.insert({2, two_component_asms});

    std::vector<Subassembly> complete_asm{components};
    subasm_length_map.insert({num_components, complete_asm});

    for (size_t subasm_length{3}; subasm_length < num_components; ++subasm_length)
    {
        std::vector<Subassembly> subassemblies{};
        for (const auto &subassembly : subasm_length_map.at(subasm_length - 1))
        {
            std::vector<Component> subasm_neighbors{this->get_neighbors(subassembly)};
            std::vector<Subassembly> subassemblies_temp{
                utils::cartesian_product(std::vector<Subassembly>{subassembly}, subasm_neighbors)};
            std::copy_if(subassemblies_temp.begin(), subassemblies_temp.end(), std::back_inserter(subassemblies),
                         [this, &subassemblies](auto &subassembly) {
                             std::sort(subassembly.begin(), subassembly.end());
                             return ((std::find(subassemblies.begin(), subassemblies.end(), subassembly) == subassemblies.end()) &&
                                     this->check_geom_feasibility(subassembly) && this->check_tech_feasibility(subassembly));
                         });
        }
        subasm_length_map.insert({subasm_length, subassemblies});
    }

    std::vector<Subassembly> subassemblies{};
    for (auto it = subasm_length_map.rbegin(); it != subasm_length_map.rend(); ++it)
        subassemblies.insert(subassemblies.end(), it->second.begin(), it->second.end());

    std::vector<std::vector<Subassembly>> cutsets{};
    for (size_t triplet3_len{num_components}; triplet3_len >= 3; --triplet3_len)
    {
        for (size_t triplet1_len{triplet3_len - 1}; triplet1_len >= std::ceil(triplet3_len / 2.0); --triplet1_len)
        {
            size_t triplet2_len{triplet3_len - triplet1_len};
            std::vector<std::vector<Subassembly>> triplets{
                utils::cartesian_product(std::vector<std::vector<Subassembly>>{subasm_length_map.at(triplet1_len),
                                                                               subasm_length_map.at(triplet2_len),
                                                                               subasm_length_map.at(triplet3_len)})};
            std::copy_if(triplets.begin(), triplets.end(), std::back_inserter(cutsets),
                         [this](auto &triplet) {
                             std::sort(triplet.at(0).begin(), triplet.at(0).end());
                             std::sort(triplet.at(1).begin(), triplet.at(1).end());
                             std::sort(triplet.at(2).begin(), triplet.at(2).end());
                             return this->validate_triplet(triplet);
                         });
        }
    }

    std::vector<std::vector<Subassembly>> two_component_cutsets{};
    std::transform(two_component_asms.begin(), two_component_asms.end(), std::back_inserter(two_component_cutsets),
                   [](const auto &two_component_asm) {
                       return std::vector<Subassembly>{{two_component_asm.at(0)},
                                                       {two_component_asm.at(1)},
                                                       two_component_asm};
                   });
    cutsets.insert(cutsets.end(), two_component_cutsets.begin(), two_component_cutsets.end());

    AndOrGraph<Subassembly> ao_graph{};

    for (const auto &cutset : cutsets)
        ao_graph.add_edge(cutset.at(2), std::vector<Subassembly>{cutset.at(0), cutset.at(1)});

    return ao_graph;
}

std::vector<Component> Assembly::get_components() const
{
    return this->components;
}

AndOrGraph<Subassembly> Assembly::get_ao_graph() const
{
    return this->ao_graph;
}

void Assembly::import_ao_graph(const nlohmann::json &json)
{
    nlohmann::json components = json.at("components");
    nlohmann::json unions = json.at("unions");

    AndOrGraph<std::string> ao_graph_ids{};
    for (const nlohmann::json &u: unions)
        ao_graph_ids.add_edge(u.at("parent_component"), u.at("child_components"));

    std::vector<std::string> leaf_comp_ids{ao_graph_ids.get_leaf_nodes()};
    std::unordered_map<std::string, Subassembly> comp_subasm_mapping{};
    for (const std::string &leaf_comp_id : leaf_comp_ids)
        comp_subasm_mapping.emplace(std::make_pair(leaf_comp_id, Subassembly{{Component{components.at(leaf_comp_id).at("label")}}}));
    std::vector<std::string> component_mapping_ids{};
    for (auto it = comp_subasm_mapping.begin(); it != comp_subasm_mapping.end(); ++it) 
        component_mapping_ids.push_back(it->first);

    std::set<std::string> open_union_ids{};
    for (const std::string &leaf_comp_id : leaf_comp_ids)
    {
        std::vector<std::string> parent_unions = components.at(leaf_comp_id).at("parent_unions");
        std::copy(parent_unions.begin(), parent_unions.end(), std::inserter(open_union_ids, open_union_ids.end()));
    }
    while(!open_union_ids.empty())
    {        
        std::vector<std::string> erasable_union_ids{};
        std::set<std::string> new_union_ids{};
        for (const std::string &union_id : open_union_ids)
        {
            std::vector<std::string> child_comp_ids = unions.at(union_id).at("child_components");
            std::sort(component_mapping_ids.begin(), component_mapping_ids.end());
            std::sort(child_comp_ids.begin(), child_comp_ids.end());

            if (std::includes(component_mapping_ids.begin(), component_mapping_ids.end(), child_comp_ids.begin(), child_comp_ids.end()))
            {
                std::string parent_comp_id{unions.at(union_id).at("parent_component")};
                Subassembly child_subasm{};
                for (const std::string &child_comp_id : child_comp_ids)
                    child_subasm.insert(child_subasm.end(), comp_subasm_mapping.at(child_comp_id).begin(), comp_subasm_mapping.at(child_comp_id).end() );
                comp_subasm_mapping.emplace(std::make_pair(parent_comp_id, child_subasm));
                component_mapping_ids.push_back(parent_comp_id);

                erasable_union_ids.push_back(union_id);
                std::vector<std::string> new_parent_unions = components.at(parent_comp_id).at("parent_unions");
                std::copy(new_parent_unions.begin(), new_parent_unions.end(), std::inserter(new_union_ids, new_union_ids.end()));
            }
        }
        for (const std::string &u : erasable_union_ids)
            open_union_ids.erase(std::find(open_union_ids.begin(), open_union_ids.end(), u));
        open_union_ids.insert(new_union_ids.begin(), new_union_ids.end());
    }

    AndOrGraph<Subassembly> ao_graph{};
    for (const auto &edge : ao_graph_ids.get_edges())
    {
        Subassembly parent_subasm{comp_subasm_mapping.at(edge.first)};
        std::vector<Subassembly> child_subasms{};
        std::transform(edge.second.begin(), edge.second.end(), std::back_inserter(child_subasms), 
            [comp_subasm_mapping](const std::string &child_id)
            {
                return comp_subasm_mapping.at(child_id);
            });
        ao_graph.add_edge(parent_subasm, child_subasms);
    }
    this->ao_graph = ao_graph;

    std::vector<Component> comps{};
    std::transform(leaf_comp_ids.begin(), leaf_comp_ids.end(), std::back_inserter(comps),
        [components](const std::string &leaf_comp_id){
            return Component{components.at(leaf_comp_id).at("label")};
        });
    this->components = comps;
}

void Assembly::import_ao_graph(const std::string &file_path)
{
    std::ifstream file_stream{file_path};
    nlohmann::json data = nlohmann::json::parse(file_stream);
    this->import_ao_graph(data);
}