#include <bits/stdc++.h>
using namespace std;

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

/**
 * sentence being said to the bot as an "input sentence"
 */
string sentence;
/**
 * words of a single sentence
 */
string words[100];
/**
 * stores each iteration of a tokenized sentence
 */
queue< vector<string> > permutations;
/**
 * the final parse tree
 */
string Parse_Tree;

#endif // end of GLOBAL_HPP

#ifndef TREE_HPP
#define TREE_HPP

typedef struct node
{
    string value;
    vector<node*> children;
} node;

pair<string,int> tree_dfs(node* head)
{
    stack <node*> bucket;
    pair<string,int> line;
    line.second=0;
    node* RPAR = new node;
    RPAR->value = ")";
    RPAR->children.clear();
    node* SPACE = new node;
    SPACE->value = " ";
    SPACE->children.clear();
    bucket.push(head);
    while(!bucket.empty())
    {
        node* current_node = new node;
        current_node = bucket.top();
        bucket.pop();
        line.first+=current_node->value;
        if(current_node->children.size())
        {
            line.first+=" (";
            bucket.push(RPAR);
        }
        else
        {
            if(current_node->value!=")")
                line.second++;
            line.first+=" ";
        }
        for(int i=current_node->children.size()-1; i>=0; i--)
            bucket.push(current_node->children[i]);
    }
    return line;
}

#endif // end of TREE_HPP

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
    void debug()
    {
        for(int i=0; i<rules.size()-1; i++)
        {
            cout<<"p: ";
            for(int j=0; j<rules[i].size(); j++)
                cout<<rules[i][j]<<" ";
            if(i<rules.size()-2)
                cout<<endl;
        }
    }
};

#endif // end of CFG_HPP

cfg cfg_n;
vector< vector<string> > rules = cfg_n.rules;

#ifndef CYK_HPP
#define CYK_HPP

int msize;
node* curr_head;
node* matrix_head;

void logging(string message,string log_line)
{
    FILE *log_t = new FILE;
    string buffer;
    buffer+=message;
    buffer+=log_line;
    buffer+="\n";
    log_t = fopen("log.txt","at+");
    fwrite(buffer.c_str(),buffer.size(),1,log_t);
    fclose(log_t);
}

bool ok_single(node* node_t)
{
    logging("ok_single : ",node_t->value);
    for(int counter=0; counter<rules.size()-1; counter++)
    {
        if(rules[counter].size()==2&&rules[counter][1]==node_t->value)
        {
            logging("ok_single true : ",rules[counter][0]);
            curr_head = new node;
            curr_head->value = rules[counter][0];
            return true;
        }
    }
    return false;
}
bool ok_full(vector<node*> line_t)
{
    logging("ok_full first : ",line_t[0]->value);
    logging("ok_full secon : ",line_t[1]->value);
    for(int counter=0; counter<rules.size()-1; counter++)
    {
        if(rules[counter].size()==3&&rules[counter][1]==line_t[0]->value&&rules[counter][2]==line_t[1]->value)
        {
            logging("ok_full true : ",rules[counter][0]);
            curr_head = new node;
            curr_head->value = rules[counter][0];
            return true;
        }
    }
    return false;
}
bool cyk(vector<string> permutation)
{
    msize = permutation.size();
    node* empty_node = new node;
    empty_node->value = "";
    node* matrix[msize+1][msize];
    for(int i=0; i<msize+1; i++)
        for(int j=0; j<msize; j++)
        {
            matrix[i][j]= new node;
        }
    logging("cycle","!!!!!!!!!!!!!!!!");
    logging("before","!");
    for(int i=0; i<msize; i++)
    {
        matrix[msize-i][i]->value = permutation[i];
    }
    logging("after","!");
    cout<<msize<<endl;
    for(int counter = 0; counter < msize+1; counter++)
    {

        for(int row = msize-counter-1, col = 0; row >= 0 && col < msize-counter; row--, col++)
        {
            logging("next box","!!!->:");
            if(ok_single(matrix[row+1][col])&&(((row!=0||col!=0)&&curr_head->value!="s")||((row==0&&col==0)&&curr_head->value=="s")))
            {
                curr_head->children.push_back(matrix[row+1][col]);
                matrix[row][col] = curr_head;
            }
            else
            {
                vector<node*> line;
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
                        vector<node*> line;
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
    //debug
    for(int i=0; i<msize+1; i++)
    {
        for(int j=0; j<msize; j++)
        {
            cout<<matrix[i][j]->value<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    //
    logging(tree_dfs(matrix[0][0]).first," ");
    if(matrix[0][0]->value=="s"&&tree_dfs(matrix[0][0]).second==msize)
    {
        matrix_head = matrix[0][0];
        return true;
    }
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

    logging("tree head not found!","");
}

#endif // end of CYK_HPP

#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

typedef struct list_node
{
    vector<string> data_;
    list_node *next;
} list_node;

int counter=0;
list_node *hashtable[27];

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
    list_node* current_list = new list_node;

    unsigned int hashvalue = hash_func(word);

    for(current_list=hashtable[hashvalue]; current_list!=NULL; current_list=current_list->next)
        if(current_list->data_.front()==word)
            return current_list->data_;
    list_node* empty_list = new list_node;
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
            list_node *new_list = new list_node;

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
 * Returns number of words in dictionary if loaded else returns 0.
 */
unsigned int size_(void)
{
    return counter;
}

/**
 * Unloads dictionary from memory. Returns true if successful else returns false.
 */
bool unload()
{
    for(int i=0; i<27; i++)
    {
        list_node* main_list = new list_node;
        list_node* sub_list = new list_node;

        for(main_list = hashtable[i]; main_list; sub_list = main_list,main_list = main_list->next)
            free(sub_list);

        free(main_list);
        hashtable[i]=NULL;
    }
    return true;
}

#endif // end of DICTIONARY_HPP

#ifndef LEXER_HPP
#define LEXER_HPP

class lexer
{
public:
    /**
     * input to the sentence variable at GLOBAL_HPP class,
     * split it, and store each word in the words[100] array
     */
    void input()
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
    /**
     * debug and output the words[100] & tokens[100][3] arrays
     */
    void debug()
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
                if(j==0)
                    flip(j);
                else if (i%(int)pow(3,j))
                    flip(j);
            record();
        }
    }
    void flip(int j)
    {
        string temp = tokens[j].front();
        tokens[j].pop();
        tokens[j].push(temp);
    }
    void record()
    {
        vector <string> iteration;
        for(int i=0; i<tokens.size(); i++)
            if(tokens[i].front()=="null")
                return;
            else
                iteration.push_back(tokens[i].front());
        permutations.push(iteration);
    }
};

#endif // end of LEXER_HPP

int main()
{
    lexer lex;
    load();
    lex.input();
    lex.debug();
    cycle_through();
    cout<<"************\n"<<tree_dfs(matrix_head).first<<endl;
}
