#include <algorithm>
#include <unordered_map>
#include <vector>

#include <AndOrGraph/Assembly.hpp>
#include <AndOrGraph/ObstructionGraph.hpp>
#include <math_utils/math_utils.hpp>

Assembly::Assembly(const std::vector<ObstructionGraph> &obstr_graphs)
    : obstruction_graphs{obstr_graphs}
{
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