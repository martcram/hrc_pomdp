#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include <graph/I_Plotable.hpp>
#include <graph/I_Printable.hpp>

template <typename T>
class Graph : public I_Plotable,
              public I_Printable
{
private:
    std::unordered_map<T, std::vector<T>> adjacency_list;
    std::unordered_map<T, int> node_indices;

    void add_directed_edge(const T &u, const T &v)
    {
        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end())
            adjacency_list.emplace(std::make_pair(u, std::vector<T>{v}));
        else if (std::find(it->second.begin(), it->second.end(), v) == it->second.end())
            it->second.push_back(v);
    }

    void add_node(const T &node)
    {
        if (node_indices.find(node) == node_indices.end())
            node_indices.emplace(std::make_pair(node, node_indices.size()));
    }

public:
    Graph()
        : adjacency_list{}, node_indices{}
    {
    }

    explicit Graph(const std::vector<std::pair<T, T>> &edges)
        : Graph()
    {
        this->add_edges(edges);
    }

    ~Graph() = default;

    void add_edge(const T &u, const T &v)
    {
        this->add_directed_edge(u, v);
        this->add_directed_edge(v, u);

        this->add_node(u);
        this->add_node(v);
    }

    void add_edges(const std::vector<std::pair<T, T>> &edges)
    {
        for (const auto &edge : edges)
            this->add_edge(edge.first, edge.second);
    }

    std::vector<T> get_neighbors(const T &u) const
    {
        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end())
            return std::vector<T>{};
        else
            return it->second;
    }

    std::vector<T> get_nodes() const
    {
        std::vector<T> nodes{};
        std::transform(node_indices.begin(), node_indices.end(), std::back_inserter(nodes),
                       [](const auto &node_index_map) { return node_index_map.first; });
        return nodes;
    }

    std::vector<std::pair<T, T>> get_edges() const
    {
        std::vector<std::pair<T, T>> edges{};
        for (auto i = adjacency_list.begin(); i != adjacency_list.end(); ++i)
        {
            for (auto j = i->second.begin(); j != i->second.end(); ++j)
            {
                std::pair<T, T> edge = ((i->first < *j) ? std::make_pair(i->first, *j) : std::make_pair(*j, i->first));
                if (std::find(edges.begin(), edges.end(), edge) == edges.end())
                    edges.push_back(edge);
            }
        }
        return edges;
    }

    virtual std::stringstream plot() const override
    {
        std::stringstream ss{};
        for (const auto &edge : this->get_edges())
            ss << node_indices.at(edge.first) << "->" << node_indices.at(edge.second)
               << '\n';
        return ss;
    }

    virtual void print(std::ostream &os) const override
    {
        for (auto it = adjacency_list.begin(); it != adjacency_list.end(); ++it)
        {
            os << it->first << '\n';

            for (const auto &successor_state : it->second)
            {
                os << std::setw(10) << ' '
                   << "-- " << successor_state << '\n';
            }
            os << '\n';
        }
    }
};

#endif // GRAPH_HPP