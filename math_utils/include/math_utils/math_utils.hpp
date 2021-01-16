#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <algorithm>
#include <utility>
#include <vector>

namespace math_utils
{
    template <typename T>
    std::vector<std::pair<T, T>> cartesian_product(const std::vector<T> &r1, const std::vector<T> &r2)
    {
        // If either of the two sets is empty; the Cartesian product of those two sets if also empty
        if (r1.empty() || r2.empty())
            return std::vector<std::pair<T, T>>{};

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

    template <typename T>
    std::vector<std::vector<T>> cartesian_product(const std::vector<std::vector<T>> &r1, const std::vector<T> &r2)
    {
        // If either of the sets is empty; the Cartesian product of those sets is also empty
        if (r1.empty() || std::any_of(r1.begin(), r1.end(), [](const auto &item) { return item.empty(); }) || r2.empty())
            return std::vector<std::vector<T>>{};

        std::vector<std::vector<T>> product{};
        for (auto i = r1.begin(); i != r1.end(); ++i)
        {
            for (auto j = r2.begin(); j != r2.end(); ++j)
            {
                std::vector<T> prod{*i};
                prod.push_back(*j);
                product.push_back(prod);
            }
        }
        return product;
    }

    // Along the lines of Anumi (2016): https://stackoverflow.com/questions/5279051
    template <typename T>
    std::vector<std::vector<T>> cartesian_product(const std::vector<std::vector<T>> &v)
    {
        // If either of the sets is empty; the Cartesian product of those sets is also empty
        if (std::any_of(v.begin(), v.end(), [](const std::vector<T> &u) { return u.empty(); }))
            return std::vector<std::vector<T>>{};

        std::vector<std::vector<T>> s{{}};
        for (const auto &u : v)
        {
            std::vector<std::vector<T>> r;
            for (const auto &x : s)
            {
                for (const auto &y : u)
                {
                    r.push_back(x);
                    r.back().push_back(y);
                }
            }
            s = std::move(r);
        }
        return s;
    }
} // namespace math_utils

#endif // MATH_UTILS_HPP