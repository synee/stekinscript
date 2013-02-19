#include <gtest/gtest.h>

#include <test/phony-errors.h>

#include "test-common.h"
#include "../expr-nodes.h"

using namespace test;

typedef GrammarTest ExprNodesTest;

TEST_F(ExprNodesTest, Pipeline)
{
    misc::position pos(1);
    semantic::CompilingSpace space;

    util::sptr<grammar::Expression const> p(new grammar::Pipeline(
                pos
              , util::mkptr(new grammar::Identifier(pos, "x20130109"))
              , "|:"
              , util::mkptr(new grammar::PipeElement(pos))));
    p->reduceAsExpr()->compile(space);
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (pos, BINARY_OP, "[ |: ]")
        (pos, OPERAND)
            (pos, IDENTIFIER, "x20130109")
        (pos, OPERAND)
            (pos, PIPE_ELEMENT)
    ;
}
