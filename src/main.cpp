#include "global.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "tree.hpp"
using namespace std;
int main()
{
    lexer lex;
    lex.input();
    lex.debug();
    cycle_through();
    cout<<"************\n"<<tree_dfs(level[0])<<endl;
}
