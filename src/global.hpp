#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "sql.hpp"
#include "cfg.hpp"
#include <bits/stdc++.h>
#include <sqlite3.h>
using namespace std;



string sentence; // sentence being said to the bot as in "input sentence"
string words[100]; // words of a SINGLE sentence
queue< vector<string> > permutations; // stores each iteration of a tokenized sentence
sql sqlObject; // initialize an sql object
string Parse_Tree;
cfg cfg_n;
vector< vector<string> > rules = cfg_n.rules;

#endif // GLOBAL_HPP
