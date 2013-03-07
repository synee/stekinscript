#include "methods.h"

using namespace output;

Method method::throwExc()
{
    return [](std::string const& exception)
           {
               return "throw " + exception + ";";
           };
}

Method method::callbackExc()
{
    return [](std::string const& exception)
           {
               return "return $c_$racb(" + exception + ");";
           };
}
