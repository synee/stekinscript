#include <report/errors.h>

#include "node-base.h"
#include "stmt-automations.h"
#include "function.h"
#include "clauses.h"

using namespace grammar;

void AutomationBase::setPrevious(util::sref<AutomationBase const> previous)
{
    _previous = previous;
}

void AutomationStack::push(util::sptr<AutomationBase> automation)
{
    if (!empty()) {
        automation->setPrevious(top());
    }
    _stack.push_back(std::move(automation));
    top()->activated(*this);
    top()->resumed(*this);
}

void AutomationStack::replace(util::sptr<AutomationBase> automation)
{
    _stack.pop_back();
    push(std::move(automation));
}

util::sref<AutomationBase> AutomationStack::top() const
{
    return *_stack.back();
}

void AutomationStack::reduced(util::sptr<Expression const> expr)
{
    _stack.pop_back();
    top()->accepted(*this, std::move(expr));
    top()->resumed(*this);
}

void AutomationStack::reduced(std::vector<util::sptr<Expression const>> list)
{
    _stack.pop_back();
    top()->accepted(*this, std::move(list));
    top()->resumed(*this);
}

void AutomationStack::pop()
{
    _stack.pop_back();
    if (!empty()) {
        top()->resumed(*this);
    }
}

bool AutomationStack::empty() const
{
    return _stack.empty();
}

void AutomationBase::pushOp(AutomationStack&, Token const& token)
{
    error::unexpectedToken(token.pos, token.image);
}

void AutomationBase::pushPipeSep(AutomationStack&, Token const& token)
{
    error::unexpectedToken(token.pos, token.image);
}

void AutomationBase::pushFactor(
                AutomationStack&, util::sptr<Expression const> factor, std::string const& image)
{
    error::unexpectedToken(factor->pos, image);
}

void AutomationBase::pushThis(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "@");
}

void AutomationBase::pushOpenParen(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "(");
}

void AutomationBase::pushOpenBracket(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "[");
}

void AutomationBase::pushOpenBrace(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "{");
}

void AutomationBase::matchClosing(AutomationStack&, Token const& closer)
{
    error::unexpectedToken(closer.pos, closer.image);
}

void AutomationBase::pushColon(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, ":");
}

void AutomationBase::pushPropertySeparator(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "::");
}

void AutomationBase::pushComma(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, ",");
}

void AutomationBase::pushIf(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "if");
}

void AutomationBase::pushElse(AutomationStack&, misc::position const& pos)
{
    error::unexpectedToken(pos, "else");
}

void ClauseStackWrapper::pushBlockReceiver(util::sref<AutomationBase> blockRecr)
{
    _clauses.push_back(util::mkptr(new BlockReceiverClause(_last_indent, _stack, _pos, blockRecr)));
}

void ClauseStackWrapper::pushIfClause(util::sptr<Expression const> predicate)
{
    util::sref<ClauseBase> parent(*_clauses.back());
    _clauses.push_back(util::mkptr(new IfClause(_last_indent, std::move(predicate), parent)));
}

void ClauseStackWrapper::pushElseClause(misc::position const& else_pos)
{
    util::sref<ClauseBase> parent(*_clauses.back());
    _clauses.push_back(util::mkptr(new ElseClause(_last_indent, else_pos, parent)));
}
