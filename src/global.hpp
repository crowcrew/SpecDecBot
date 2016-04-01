#include <bits/stdc++.h>
#include <sqlite3.h>
using namespace std;

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

string words[100]; // words of a SINGLE sentence
string tokens[100][3]; // tokens of each word in a SINGLE sentence
string sentence; // sentence being said to the bot as in "input sentence"
string word; // word being tokenized
string field; // query result as in "token of a word"
string statement; // SQL statement to be queried
int pos=0; // position of the last word in a SINGLE sentence

#endif // VAR_HPP
