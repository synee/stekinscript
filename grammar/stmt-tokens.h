#ifndef __STEKIN_GRAMMAR_STATEMENT_TOKEN_H__
#define __STEKIN_GRAMMAR_STATEMENT_TOKEN_H__

#include "automation-base.h"

namespace grammar {

    struct IfToken
        : Token
    {
        explicit IfToken(misc::position const& pos)
            : Token(pos, "")
        {}

        void act(AutomationStack& stack);
    };

    struct ElseToken
        : Token
    {
        explicit ElseToken(misc::position const& pos)
            : Token(pos, "")
        {}

        void act(AutomationStack& stack);
    };

}

#endif /* __STEKIN_GRAMMAR_STATEMENT_TOKEN_H__ */
