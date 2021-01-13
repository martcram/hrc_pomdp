#ifndef CONNECTION_GRAPH_HPP
#define CONNECTION_GRAPH_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ConnectionGraph
{
private:
    std::unordered_map<std::string, std::vector<std::string>> adjacency_list;

    void add_directed_edge(const std::pair<std::string, std::string> &edge);

public:
    ConnectionGraph();
    explicit ConnectionGraph(const std::vector<std::pair<std::string, std::string>> &edges);
    ~ConnectionGraph() = default;

    void add_edge(const std::pair<std::string, std::string> &edge);
    void add_edges_from(const std::vector<std::pair<std::string, std::string>> &edges);

    std::vector<std::string> get_nodes() const;
    std::vector<std::string> get_neighbors(const std::string &node) const;
};

#endif // CONNECTION_GRAPH_HPP