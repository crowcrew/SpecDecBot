#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <stack>

using namespace std;

typedef struct node
{
    string value;
    vector<node*> children;
} node;

void node_connect(node* father,node* child)
{
    father->children.push_back(child);
}

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
#endif // TREE_HPP


