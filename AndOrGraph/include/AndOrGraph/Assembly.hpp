#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <vector>
#include <AndOrGraph/ObstructionGraph.hpp>

class Assembly
{
private:
    std::vector<ObstructionGraph> obstruction_graphs;

public:
    explicit Assembly(const std::vector<ObstructionGraph> &obstr_graphs);
    ~Assembly() = default;
};

#endif // ASSEMBLY_HPP