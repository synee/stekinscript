#include <algorithm>

#include "name-mangler.h"

std::string output::formName(std::string const& name)
{
    return "s_" + name;
}

std::vector<std::string> output::formNames(std::vector<std::string> const& names)
{
    std::vector<std::string> result;
    std::for_each(names.begin()
                , names.end()
                , [&](std::string const& name)
                  {
                      result.push_back(formName(name));
                  });
    return std::move(result);
}