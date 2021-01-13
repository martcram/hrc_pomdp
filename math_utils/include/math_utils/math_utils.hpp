#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <utility>
#include <vector>

namespace math_utils
{
    template <typename T>
    std::vector<std::pair<T, T>> cartesian_product(const std::vector<T> &r1, const std::vector<T> &r2)
    {
        std::vector<std::pair<T, T>> product{};
        for (auto i = r1.begin(); i != r1.end(); ++i)
        {
            for (auto j = r2.begin(); j != r2.end(); ++j)
            {
                product.push_back(std::make_pair(*i, *j));
            }
        }
        return product;
    }
} // namespace math_utils

#endif // MATH_UTILS_HPP