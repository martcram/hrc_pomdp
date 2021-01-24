#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <graph/Plotter.hpp>

#include <graphviz/gvc.h>

#include <boost/algorithm/string.hpp>

Plotter::Plotter(std::shared_ptr<I_Plotable> obj, const std::string &file_path)
    : obj{obj}, file_path{file_path}
{
}

void Plotter::generate()
{
    std::string file_loc{file_path + ".dot"};
    std::ofstream output_file{file_loc, std::ios_base::trunc};
    if (!output_file.is_open())
        std::cerr << "ERROR: Couldn't open output file: " << file_loc;
    else
    {
        std::vector<std::string> split_temp{};
        boost::split(split_temp, file_path, boost::is_any_of("/"));

        output_file << "digraph " << split_temp.back() << "{\n"
                    << obj->plot().str()
                    << "}\n";
    }
    output_file.close();
}

void Plotter::render() const
{
    std::FILE *fp = std::fopen((file_path + ".dot").c_str(), "r");
    GVC_t *gvc = gvContext();
    Agraph_t *g = agread(fp, 0);

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", std::fopen((file_path + ".png").c_str(), "w"));

    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}