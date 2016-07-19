#include <bits/stdc++.h>
using namespace std;

#ifndef GLOBAL_HPP
#define GLOBAL_HPP


/**
 * stores each permutation of a tokenized sentence
 */
queue< vector<string> > permutations;

/**
 * the final parse tree
 */
string Parse_Tree;

/**
 * a function for logging and display
 */
void LogThis(string message,string LogLine)
{
    string buffer;
    buffer+=message;
    buffer+=LogLine;
    cout<<buffer;
    FILE *LogFile = new FILE;
    LogFile = fopen("log.txt","at+");
    fwrite(buffer.c_str(),buffer.size(),1,LogFile);
    fclose(LogFile);
}

#endif /** end of GLOBAL_HPP **/

#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

typedef struct dictionary_node
{
    vector<string> data_;
    dictionary_node *next;
} dictionary_node;

int counter=0;
dictionary_node *hashtable[27];

int hash_func(string data)
{
    int hashvalue=0;
    int counter_t=0;
    while(data[counter_t]!='\0')
        hashvalue +=(int)tolower(data[counter_t++]);
    return hashvalue % 27;
}

/**
 * Returns vector of tokens of a word in dictionary else returns empty vector.
 */
vector<string> get_tokens(string word)
{
    dictionary_node* current_list = new dictionary_node;

    unsigned int hashvalue = hash_func(word);

    for(current_list=hashtable[hashvalue]; current_list!=NULL; current_list=current_list->next)
        if(current_list->data_.front()==word)
            return current_list->data_;
    dictionary_node* empty_list = new dictionary_node;
    return empty_list->data_;
}

/**
 * Loads dictionary into memory. Returns true if successful else returns false.
 */
bool load()
{
    FILE* file = fopen("dictionary.txt","r");
    if(file==NULL)
    {
        printf("Cannot open dictionary.txt\n");
        return false;
    }
    char buffer;
    string word;
    vector<string> word_tokens;

    while(!feof(file))
    {
        fread(&buffer,sizeof(char),1,file);
        if(buffer!='\t'&&buffer!='\n')
            word+=buffer;
        if(buffer=='\t'||buffer=='\n')
        {
            word_tokens.push_back(word);
            word.clear();
        }
        if(buffer=='\n'&&word_tokens.size()>1)
        {
            dictionary_node *new_list = new dictionary_node;

            new_list->data_ = word_tokens;

            int hashvalue = hash_func(word_tokens.front());

            if (hashtable[hashvalue])
            {
                new_list->next = hashtable[hashvalue];
                hashtable[hashvalue] = new_list;
            }
            else
            {
                hashtable[hashvalue] = new_list;
                new_list->next = NULL;
            }
            counter++;
            word_tokens.clear();
        }
    }
    fclose(file);
}


/**
 * Unloads dictionary from memory. Returns true if successful else returns false.
 */
bool unload()
{
    for(int i=0; i<27; i++)
    {
        dictionary_node* main_list = new dictionary_node;
        dictionary_node* sub_list = new dictionary_node;

        for(main_list = hashtable[i]; main_list; sub_list = main_list,main_list = main_list->next)
            free(sub_list);

        free(main_list);
        hashtable[i]=NULL;
    }
    return true;
}

#endif /** end of DICTIONARY_HPP **/

#ifndef LEXER_HPP
#define LEXER_HPP

class lexer
{
public:

    /**
    * sentence being said to the bot as an "input sentence"
    */
    string sentence;

    /**
     * words of a single sentence
     */
    string words[100];

    /**
     * input to the sentence variable at GLOBAL_HPP class,
     * split it, and store each word in the words[100] array
     */
    void input()
    {
        eos=0;
        getline(cin,sentence);
        LogThis("Input:"," ");
        LogThis(sentence,"\n");
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

private:
    /**
     * word being tokenized
     */
    string word;
    /**
     * tokens of each word in a single sentence
     */
    vector< queue<string> > tokens;
    /**
     * end of sentence
     */
    int eos; //
    /**
     * query the words, and store their tokens to the tokens[100][3] array at GLOBAL_HPP class
     */
    void tokenize()
    {
        queue <string> word_tokens;
        word_tokens.push(get_tokens(word)[1]);
        word_tokens.push(get_tokens(word)[2]);
        word_tokens.push(get_tokens(word)[3]);
        tokens.push_back(word_tokens);
    }
    /**
     * iterates over tokens to generate permutations of tokenized sentences
     */
    void iterate()
    {
        for(int i=1; i<=pow(3,tokens.size()); i++)
        {
            for(int j=0; j<tokens.size(); j++)
                if(j==0||i%(int)pow(3,j))
                {
                    string temp = tokens[j].front();
                    tokens[j].pop();
                    tokens[j].push(temp);
                }
            record();
        }
    }

    void record()
    {
        vector <string> permutation;
        for(int i=0; i<tokens.size(); i++)
            if(tokens[i].front()=="null")
                return;
            else
                permutation.push_back(tokens[i].front());
        permutations.push(permutation);
    }
};

#endif /** end of LEXER_HPP **/

#ifndef TREE_HPP
#define TREE_HPP

typedef struct tree_node
{
    string value;
    vector<tree_node*> children;
} tree_node;

pair<string,int> tree_dfs(tree_node* head)
{
    stack <tree_node*> bucket;
    pair<string,int> line;
    line.second=0;
    tree_node* RPAR = new tree_node;
    RPAR->value = ")";
    RPAR->children.clear();
    tree_node* SPACE = new tree_node;
    SPACE->value = " ";
    SPACE->children.clear();
    bucket.push(head);
    while(!bucket.empty())
    {
        tree_node* current_node = new tree_node;
        current_node = bucket.top();
        bucket.pop();
        line.first+=current_node->value;
        if(current_node->children.size())
        {
            line.first+="(";
            bucket.push(RPAR);
        }
        else
        {
            if(current_node->value!=")")
                line.second++;
            if(bucket.size()&&bucket.top()->value!=")")
                line.first+=" ";
        }
        for(int i=current_node->children.size()-1; i>=0; i--)
            bucket.push(current_node->children[i]);
    }
    return line;
}

#endif /** end of TREE_HPP **/

#ifndef CFG_HPP
#define CFG_HPP

class cfg
{
public:
    vector< vector<string> > rules;
    cfg()
    {
        FILE* file = fopen("cfg.txt","r");
        if(file==NULL)
        {
            printf("Cannot open cfg.txt\n");
            return;
        }

        char buffer;
        string word;
        vector <string> rule;

        while(!feof(file))
        {
            fread(&buffer,sizeof(char),1,file);
            if(buffer!=' '&&buffer!='\n')
                word+=buffer;
            if(buffer==' '||buffer=='\n')
            {
                rule.push_back(word);
                word.clear();
            }
            if(buffer=='\n')
            {
                rules.push_back(rule);
                rule.clear();
            }
        }
        fclose(file);
    }
};

#endif /** end of CFG_HPP **/

cfg CFGinMemory;
vector< vector<string> > rules = CFGinMemory.rules;

#ifndef CYK_HPP
#define CYK_HPP

int MatrixSize;
tree_node* curr_head;

bool ok_single(tree_node* node_t)
{
    for(int counter=0; counter<rules.size()-1; counter++)
    {
        if(rules[counter].size()==2&&rules[counter][1]==node_t->value)
        {
            curr_head = new tree_node;
            curr_head->value = rules[counter][0];
            return true;
        }
    }
    return false;
}

bool ok_full(vector<tree_node*> line_t)
{
    for(int counter=0; counter<rules.size()-1; counter++)
    {
        if(rules[counter].size()==3&&rules[counter][1]==line_t[0]->value&&rules[counter][2]==line_t[1]->value)
        {
            curr_head = new tree_node;
            curr_head->value = rules[counter][0];
            return true;
        }
    }
    return false;
}

bool cyk(vector<string> permutation)
{
    MatrixSize = permutation.size();
    tree_node* empty_node = new tree_node;
    empty_node->value = "";
    tree_node* matrix[MatrixSize+1][MatrixSize];
    for(int i=0; i<MatrixSize+1; i++)
        for(int j=0; j<MatrixSize; j++)
        {
            matrix[i][j]= new tree_node;
        }
    for(int i=0; i<MatrixSize; i++)
    {
        matrix[MatrixSize-i][i]->value = permutation[i];
    }
    for(int counter = 0; counter < MatrixSize+1; counter++)
    {

        for(int row = MatrixSize-counter-1, col = 0; row >= 0 && col < MatrixSize-counter; row--, col++)
        {
            if(ok_single(matrix[row+1][col])&&(((row!=0||col!=0)&&curr_head->value!="s")||((row==0&&col==0)&&curr_head->value=="s")))
            {
                curr_head->children.push_back(matrix[row+1][col]);
                matrix[row][col] = curr_head;
            }
            else
            {
                vector<tree_node*> line;
                line.push_back(matrix[row+1][col]);
                line.push_back(matrix[row][col+1]);
                if(ok_full(line)&&(((row!=0||col!=0)&&curr_head->value!="s")||((row==0&&col==0)&&curr_head->value=="s")))
                {
                    curr_head->children.push_back(matrix[row+1][col]);
                    curr_head->children.push_back(matrix[row][col+1]);
                    matrix[row][col] = curr_head;

                }
                else
                {
                    matrix[row][col]=matrix[row+1][col];
                    if(matrix[row][col]->children.size()==1)
                    {
                        vector<tree_node*> line;
                        line.push_back(matrix[row][col]->children.front());
                        line.push_back(matrix[row][col+1]);
                        if(ok_full(line))
                        {
                            matrix[row][col]->children.push_back(matrix[row][col+1]);
                        }
                    }
                }

            }
        }
    }
    /**
     * Logging
     */
    for(int i=0; i<MatrixSize+1; i++)
    {
        for(int j=0; j<MatrixSize; j++)
        {
            LogThis(matrix[i][j]->value," ");
        }
        LogThis(" ","\n");
    }
    LogThis("This Cycle produced this Parse Line:"," ");
    LogThis(tree_dfs(matrix[0][0]).first,"\n");

    /**
     * Finding Tree Head
     */
    if(matrix[0][0]->value=="s"&&tree_dfs(matrix[0][0]).second==MatrixSize)
        return true;
    else
        return false;
}

void cycle_through()
{
    vector<string> permutation;
    while(!permutations.empty())
    {
        permutation = permutations.front();
        permutations.pop();
        if(cyk(permutation))
            return;
        permutation.clear();
    }

    LogThis("tree head not found!","\n");
}

#endif /** end of CYK_HPP **/

