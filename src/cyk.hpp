#ifndef CYK_HPP
#define CYK_HPP

#include <bits/stdc++.h>
#include "tree.hpp"
#include "global.hpp"
using namespace std;

int msize;
node* curr_head;

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
                node_connect(curr_head,matrix[row+1][col]);
                matrix[row][col] = curr_head;
            }
            else
            {
                vector<node*> line;
                line.push_back(matrix[row+1][col]);
                line.push_back(matrix[row][col+1]);
                if(ok_full(line)&&(((row!=0||col!=0)&&curr_head->value!="s")||((row==0&&col==0)&&curr_head->value=="s")))
                {
                    node_connect(curr_head,matrix[row+1][col]);
                    node_connect(curr_head,matrix[row][col+1]);
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
                            node_connect(matrix[row][col],matrix[row][col+1]);
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

    logging("tree head not found!","");
}

#endif // CYK_HPP
