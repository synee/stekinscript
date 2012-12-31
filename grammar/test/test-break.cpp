#include <gtest/gtest.h>

#include <test/phony-errors.h>

#include "test-common.h"
#include "../yy-misc.h"

using namespace test;

TEST(Syntax, Break)
{
    yyparse();
    grammar::builder.buildAndClear()->compile(nulSymbols());
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (BLOCK_BEGIN)
            (misc::position(1), NAME_DEF, "x")
                (misc::position(1), BINARY_OP, "*")
                (misc::position(1), OPERAND)
                    (misc::position(1), IDENTIFIER, "y")
                (misc::position(1), OPERAND)
                    (misc::position(2), IDENTIFIER, "z")
            (misc::position(3), NAME_DEF, "x")
                (misc::position(3), BINARY_OP, "+")
                (misc::position(3), OPERAND)
                    (misc::position(3), IDENTIFIER, "y")
                (misc::position(3), OPERAND)
                    (misc::position(4), IDENTIFIER, "z")
            (misc::position(5), NAME_DEF, "x")
                (misc::position(5), BINARY_OP, "||")
                (misc::position(5), OPERAND)
                    (misc::position(5), IDENTIFIER, "y")
                (misc::position(5), OPERAND)
                    (misc::position(6), IDENTIFIER, "z")
            (misc::position(7), NAME_DEF, "x")
                (misc::position(7), BINARY_OP, "&&")
                (misc::position(7), OPERAND)
                    (misc::position(7), IDENTIFIER, "y")
                (misc::position(7), OPERAND)
                    (misc::position(8), IDENTIFIER, "z")
            (misc::position(9), NAME_DEF, "x")
                (misc::position(10), BINARY_OP, "+")
                (misc::position(10), OPERAND)
                    (misc::position(9), BINARY_OP, "+")
                    (misc::position(9), OPERAND)
                        (misc::position(9), IDENTIFIER, "a")
                    (misc::position(9), OPERAND)
                        (misc::position(9), BINARY_OP, "*")
                        (misc::position(9), OPERAND)
                            (misc::position(9), IDENTIFIER, "b")
                        (misc::position(9), OPERAND)
                            (misc::position(10), IDENTIFIER, "c")
                (misc::position(10), OPERAND)
                    (misc::position(10), IDENTIFIER, "d")
            (misc::position(11), NAME_DEF, "x")
                (misc::position(11), BINARY_OP, "=")
                (misc::position(11), OPERAND)
                    (misc::position(11), BINARY_OP, "*")
                    (misc::position(11), OPERAND)
                        (misc::position(11), IDENTIFIER, "a")
                    (misc::position(11), OPERAND)
                        (misc::position(11), IDENTIFIER, "b")
                (misc::position(11), OPERAND)
                    (misc::position(12), BINARY_OP, "*")
                    (misc::position(12), OPERAND)
                        (misc::position(12), IDENTIFIER, "c")
                    (misc::position(12), OPERAND)
                        (misc::position(12), IDENTIFIER, "d")
            (misc::position(13), NAME_DEF, "x")
                (misc::position(14), LIST_BEGIN)
                    (misc::position(14), IDENTIFIER, "y")
                (misc::position(14), LIST_END)
            (misc::position(15), BRANCH_ALTER_ONLY)
                (misc::position(16), LIST_BEGIN)
                    (misc::position(15), IDENTIFIER, "y")
                    (misc::position(16), IDENTIFIER, "z")
                (misc::position(16), LIST_END)
            (ALTERNATIVE)
            (BLOCK_BEGIN)
                (misc::position(17), ARITHMETICS)
                    (misc::position(17), CALL_BEGIN)
                        (misc::position(17), IDENTIFIER, "x")
                    (misc::position(17), ARGUMENTS)
                    (misc::position(17), CALL_END)
            (BLOCK_END)
            (misc::position(19), ARITHMETICS)
                (misc::position(19), CALL_BEGIN)
                    (misc::position(19), IDENTIFIER, "x")
                (misc::position(19), ARGUMENTS)
                    (misc::position(20), IDENTIFIER, "y")
                (misc::position(19), CALL_END)
            (misc::position(21), ARITHMETICS)
                (misc::position(21), CALL_BEGIN)
                    (misc::position(21), IDENTIFIER, "x")
                (misc::position(21), ARGUMENTS)
                    (misc::position(21), IDENTIFIER, "y")
                    (misc::position(22), IDENTIFIER, "z")
                (misc::position(21), CALL_END)
            (misc::position(24), BRANCH_CONSQ_ONLY)
                (misc::position(24), BINARY_OP, "=")
                (misc::position(24), OPERAND)
                    (misc::position(24), BINARY_OP, "*")
                    (misc::position(24), OPERAND)
                        (misc::position(24), IDENTIFIER, "a")
                    (misc::position(24), OPERAND)
                        (misc::position(24), IDENTIFIER, "b")
                (misc::position(24), OPERAND)
                    (misc::position(25), BINARY_OP, "*")
                    (misc::position(25), OPERAND)
                        (misc::position(25), IDENTIFIER, "c")
                    (misc::position(25), OPERAND)
                        (misc::position(26), IDENTIFIER, "d")
            (CONSEQUENCE)
            (BLOCK_BEGIN)
                (misc::position(27), ARITHMETICS)
                    (misc::position(27), CALL_BEGIN)
                        (misc::position(27), BINARY_OP, "[ . ]")
                        (misc::position(27), OPERAND)
                            (misc::position(27), IDENTIFIER, "console")
                        (misc::position(27), OPERAND)
                            (misc::position(27), IDENTIFIER, "log")
                    (misc::position(27), ARGUMENTS)
                        (misc::position(28), IDENTIFIER, "x")
                    (misc::position(27), CALL_END)
            (BLOCK_END)
            (misc::position(29), ARITHMETICS)
                (misc::position(29), IDENTIFIER, "over")
        (BLOCK_END)
    ;
    DataTree::verify();
}