#ifndef __STEKIN_OUTPUT_FUNCTION_H__
#define __STEKIN_OUTPUT_FUNCTION_H__

#include "fwd-decl.h"
#include "block.h"

namespace output {

    struct Function {
        Function(misc::position const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , util::sptr<Statement const> b)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , body(std::move(b))
        {}

        Function(Function const&) = delete;

        misc::position const pos;
        std::string const name;
        std::vector<std::string> const param_names;
        util::sptr<Statement const> const body;

        void write(std::ostream& os) const;
    };

}

#endif /* __STEKIN_OUTPUT_FUNCTION_H__ */
