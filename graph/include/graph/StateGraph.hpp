#ifndef STATE_GRAPH_HPP
#define STATE_GRAPH_HPP

#include <graph/DiGraph.hpp>
#include <graph/Node.hpp>

using state_t = std::vector<Node>;

class StateGraph : public I_Printable,
                   public DiGraph<state_t>
{
public:
    StateGraph();
    explicit StateGraph(const std::vector<std::pair<state_t, state_t>> &edges);
    ~StateGraph() = default;

    virtual void print(std::ostream &os) const override;
    friend std::ostream &operator<<(std::ostream &os, const state_t &state);
};

#endif // STATE_GRAPH_HPP