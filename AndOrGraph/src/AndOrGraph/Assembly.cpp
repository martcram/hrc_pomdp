#include <algorithm>
#include <cmath>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <AndOrGraph/AndOrGraph.hpp>
#include <AndOrGraph/Assembly.hpp>
#include <AndOrGraph/ConnectionGraph.hpp>
#include <AndOrGraph/ObstructionGraph.hpp>
#include <math_utils/math_utils.hpp>

Assembly::Assembly(const std::vector<ObstructionGraph> &obstr_graphs, const ConnectionGraph &connect_graph)
    : parts{}, obstruction_graphs{obstr_graphs}, connection_graph{connect_graph}, ao_graph{}, blocking_rules{}
{
    parts = connection_graph.get_nodes();
    blocking_rules = this->compute_blocking_rules();
}

std::unordered_map<std::string, std::vector<std::vector<std::string>>> Assembly::compute_blocking_rules() const
{
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_parts{};
    for (const auto &node : parts)
        blocking_parts.insert({node, std::vector<std::vector<std::string>>{}});

    for (const auto &obstr_graph : obstruction_graphs)
    {
        for (const auto &node : parts)
        {
            std::vector<std::string> successors{obstr_graph.get_successors(node)};
            std::sort(successors.begin(), successors.end());
            blocking_parts.at(node).push_back(successors);
        }
    }

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_rules{};
    for (const auto &node : parts)
    {
        std::vector<std::vector<std::string>> product{math_utils::cartesian_product(blocking_parts.at(node))};
        for (auto &p : product)
        {
            std::sort(p.begin(), p.end());
            p.erase(std::unique(p.begin(), p.end()), p.end());
        }
        std::sort(product.begin(), product.end());
        product.erase(std::unique(product.begin(), product.end()), product.end());

        blocking_rules.insert({node, product});
    }

    return blocking_rules;
}

bool Assembly::check_geom_feasibility(std::vector<std::string> subassembly, bool is_sorted) const
{
    if (!is_sorted)
        std::sort(subassembly.begin(), subassembly.end());

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

bool Assembly::validate_triplet(std::vector<std::vector<std::string>> triplet) const
{
    std::sort(triplet.at(0).begin(), triplet.at(0).end());
    std::sort(triplet.at(1).begin(), triplet.at(1).end());
    std::sort(triplet.at(2).begin(), triplet.at(2).end());

    std::vector<std::string> subasm_union{};
    std::set_union(triplet.at(0).begin(), triplet.at(0).end(),
                   triplet.at(1).begin(), triplet.at(1).end(),
                   std::back_inserter(subasm_union));

    std::vector<std::string> subasm_intersect{};
    std::set_intersection(triplet.at(0).begin(), triplet.at(0).end(),
                          triplet.at(1).begin(), triplet.at(1).end(),
                          std::back_inserter(subasm_intersect));

    return (subasm_union == triplet.at(2) && subasm_intersect.empty());
}

AndOrGraph Assembly::reversed_cutset() const
{
    size_t num_parts{parts.size()};
    std::map<size_t, std::vector<std::vector<std::string>>> subasm_length_map{};

    std::vector<std::vector<std::string>> one_part_asms{};
    std::transform(parts.begin(), parts.end(), std::back_inserter(one_part_asms),
                   [](const std::string &part) { return std::vector<std::string>{part}; });
    subasm_length_map.insert({1, one_part_asms});

    std::vector<std::vector<std::string>> two_parts_asms{};
    std::vector<std::vector<std::string>> connections{connection_graph.get_edges()};
    std::copy_if(connections.begin(), connections.end(), std::back_inserter(two_parts_asms),
                 [this](const std::vector<std::string> &edge) { return this->check_geom_feasibility(edge); });
    subasm_length_map.insert({2, two_parts_asms});

    std::vector<std::vector<std::string>> complete_asm{parts};
    subasm_length_map.insert({num_parts, complete_asm});

    for (size_t subasm_length{3}; subasm_length < num_parts; ++subasm_length)
    {
        std::vector<std::vector<std::string>> subassemblies{};
        for (const auto &subassembly : subasm_length_map.at(subasm_length - 1))
        {
            std::vector<std::string> subasm_neighbors{this->get_neighbors(subassembly)};
            std::vector<std::vector<std::string>> subassemblies_temp =
                math_utils::cartesian_product(std::vector<std::vector<std::string>>{subassembly}, subasm_neighbors);
            std::copy_if(subassemblies_temp.begin(), subassemblies_temp.end(), std::back_inserter(subassemblies),
                         [this, &subassemblies](auto &subassembly) {
                             std::sort(subassembly.begin(), subassembly.end());
                             return ((std::find(subassemblies.begin(), subassemblies.end(), subassembly) == subassemblies.end()) &&
                                     this->check_geom_feasibility(subassembly, true));
                         });
        }
        subasm_length_map.insert({subasm_length, subassemblies});
    }

    std::vector<std::vector<std::string>> subassemblies{};
    for (auto it = subasm_length_map.rbegin(); it != subasm_length_map.rend(); ++it)
        subassemblies.insert(subassemblies.end(), it->second.begin(), it->second.end());

    std::vector<std::vector<std::vector<std::string>>> cutsets{};
    for (size_t triplet3_len{num_parts}; triplet3_len >= 3; --triplet3_len)
    {
        for (size_t triplet1_len{triplet3_len - 1}; triplet1_len >= std::ceil(triplet3_len / 2.0); --triplet1_len)
        {
            size_t triplet2_len{triplet3_len - triplet1_len};
            std::vector<std::vector<std::vector<std::string>>> triplets{
                math_utils::cartesian_product(std::vector<std::vector<std::vector<std::string>>>{subasm_length_map.at(triplet1_len),
                                                                                                 subasm_length_map.at(triplet2_len),
                                                                                                 subasm_length_map.at(triplet3_len)})};
            std::copy_if(triplets.begin(), triplets.end(), std::back_inserter(cutsets),
                         [this](const auto &triplet) { return this->validate_triplet(triplet); });
        }
    }

    AndOrGraph ao_graph{};
    return ao_graph;
}

std::vector<std::string> Assembly::get_neighbors(const std::vector<std::string> &subassembly) const
{
    std::vector<std::string> asm_neighbors{};
    for (const auto &part : subassembly)
    {
        std::vector<std::string> part_neighbors{connection_graph.get_neighbors(part)};
        std::copy_if(part_neighbors.begin(), part_neighbors.end(), std::back_inserter(asm_neighbors),
                     [&asm_neighbors, &subassembly](std::string &part) { return (std::find(asm_neighbors.begin(), asm_neighbors.end(), part) == asm_neighbors.end()) &&
                                                                                (std::find(subassembly.begin(), subassembly.end(), part) == subassembly.end()); });
    }
    return asm_neighbors;
}