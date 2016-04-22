#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>

using namespace std;

#ifndef TREE_HPP
#define TREE_HPP

vector< queue<string> > iterations;
int tokens_iteration=0;

typedef struct node
{
    char* value;
    std::vector<node*> children;
}node;

void connect(node* father,node* child)
{
father->children.push_back(child);
}

void fill_iterations()
{
    for (int i=0;tokens[i][0]!='\0';i++)
    {

    }
}

void scan()
{

}

#endif // TREE_HPP





int main()
{

node* father_ = (node*) malloc(sizeof(node));
node* child_ = (node*) malloc(sizeof(node));

connect(father_,child_);


father_->children[0]->value="a";
cout<<child_->value;

}
