#include <semantic/function.h>
#include <report/errors.h>

#include "clauses.h"
#include "stmt-automations.h"
#include "expr-automations.h"
#include "stmt-nodes.h"
#include "function.h"

using namespace grammar;

ExprStmtAutomation::ExprStmtAutomation(util::sref<ClauseBase> clause)
    : _clause(clause)
    , _before_colon(true)
{
    static AutomationCreator const createPipeline(
            [](TypedToken const&)
            {
                return util::mkptr(new PipelineAutomation);
            });
    _setShifts({
        { THIS, { createPipeline, true } },
        { OPEN_PAREN, { createPipeline, true } },
        { OPEN_BRACKET, { createPipeline, true } },
        { OPEN_BRACE, { createPipeline, true } },
        { OPERATOR, { createPipeline, true } },
    });

    _actions[IF] = [](AutomationStack& stack, TypedToken const&)
                   {
                       stack.replace(util::mkptr(new IfAutomation));
                   };
    _actions[ELSE] = [](AutomationStack& stack, TypedToken const& token)
                     {
                         stack.replace(util::mkptr(new ElseAutomation(token.pos)));
                     };
    _actions[COLON] = [&](AutomationStack& stack, TypedToken const& token)
                      {
                          _pushColon(stack, token.pos);
                      };
}

void ExprStmtAutomation::pushFactor(AutomationStack& stack
                                  , util::sptr<Expression const> factor
                                  , std::string const& image)
{
    stack.push(util::mkptr(new PipelineAutomation));
    stack.top()->pushFactor(stack, std::move(factor), image);
}

void ExprStmtAutomation::accepted(AutomationStack&, util::sptr<Expression const> expr)
{
    _exprs.push_back(std::move(expr));
}

bool ExprStmtAutomation::finishOnBreak(bool sub_empty) const
{
    return _before_colon || !sub_empty || _exprs.size() == 2;
}

void ExprStmtAutomation::finish(ClauseStackWrapper&, AutomationStack& stack, misc::position const&)
{
    if (!_exprs.empty()) {
        _clause->acceptStmt(_reduceAsStmt());
    }
    stack.pop();
}

void ExprStmtAutomation::_pushColon(AutomationStack& stack, misc::position const& pos)
{
    _before_colon = false;
    if (_exprs.size() == 1) {
        stack.push(util::mkptr(new PipelineAutomation));
        return;
    }
    error::unexpectedToken(pos, ":");
}

util::sptr<Statement> ExprStmtAutomation::_reduceAsStmt()
{
    misc::position pos(_exprs[0]->pos);
    if (_exprs.size() == 1) {
        return util::mkptr(new Arithmetics(pos, std::move(_exprs[0])));
    }
    if (_exprs[0]->isName()) {
        return util::mkptr(new NameDef(pos, _exprs[0]->reduceAsName(), std::move(_exprs[1])));
    }
    return util::mkptr(new AttrSet(pos, std::move(_exprs[0]), std::move(_exprs[1])));
}

void IfAutomation::activated(AutomationStack& stack)
{
    stack.push(util::mkptr(new PipelineAutomation));
}

void IfAutomation::accepted(AutomationStack&, util::sptr<Expression const> expr)
{
    _pred_cache = std::move(expr);
}

bool IfAutomation::finishOnBreak(bool sub_empty) const
{
    return !sub_empty;
}

void IfAutomation::finish(
                ClauseStackWrapper& wrapper, AutomationStack& stack, misc::position const&)
{
    if (_pred_cache->empty()) {
        error::invalidEmptyExpr(_pred_cache->pos);
    }
    wrapper.pushIfClause(std::move(_pred_cache));
    stack.pop();
}

void ElseAutomation::finish(
                ClauseStackWrapper& wrapper, AutomationStack& stack, misc::position const&)
{
    wrapper.pushElseClause(else_pos);
    stack.pop();
}

void ExprReceiver::activated(AutomationStack& stack)
{
    stack.push(util::mkptr(new PipelineAutomation));
}

void ExprReceiver::accepted(AutomationStack&, util::sptr<Expression const> expr)
{
    _expr = std::move(expr);
}

void ExprReceiver::finish(ClauseStackWrapper&, AutomationStack& stack, misc::position const&)
{
    if (_expr->empty()) {
        error::invalidEmptyExpr(_expr->pos);
    }
    _clause->acceptExpr(std::move(_expr));
    stack.pop();
}

void ReturnAutomation::finish(ClauseStackWrapper&, AutomationStack& stack, misc::position const&)
{
    if (_expr->empty()) {
        _clause->acceptStmt(util::mkptr(new ReturnNothing(_expr->pos)));
    } else {
        _clause->acceptStmt(util::mkptr(new Return(_expr->pos, std::move(_expr))));
    }
    stack.pop();
}

void ExportStmtAutomation::finish(
                        ClauseStackWrapper&, AutomationStack& stack, misc::position const&)
{
    if (_expr->empty()) {
        error::invalidEmptyExpr(_expr->pos);
    }
    _clause->acceptStmt(util::mkptr(new Export(_expr->pos, export_point, std::move(_expr))));
    stack.pop();
}
