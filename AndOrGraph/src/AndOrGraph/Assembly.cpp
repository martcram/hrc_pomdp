#include <algorithm>
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

bool Assembly::check_geom_feasibility(std::vector<std::string> subassembly) const
{
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
                                     this->check_geom_feasibility(subassembly));
                         });
        }
        subasm_length_map.insert({subasm_length, subassemblies});
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