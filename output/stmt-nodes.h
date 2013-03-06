#ifndef __STEKIN_OUTPUT_STATEMENT_NODES_H__
#define __STEKIN_OUTPUT_STATEMENT_NODES_H__

#include <vector>

#include "node-base.h"

namespace output {

    struct Branch
        : Statement
    {
        Branch(util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a)
                : predicate(std::move(p))
                , consequence(std::move(c))
                , alternative(std::move(a))
        {}

        void write(std::ostream& os) const;

        util::sptr<Expression const> const predicate;
        util::sptr<Statement const> const consequence;
        util::sptr<Statement const> const alternative;
    };

    struct Arithmetics
        : Statement
    {
        explicit Arithmetics(util::sptr<Expression const> e)
            : expr(std::move(e))
        {}

        void write(std::ostream& os) const;

        util::sptr<Expression const> const expr;
    };

    struct NameDef
        : Statement
    {
        NameDef(std::string const& n, util::sptr<Expression const> i)
            : name(n)
            , init(std::move(i))
        {}

        void write(std::ostream& os) const;

        std::string const name;
        util::sptr<Expression const> const init;
    };

    struct AsyncCallResultDef
        : Statement
    {
        explicit AsyncCallResultDef(util::sptr<Expression const> ar)
            : async_result(std::move(ar))
        {}

        void write(std::ostream& os) const;

        util::sptr<Expression const> const async_result;
    };

    struct Return
        : Statement
    {
        explicit Return(util::sptr<Expression const> r)
            : ret_val(std::move(r))
        {}

        void write(std::ostream& os) const;

        util::sptr<Expression const> const ret_val;
    };

    struct Export
        : Statement
    {
        Export(std::vector<std::string> const& e, util::sptr<Expression const> v)
            : export_point(e)
            , value(std::move(v))
        {}

        void write(std::ostream& os) const;
        int count() const;

        std::vector<std::string> const export_point;
        util::sptr<Expression const> const value;
    };

    struct AttrSet
        : Statement
    {
        AttrSet(util::sptr<Expression const> s, util::sptr<Expression const> v)
            : set_point(std::move(s))
            , value(std::move(v))
        {}

        void write(std::ostream& os) const;

        util::sptr<Expression const> const set_point;
        util::sptr<Expression const> const value;
    };

    struct ThisDeclaration
        : Statement
    {
        void write(std::ostream& os) const;
    };

}

#endif /* __STEKIN_OUTPUT_STATEMENT_NODES_H__ */
