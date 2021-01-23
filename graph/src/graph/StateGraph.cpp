#include <iomanip>
#include <ostream>

#include <graph/DiGraph.hpp>
#include <graph/Node.hpp>
#include <graph/StateGraph.hpp>

StateGraph::StateGraph()
    : DiGraph<state_t>{}
{
}

StateGraph::StateGraph(const std::vector<std::pair<state_t, state_t>> &edges)
    : DiGraph<state_t>{edges}
{
}

std::ostream &operator<<(std::ostream &os, const state_t &state)
{
    for (const auto &node : state)
        os << node << ' ';
    return os;
}

void StateGraph::print(std::ostream &os) const
{
    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); ++it)
    {
        os << it->first << '\n';

        for (const auto &successor_state : it->second)
        {
            os << std::setw(10) << ' '
               << "--> " << successor_state << '\n';
        }
        os << '\n';
    }
}