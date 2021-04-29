#include <cstdio>   // std::FILE, std::fopen
#include <fstream>  // std::ofstream
#include <ios>      // std::ios_base
#include <iostream> // std::cerr
#include <memory>   // std::shared_ptr
#include <string>   // std::string

#include <graphviz/gvc.h>

#include <plot/I_Plotable.hpp>
#include <plot/Plotter.hpp>

#include <utils/utils.hpp>

Plotter::Plotter(std::shared_ptr<I_Plotable> obj, const std::string &file_loc)
    : obj{obj}, file_loc{}
{
    this->file_loc = file_loc + "/" + utils::to_snake_case(this->obj->get_name());
}

void Plotter::generate_dot()
{
    std::string file_path{this->file_loc + ".dot"};
    std::ofstream output_file{file_path, std::ios_base::trunc};
    if (!output_file.is_open())
        std::cerr << "[Plotter]: Couldn't open output file: " << file_path;
    else
        output_file << this->obj->generate_dot().rdbuf();
    output_file.close();
}

void Plotter::render_dot() const
{
    std::FILE *fp = std::fopen((this->file_loc + ".dot").c_str(), "r");
    GVC_t *gvc = gvContext();
    Agraph_t *g = agread(fp, 0);

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", std::fopen((this->file_loc + ".png").c_str(), "w"));

    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}