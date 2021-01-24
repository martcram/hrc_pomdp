#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <memory>
#include <sstream>
#include <string>

#include <graph/I_Plotable.hpp>

class Plotter
{
private:
    std::shared_ptr<I_Plotable> obj;
    std::string file_path;
    
public:
    explicit Plotter(std::shared_ptr<I_Plotable> obj, const std::string &file_path);
    ~Plotter() = default;

    void generate();
    void render() const;
};

#endif // PLOTTER_HPP