#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include <AndOrGraph/Assembly.hpp>
#include <AndOrGraph/ConnectionGraph.hpp>
#include <AndOrGraph/ObstructionGraph.hpp>
#include <math_utils/math_utils.hpp>

Assembly::Assembly(const std::vector<ObstructionGraph> &obstr_graphs, const ConnectionGraph &connect_graph)
    : obstruction_graphs{obstr_graphs}, connection_graph{connect_graph}, blocking_rules{}
{
    blocking_rules = this->compute_blocking_rules();
}

std::unordered_map<std::string, std::vector<std::vector<std::string>>> Assembly::compute_blocking_rules() const
{
    std::vector<std::string> nodes{obstruction_graphs.at(0).get_nodes()};

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_parts{};
    for (const auto &node : nodes)
        blocking_parts.insert({node, std::vector<std::vector<std::string>>{}});

    for (const auto &obstr_graph : obstruction_graphs)
    {
        for (const auto &node : nodes)
        {
            std::vector<std::string> successors{obstr_graph.get_successors(node)};
            std::sort(successors.begin(), successors.end());
            blocking_parts.at(node).push_back(successors);
        }
    }

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> blocking_rules{};
    for (const auto &node : nodes)
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