#include "specdecbot.h"

int main()
{
    lexer lex;
    load();
    LogThis("**new input**","\n");
    lex.input();
    cycle_through();
}
