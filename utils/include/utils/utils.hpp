#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>  // std::string
#include <utility> // std::pair
#include <vector>  // std::vector

namespace utils
{
    template <typename T>
    std::vector<std::pair<T, T>> cartesian_product(const std::vector<T> &r1, const std::vector<T> &r2);

    template <typename T>
    std::vector<std::vector<T>> cartesian_product(const std::vector<std::vector<T>> &r1, const std::vector<T> &r2);

    template <typename T>
    std::vector<std::vector<T>> cartesian_product(const std::vector<std::vector<T>> &v);

    template <typename T>
    T cast_cstring(const char *cstring);

    template <typename T>
    std::vector<T> split_cstring(const char *cstring);

    template <typename T>
    std::vector<T> split_string(const std::string &string, char delimiter = '\0');

    template <typename T>
    std::string vector_to_string(const std::vector<T> &container, const char *delimiter = "");

    template <typename T>
    T dot_product(const std::vector<T> &v1, const std::vector<T> &v2);

    std::string to_snake_case(const std::string &string);
} // namespace utils

#include <utils/utils.tpp>

#endif // UTILS_HPP