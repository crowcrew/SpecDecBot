#include "sql.hpp"
#include <bits/stdc++.h>
#include <sqlite3.h>
using namespace std;

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

string sentence; // sentence being said to the bot as in "input sentence"
string words[100]; // words of a SINGLE sentence
queue< vector<string> > permutations; // stores each iteration of a tokenized sentence
sql sqlObject; // initialize an sql object

#endif // GLOBAL_HPP
