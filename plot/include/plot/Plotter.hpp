#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <memory> // std::shared_ptr
#include <string> // std::string

#include <plot/I_Plotable.hpp>

class Plotter
{
private:
    std::shared_ptr<I_Plotable> obj;
    std::string file_loc;
    
public:
    explicit Plotter(std::shared_ptr<I_Plotable> obj, const std::string &file_loc);
    ~Plotter() = default;

    void generate_dot();
    void render_dot() const;
};

#endif // PLOTTER_HPP