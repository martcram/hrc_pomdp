#include <algorithm> // std::any_of, std::copy, std::transform
#include <iterator>  // std::back_inserter, std::istream_iterator, std::ostream_iterator
#include <numeric>   // std::accumulate
#include <sstream>   // std::istringstream, std::ostringstream, std::stringstream
#include <string>    // std::getline, std::string
#include <utility>   // std::make_pair, std::pair
#include <vector>    // std::vector

template <typename T>
std::vector<std::pair<T, T>> utils::cartesian_product(const std::vector<T> &r1, const std::vector<T> &r2)
{
    // If either of the two vectors is empty, the Cartesian product of those two vectors if also empty.
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
std::vector<std::vector<T>> utils::cartesian_product(const std::vector<std::vector<T>> &r1, const std::vector<T> &r2)
{
    // If either of the sets is empty, the Cartesian product of those sets is also empty.
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

template <typename T>
std::vector<std::vector<T>> utils::cartesian_product(const std::vector<std::vector<T>> &v)
{
    // If either of the vectors is empty, the Cartesian product of those vectors is also empty
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

template <typename T>
T utils::cast_cstring(const char *cstring)
{
    std::stringstream ss{};
    ss << cstring;

    T output{};
    ss >> output;
    return output;
}

template <typename T>
std::vector<T> utils::split_cstring(const char *cstring)
{
    std::istringstream ss{cstring};
    std::vector<T> container(std::istream_iterator<T>{ss}, std::istream_iterator<T>{});
    return container;
}

template <typename T>
std::vector<T> utils::split_string(const std::string &string, char delimiter)
{
    std::istringstream ss{string};
    std::string segment{};
    std::vector<T> container{};

    while (std::getline(ss, segment, delimiter))
        container.push_back(static_cast<T>(segment));
    return container;
}

template <typename T>
std::string utils::vector_to_string(const std::vector<T> &container, const char *delimiter)
{
    std::ostringstream ss{};
    std::copy(container.begin(), container.end() - 1,
              std::ostream_iterator<T>(ss, delimiter));
    ss << container.back();
    return ss.str();
}

template <typename T>
T utils::dot_product(const std::vector<T> &v1, const std::vector<T> &v2)
{
    std::vector<T> v{};
    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(v), std::multiplies<T>());
    return std::accumulate(v.begin(), v.end(), T{});
}