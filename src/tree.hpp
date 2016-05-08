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

string tree_dfs(node* head)
{
    stack <node*> bucket;
    string line;
    bucket.push(head);
    while(!bucket.empty())
    {
        node* current_node = new node;
        current_node = bucket.top();
        bucket.pop();
        line+=current_node->value;
        if(current_node->children.size())
            line+="+";
        else
            line+="-";
        for(int i=0; i<current_node->children.size(); i++)
            bucket.push(current_node->children[i]);
    }
    return line;
}
#endif // TREE_HPP


