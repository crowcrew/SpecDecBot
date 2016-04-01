/*
SpecDecBot (modified zlib) License

Copyright (c) 2015 SpecDecBot Project (Aly Shmahell, Alya Salman, Elias Soud, Mohammad Shbani, Ruaa Sleiman, Sarah Ammar).

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation is required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "sql.hpp"
#include "global.hpp"

#ifndef LEXER_HPP
#define LEXER_HPP

class lexer
{
public:
    lexer(); // constructor
    sql db; // initialize an sql object
    void input(); // input to the sentence variable at GLOBAL_HPP class, split it, and store each word in the words[100] array
    void tokenize(); // query the words, and store their tokens to the tokens[100][3] array at GLOBAL_HPP class
    void debug(); // debug and output the words[100] & tokens[100][3] arrays
};

lexer::lexer()
{
    sql db;
}

void lexer::tokenize()
{
    statement="SELECT POS1 FROM DIC WHERE WORD = '";
    statement+=word;
    statement+="'";
    db.query();
    tokens[pos][0]=field;
    field.clear();

    statement="SELECT POS2 FROM DIC WHERE WORD = '";
    statement+=word;
    statement+="'";
    db.query();
    tokens[pos][1]=field;
    field.clear();

    statement="SELECT POS3 FROM DIC WHERE WORD = '";
    statement+=word;
    statement+="'";
    db.query();
    tokens[pos][2]=field;
    field.clear();
}

void lexer::debug()
{
    cout<<"debugging at pos : "<<pos<<"\n";
    for(int i=0; i<pos; i++)
        cout<<words[i]<<" ";
    cout<<endl;
    for(int i=1; i<=pos; i++)
    {
        for(int j=0; j<3; j++)
        {
            cout<<tokens[i][j]<<" ";
        }
        cout<<endl;
    }
}

void lexer::input()
{
    pos=0;
    getline(cin,sentence);
    for(int i=0; i<=sentence.size(); i++)
    {
        if(sentence[i]!=' '&&sentence[i]!='\0')
            word+=sentence[i];
        if(sentence[i]==' '||i==sentence.size()-1)
        {
            words[pos]=word;
            pos++;
            tokenize();
            word.clear();
        }
    }
    sentence.clear();
}

#endif // LEXER_HPP
