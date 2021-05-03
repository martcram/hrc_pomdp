#include <algorithm> // std::for_each
#include <cctype>    // std::isspace
#include <string>    // std::string

#include <utils/utils.hpp>

std::string utils::to_snake_case(const std::string &string)
{
    std::string snake_case_string{string};
    std::for_each(snake_case_string.begin(), snake_case_string.end(),
                  [](char &c) {
                      if (std::isspace(c))
                          c = '_';
                  });
    return snake_case_string;
}
