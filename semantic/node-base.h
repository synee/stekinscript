#ifndef __STEKIN_SEMANTIC_NODE_BASE_H__
#define __STEKIN_SEMANTIC_NODE_BASE_H__

#include <string>
#include <vector>
#include <utility>
#include <gmpxx.h>

#include <output/fwd-decl.h>
#include <util/pointer.h>
#include <misc/pos-type.h>

#include "fwd-decl.h"

namespace semantic {

    struct Statement {
        misc::position const pos;

        virtual void compile(util::sref<SymbolTable> st, util::sref<output::Block> block) const = 0;

        virtual ~Statement() {}
    protected:
        explicit Statement(misc::position const& ps)
            : pos(ps)
        {}

        Statement(Statement const&) = delete;
    };

    struct Expression {
        misc::position const pos;

        virtual util::sptr<output::Expression const> compile(util::sref<SymbolTable> st) const = 0;
        virtual bool isLiteral(util::sref<SymbolTable const> st) const;
        virtual std::string literalType(util::sref<SymbolTable const> st) const;

        virtual bool boolValue(util::sref<SymbolTable const> st) const;
        virtual mpz_class intValue(util::sref<SymbolTable const> st) const;
        virtual mpf_class floatValue(util::sref<SymbolTable const> st) const;
        virtual std::string stringValue(util::sref<SymbolTable const> st) const;

        virtual ~Expression() {}

        Expression(misc::position const& ps)
            : pos(ps)
        {}

        Expression(Expression const&) = delete;
    };

}

#endif /* __STEKIN_SEMANTIC_NODE_BASE_H__ */