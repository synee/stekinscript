#include <gtest/gtest.h>

#include <output/node-base.h>
#include <test/phony-errors.h>
#include <test/common.h>

#include "test-common.h"
#include "../function.h"
#include "../expr-nodes.h"
#include "../stmt-nodes.h"
#include "../compiling-space.h"

using namespace test;

typedef SemanticTest FunctionTest;

TEST_F(FunctionTest, RegularAsyncFunction)
{
    misc::position pos(1);
    semantic::CompilingSpace space;
    util::ptrarr<semantic::Expression const> fargs;
    util::ptrarr<semantic::Expression const> largs;
    semantic::Block body;
    space.sym()->defName(pos, "setTimeout");

    largs.append(util::mkptr(new semantic::IntLiteral(pos, "1600")));
    body.addStmt(util::mkptr(new semantic::Arithmetics(pos, util::mkptr(new semantic::AsyncCall(
                                        pos
                                      , util::mkptr(new semantic::Reference(pos, "setTimeout"))
                                      , std::move(fargs)
                                      , std::vector<std::string>()
                                      , std::move(largs))))));
    body.addStmt(util::mkptr(
                new semantic::Return(pos, util::mkptr(new semantic::Reference(pos, "light")))));
    semantic::RegularAsyncFunction af(
                pos, "li", std::vector<std::string>({ "light", "dark" }), 1, std::move(body));

    af.compile(space.sym())->write(dummyos());
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (FUNCTION, "li", 3)
            (PARAMETER, "light")
            (PARAMETER, "# RegularAsyncParam")
            (PARAMETER, "dark")
            (SCOPE_BEGIN)
                (ASYNC_RESULT_DEF)
                    (pos, CALL, 2)
                        (pos, REFERENCE, "setTimeout")
                        (pos, FUNCTION, 0)
                            (COPY_PARAM_DECL)
                            (SCOPE_BEGIN)
                                (ARITHMETICS)
                                    (pos, ASYNC_REFERENCE)
                                (RETURN)
                                    (pos, REGULAR_ASYNC_RETURN)
                                        (pos, REFERENCE, "light")
                                (RETURN)
                                    (pos, REGULAR_ASYNC_RETURN)
                                        (pos, UNDEFINED)
                            (SCOPE_END)
                        (pos, INTEGER, "1600")
            (SCOPE_END)
    ;
}

TEST_F(FunctionTest, RegularAsyncCallInRegularAsyncFunction)
{
    misc::position pos(2);
    semantic::CompilingSpace space;
    util::ptrarr<semantic::Expression const> fargs;
    util::ptrarr<semantic::Expression const> largs;
    semantic::Block body;
    space.sym()->defName(pos, "x20130308");

    largs.append(util::mkptr(new semantic::StringLiteral(pos, "200d0308")));
    body.addStmt(util::mkptr(new semantic::Return(pos, util::mkptr(
                        new semantic::RegularAsyncCall(
                                        pos
                                      , util::mkptr(new semantic::Reference(pos, "x20130308"))
                                      , std::move(fargs)
                                      , std::move(largs))))));
    semantic::RegularAsyncFunction af(
                pos, "yukito", std::vector<std::string>({ "touya" }), 1, std::move(body));

    af.compile(space.sym())->write(dummyos());
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (FUNCTION, "yukito", 2)
            (PARAMETER, "touya")
            (PARAMETER, "# RegularAsyncParam")
            (SCOPE_BEGIN)
                (ARITHMETICS)
                    (pos, CALL, 2)
                        (pos, REFERENCE, "x20130308")
                        (pos, FUNCTION)
                            (PARAMETER, "# RegularAsyncCallbackParameters")
                            (EXC_CALLBACK)
                            (SCOPE_BEGIN)
                                (RETURN)
                                    (pos, REGULAR_ASYNC_RETURN)
                                        (pos, ASYNC_REFERENCE)
                                (RETURN)
                                    (pos, REGULAR_ASYNC_RETURN)
                                        (pos, UNDEFINED)
                            (SCOPE_END)
                        (pos, STRING, "200d0308")
            (SCOPE_END)
    ;
}
