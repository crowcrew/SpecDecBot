#include "sql.hpp"

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

string sentence; // sentence being said to the bot as in "input sentence"
string words[100]; // words of a SINGLE sentence
string tokens[100][3]; // tokens of each word in a SINGLE sentence
sql sqlObject; // initialize an sql object

#endif // GLOBAL_HPP
