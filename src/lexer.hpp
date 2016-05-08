#ifndef LEXER_HPP
#define LEXER_HPP

#include "global.hpp"

class lexer
{
public:
    lexer(); // constructor
    void input(); // input to the sentence variable at GLOBAL_HPP class, split it, and store each word in the words[100] array
    void debug(); // debug and output the words[100] & tokens[100][3] arrays
private:
    string word; // word being tokenized
    string statement; // SQL statement to be queried
    vector< queue<string> > tokens; // tokens of each word in a SINGLE sentence
    int eos; // end of sentence
    void tokenize(); // query the words, and store their tokens to the tokens[100][3] array at GLOBAL_HPP class
    void iterate(); // iterates over tokens to generate permutations of tokenized sentences
    void flip(int j);
    void record();
};

lexer::lexer()
{
    sql sqlObject;
}

void lexer::tokenize()
{
    queue <string> word_tokens;

    statement="SELECT POS1 FROM DIC WHERE WORD = '";
    statement+=word;
    statement+="'";
    sqlObject.query(statement);
    word_tokens.push(field);
    field.clear();

    statement="SELECT POS2 FROM DIC WHERE WORD = '";
    statement+=word;
    statement+="'";
    sqlObject.query(statement);
    word_tokens.push(field);
    field.clear();

    statement="SELECT POS3 FROM DIC WHERE WORD = '";
    statement+=word;
    statement+="'";
    sqlObject.query(statement);
    word_tokens.push(field);
    field.clear();

    tokens.push_back(word_tokens);
}

void lexer::record()
{
    vector <string> iteration;
    for(int i=0; i<tokens.size(); i++)
        if(tokens[i].front()=="null")
            return;
        else
            iteration.push_back(tokens[i].front());
    permutations.push(iteration);
}

void lexer::flip(int j)
{
    string temp = tokens[j].front();
    tokens[j].pop();
    tokens[j].push(temp);
}

void lexer::iterate()
{
    for(int i=1; i<=pow(3,tokens.size()); i++)
    {
        for(int j=0; j<tokens.size(); j++)
            if(j==0)
                flip(j);
            else if (i%(int)pow(3,j))
                flip(j);
        record();
    }
}

void lexer::debug()
{
    cout<<"debugging each word at eos : "<<eos<<"\n";
    vector< queue<string> > tokens_t = tokens;
    for(int i=0; i<tokens_t.size(); i++)
    {
        while(!tokens_t[i].empty())
        {
            cout<<tokens_t[i].front()<<" ";
            tokens_t[i].pop();
        }
        cout<<endl;
    }
    tokens_t.clear();

    cout<<"debugging permutations at size : "<<permutations.size()<<"\n";
    queue< vector<string> > permutations_t = permutations;
    while(!permutations_t.empty())
    {
        for(int i=0; i<permutations_t.front().size(); i++)
            cout<<permutations_t.front()[i]<<" ";
        cout<<endl;
        permutations_t.pop();
    }

}

void lexer::input()
{
    eos=0;
    getline(cin,sentence);
    for(int i=0; i<=sentence.size(); i++)
    {
        if(sentence[i]!=' '&&sentence[i]!='\0')
            word+=sentence[i];
        if(sentence[i]==' '||i==sentence.size()-1)
        {
            words[eos]=word;
            eos++;
            tokenize();
            word.clear();
        }
    }
    sentence.clear();
    iterate();
}

#endif // LEXER_HPP
