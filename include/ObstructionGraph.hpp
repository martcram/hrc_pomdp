#ifndef OBSTRUCTION_GRAPH_HPP
#define OBSTRUCTION_GRAPH_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ObstructionGraph
{
private:
    std::unordered_map<std::string, std::vector<std::string>> adjacency_list;

public:
    ObstructionGraph();
    explicit ObstructionGraph(const std::vector<std::pair<std::string, std::string>> &edges);
    ~ObstructionGraph() = default;

    void add_edge(const std::pair<std::string, std::string> &edge);
    void add_edges_from(const std::vector<std::pair<std::string, std::string>> &edges);
    ObstructionGraph reverse();
};

#endif // OBSTRUCTION_GRAPH_HPP