#ifndef POMDPX_WRITER_HPP
#define POMDPX_WRITER_HPP

#include <string> // std::string

#include <pomdp/Pomdp.hpp>

#include "tinyxml2.h"

class PomdpxWriter
{
private:
    Pomdp pomdp;

    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLElement *root_ptr;

    std::string state_var_tag;
    std::string obs_var_tag;
    std::string action_var_tag;
    std::string reward_var_tag;

    void _gen_header();
    void _gen_description();
    void _gen_discount();
    void _gen_variables();
    void _gen_init_belief();
    void _gen_state_trans_func();
    void _gen_obs_func();
    void _gen_reward_func();

public:
    explicit PomdpxWriter(const Pomdp &pomdp);
    ~PomdpxWriter() = default;

    void write(const std::string &file_path);
};

#endif // POMDPX_WRITER_HPP