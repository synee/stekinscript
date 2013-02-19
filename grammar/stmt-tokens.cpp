#include "stmt-tokens.h"

using namespace grammar;

void IfToken::act(AutomationStack& stack)
{
    stack.top()->pushIf(stack, pos);
}

void ElseToken::act(AutomationStack& stack)
{
    stack.top()->pushElse(stack, pos);
}
