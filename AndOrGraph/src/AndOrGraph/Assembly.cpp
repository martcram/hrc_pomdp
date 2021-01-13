#include <vector>

#include <AndOrGraph/Assembly.hpp>
#include <AndOrGraph/ObstructionGraph.hpp>

Assembly::Assembly(const std::vector<ObstructionGraph> &obstr_graphs)
: obstruction_graphs{obstr_graphs}
{
}