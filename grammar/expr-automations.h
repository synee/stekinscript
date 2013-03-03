#ifndef __STEKIN_GRAMMAR_EXPRESSION_AUTOMATIONS_H__
#define __STEKIN_GRAMMAR_EXPRESSION_AUTOMATIONS_H__

#include "automation-base.h"
#include "expr-nodes.h"

namespace grammar {

    struct PipelineAutomation
        : AutomationBase
    {
        PipelineAutomation()
            : _cache_list(nullptr)
            , _cache_section(nullptr)
        {}

        void activated(AutomationStack& stack);
        void pushPipeSep(AutomationStack& stack, Token const& token);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushColon(AutomationStack& stack, misc::position const& pos);
        void pushPropertySeparator(AutomationStack& stack, misc::position const& pos);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack&, util::sptr<Expression const> expr);
        void accepted(AutomationStack& stack, misc::position const& pos, Block&& block);
        bool finishOnBreak(bool sub_empty) const;
        void finish(ClauseStackWrapper&, AutomationStack&, misc::position const&);
    private:
        void _tryReducePipe();
        void _reduce(AutomationStack& stack);
        bool _afterOperator(bool sub_empty) const;

        util::sptr<Expression const> _cache_list;
        util::sptr<Expression const> _cache_section;
        misc::position _cache_op_pos;
        std::string _cache_pipe_op;
    };

    struct ConditionalAutomation
        : AutomationBase
    {
        ConditionalAutomation()
            : _before_if(true)
            , _cache_consq(nullptr)
            , _before_else(true)
            , _cache_pred(nullptr)
        {}

        void activated(AutomationStack& stack);
        void pushPipeSep(AutomationStack& stack, Token const& token);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushColon(AutomationStack& stack, misc::position const& pos);
        void pushPropertySeparator(AutomationStack& stack, misc::position const& pos);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void pushIf(AutomationStack& stack, misc::position const& pos);
        void pushElse(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack& stack, util::sptr<Expression const> expr);
        bool finishOnBreak(bool sub_empty) const;
        void finish(ClauseStackWrapper& wrapper, AutomationStack& stack, misc::position const& pos);
    private:
        void _forceReduce(AutomationStack& stack);

        bool _before_if;
        util::sptr<Expression const> _cache_consq;
        bool _before_else;
        util::sptr<Expression const> _cache_pred;
    };

    struct ArithAutomation
        : AutomationBase
    {
        void pushOp(AutomationStack& stack, Token const& token);
        void pushPipeSep(AutomationStack& stack, Token const& token);
        void pushFactor(AutomationStack& stack
                      , util::sptr<Expression const> factor
                      , std::string const& image);
        void pushThis(AutomationStack& stack, misc::position const& pos);
        void pushOpenParen(AutomationStack& stack, misc::position const& pos);
        void pushOpenBracket(AutomationStack& stack, misc::position const& pos);
        void pushOpenBrace(AutomationStack& stack, misc::position const& pos);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushColon(AutomationStack& stack, misc::position const& pos);
        void pushPropertySeparator(AutomationStack& stack, misc::position const& pos);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void pushIf(AutomationStack& stack, misc::position const& pos);
        void pushElse(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack&, util::sptr<Expression const> expr);
        void accepted(AutomationStack&, std::vector<util::sptr<Expression const>> list);
        bool finishOnBreak(bool sub_empty) const;
        void finish(ClauseStackWrapper&, AutomationStack& stack, misc::position const&);

        ArithAutomation();

        struct Operator;
        typedef std::vector<util::sptr<Operator const>> OpStack;
        typedef std::vector<util::sptr<Expression const>> FactorStack;

        struct Operator {
            virtual ~Operator() {}

            Operator(int p)
                : pri(p)
            {}

            virtual void operate(FactorStack& factors) const = 0;

            int const pri;
        };
    private:
        bool _empty() const;
        void _pushFactor(util::sptr<Expression const> factor, Token const& token);
        void _reduceBinaryOrPostfix(int pri);
        bool _reduceIfPossible(
                AutomationStack& stack, misc::position const& token, std::string const& image);

        bool _need_factor;
        bool _accept_list_for_args;
        std::string _reduce_close_paren;
        FactorStack _factor_stack;
        OpStack _op_stack;
    };

    struct ExprListAutomation
        : AutomationBase
    {
        void activated(AutomationStack& stack);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack&, util::sptr<Expression const> expr);
        bool finishOnBreak(bool) const { return false; }
        void finish(ClauseStackWrapper&, AutomationStack&, misc::position const&) {}
    protected:
        std::vector<util::sptr<Expression const>> _list;
    };

    struct ListLiteralAutomation
        : ExprListAutomation
    {
        void matchClosing(AutomationStack& stack, Token const& closer);
    };

    struct NestedOrParamsAutomation
        : ExprListAutomation
    {
        void pushOp(AutomationStack& stack, Token const& token);
        void pushPipeSep(AutomationStack& stack, Token const& token);
        void pushOpenParen(AutomationStack& stack, misc::position const& pos);
        void pushOpenBracket(AutomationStack& stack, misc::position const& pos);
        void matchClosing(AutomationStack&, Token const& closer);
        void pushColon(AutomationStack& stack, misc::position const& pos);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack& stack, util::sptr<Expression const> expr);
        void accepted(AutomationStack& stack, misc::position const& pos, Block&& block);
        bool finishOnBreak(bool sub_empty) const;
        void finish(ClauseStackWrapper& wrapper, AutomationStack& stack, misc::position const& pos);

        NestedOrParamsAutomation()
            : _wait_for_closing(true)
            , _wait_for_colon(false)
            , _lambda_ret_val(nullptr)
        {}
    private:
        void _reduceAsNested(AutomationStack& stack, misc::position const& rp);
        void _reduceAsLambda(AutomationStack& stack);
        void _reduceAsLambda(AutomationStack& stack, misc::position const& pos, Block body);
        bool _afterColon() const;

        bool _wait_for_closing;
        bool _wait_for_colon;
        util::sptr<Expression const> _lambda_ret_val;
    };

    struct BracketedExprAutomation
        : ExprListAutomation
    {
        void matchClosing(AutomationStack& stack, Token const& closer);
    };

    struct DictAutomation
        : AutomationBase
    {
        void activated(AutomationStack& stack);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void pushColon(AutomationStack& stack, misc::position const& pos);
        void pushPropertySeparator(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack&, util::sptr<Expression const> expr);
        bool finishOnBreak(bool) const { return false; }
        void finish(ClauseStackWrapper&, AutomationStack&, misc::position const&) {}

        DictAutomation()
            : _wait_for_key(true)
            , _wait_for_colon(false)
            , _wait_for_comma(false)
            , _key_cache(nullptr)
        {}
    private:
        bool _pushSeparator(AutomationStack& stack
                          , misc::position const& pos
                          , std::string const& sep);

        bool _wait_for_key;
        bool _wait_for_colon;
        bool _wait_for_comma;
        util::sptr<Expression const> _key_cache;
        util::ptrkvarr<Expression const> _items;
    };

    struct AsyncPlaceholderAutomation
        : AutomationBase
    {
        explicit AsyncPlaceholderAutomation(misc::position const& ps)
            : pos(ps)
        {}

        void pushFactor(AutomationStack& stack
                      , util::sptr<Expression const> factor
                      , std::string const& image);
        void pushOpenParen(AutomationStack& stack, misc::position const& pos);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack& stack, util::sptr<Expression const> expr);
        void accepted(AutomationStack& stack, std::vector<util::sptr<Expression const>> list);
        bool finishOnBreak(bool) const { return false; }
        void finish(ClauseStackWrapper&, AutomationStack&, misc::position const&) {}

        misc::position const pos;
    };

    struct ThisPropertyAutomation
        : AutomationBase
    {
        explicit ThisPropertyAutomation(misc::position const& ps)
            : pos(ps)
        {}

        void pushOp(AutomationStack& stack, Token const& token);
        void pushPipeSep(AutomationStack& stack, Token const& token);
        void pushFactor(AutomationStack& stack
                      , util::sptr<Expression const> factor
                      , std::string const& image);
        void pushOpenParen(AutomationStack& stack, misc::position const& pos);
        void pushOpenBracket(AutomationStack& stack, misc::position const& pos);
        void matchClosing(AutomationStack& stack, Token const& closer);
        void pushComma(AutomationStack& stack, misc::position const& pos);
        void pushPropertySeparator(AutomationStack& stack, misc::position const& pos);
        void pushColon(AutomationStack& stack, misc::position const& pos);
        void accepted(AutomationStack&, util::sptr<Expression const>) {}
        bool finishOnBreak(bool sub_empty) const;
        void finish(ClauseStackWrapper& clauses, AutomationStack& stack, misc::position const& pos);

        misc::position const pos;
    };

}

#endif /* __STEKIN_GRAMMAR_EXPRESSION_AUTOMATIONS_H__ */
