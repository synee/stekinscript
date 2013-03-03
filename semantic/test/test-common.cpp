#include <sstream>

#include <test/common.h>
#include <test/phony-errors.h>
#include <test/phony-warnings.h>
#include <semantic/compiling-space.h>
#include <output/function.h>
#include <util/string.h>

#include "test-common.h"

using namespace test;

std::ostream& test::dummyos()
{
    static std::ostringstream os;
    return os;
}

util::sptr<output::Statement const> test::compile(
        semantic::Filter& f, util::sref<semantic::SymbolTable> sym)
{
    return f.deliver().compile(
            semantic::CompilingSpace(misc::position(), sym, std::vector<std::string>()));
}

DataTree& DataTree::operator()(misc::position const& pos
                             , NodeType const& type
                             , std::string const& str)
{
    BaseType::operator()(type, SemanticData(pos), str);
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos, NodeType const& type)
{
    BaseType::operator()(type, SemanticData(pos));
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos, NodeType const& type, int size)
{
    BaseType::operator()(type, SemanticData(pos, size));
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos
                             , NodeType const& type
                             , std::string const& str
                             , int size)
{
    BaseType::operator()(type, SemanticData(pos, size), str);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type)
{
    BaseType::operator()(type, SemanticData());
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, std::string const& str)
{
    BaseType::operator()(type, SemanticData(), str);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, int value)
{
    BaseType::operator()(type, SemanticData(misc::position(), value));
    return *this;
}

std::string SemanticData::str() const
{
    std::ostringstream os;
    -1 == int_val ? (os << pos) : (os << pos << " int value=" << int_val);
    return os.str();
}

NodeType const test::BOOLEAN("boolean");
NodeType const test::INTEGER("integer");
NodeType const test::FLOATING("floating");
NodeType const test::STRING("string");
NodeType const test::LIST("list");
NodeType const test::THIS("this");
NodeType const test::BINARY_OP("binary operation");
NodeType const test::PRE_UNARY_OP("prefix unary operation");
NodeType const test::REFERENCE("reference");
NodeType const test::IMPORTED_NAME("imported name");
NodeType const test::PIPE_ELEMENT("list element");
NodeType const test::PIPE_INDEX("list index");
NodeType const test::PIPE_KEY("list key");

NodeType const test::CALL("call");
NodeType const test::ASYNC_REFERENCE("asynchronous reference");

NodeType const test::ASYNC_PIPE_RESULT("asynchronous pipe result");
NodeType const test::ASYNC_PIPELINE("asynchronous pipeline");
NodeType const test::SYNC_PIPELINE("synchronous pipeline");
NodeType const test::PIPELINE_RETURN("pipeline return");
NodeType const test::PIPELINE_CONTINUE("pipeline continue");

NodeType const test::LIST_SLICE("list slice");
NodeType const test::LIST_SLICE_DEFAULT("list slice default");

NodeType const test::DICT_BEGIN("dictionary begin");
NodeType const test::DICT_END("dictionary end");
NodeType const test::DICT_ITEM("dictionary item");

NodeType const test::NAME_DEF("name definition");
NodeType const test::ASYNC_RESULT_DEF("asynchronous result definition");

NodeType const test::STATEMENT("statement");
NodeType const test::ARITHMETICS("arithmetics");
NodeType const test::RETURN("return");
NodeType const test::RETURN_NOTHING("return nothing");
NodeType const test::EXPORT("export");
NodeType const test::EXPORT_VALUE("exported value");
NodeType const test::ATTR_SET("attribute set");
NodeType const test::DEC_THIS("declare this");

NodeType const test::FUNC_DECL("func decl");
NodeType const test::PARAMETER("parameter");

NodeType const test::SCOPE_BEGIN("scope begin");
NodeType const test::SCOPE_END("scope end");

NodeType const test::BRANCH("branch");

void SemanticTest::SetUp()
{
    clearErr();
    clearWarn();
}

void SemanticTest::TearDown()
{
    DataTree::verify();
}
