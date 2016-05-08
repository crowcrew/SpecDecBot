#ifndef PARSER_HPP
#define PARSER_HPP

#include <bits/stdc++.h>
#include "tree.hpp"
#include "global.hpp"
using namespace std;

node* tree_head = new node;
vector<node*> level;

bool ok_full(vector<string> line_t)
{
    bool flag;
    for(int i=0; i<rules.size()-1; i++)
    {
        flag=0;
        if(rules[i].size()==line_t.size())
            for(int counter = 0; counter<line_t.size(); counter++)
            {
                if(rules[i][counter]!=line_t[counter])
                    flag = 1;
            }
        else
            flag=1;
        if (flag==0)
            return true;
    }
    return false;
}

int ok_partial(vector<string> line_t)
{
    bool flag;
    for(int i=0; i<rules.size()-1; i++)
    {
        flag=0;
        if(rules[i].size()-1==line_t.size())
            for(int counter = 0; counter<line_t.size(); counter++)
            {
                if(rules[i][counter+1]!=line_t[counter])
                    flag = 1;
            }
        else
            flag=1;
        if (flag==0)
            return i;
    }
    return -1;
}

void parse_LtR(vector<string> line)
{
    /**
    Parsing Left to Right
    */
    for(int counter = 0; counter<=line.size()-1; counter++)
    {
        vector<string> line_t;
        if(counter==line.size()-1)
            line_t.push_back(line[line.size()-1]);
        else for(int counter_t = counter; counter_t<line.size(); counter_t++)
            {
                line_t.push_back(line[counter_t]);
                cout<<"debugging LtR : "<<line[counter_t]<<endl;
            }
        if(ok_full(line_t))
        {
            cout<<"ok_full\n";
            node* head = new node;
            head->value = line_t[0];
            for(int i = 1; i<line_t.size(); i++)
            {
                node* child = new node;
                child->value = line_t[i];
                node_connect(head,child);
                cout<<"debugging L child ok_full: "<<child->value<<endl;
            }
            level.push_back(head);
            cout<<"debugging L head ok_full: "<<head->value<<endl;
        }
        else if(ok_partial(line_t)>=0)
        {
            cout<<"ok_par\n";
            node* head = new node;
            head->value = rules[ok_partial(line_t)][0];
            for(int i = 0; i<line_t.size(); i++)
            {
                node* child = new node;
                child->value = line_t[i];
                node_connect(head,child);
                cout<<"debugging L child ok_par: "<<child->value<<endl;
            }
            level.push_back(head);
            cout<<"debugging L head ok_par: "<<head->value<<endl;
        }
        else
        {
            cout<<"not_ok\n";
            node* head = new node;
            head->value = line[counter];
            level.push_back(head);
            cout<<"debugging L head not_ok: "<<head->value<<endl;
        }
    }
}

void parse_RtL(vector<string> line)
{
    /**
    Parsing Right to Left
    */
    for(int counter = line.size()-1; counter>=0; counter--)
    {
        vector<string> line_t;
        for(int counter_t = 0; counter_t<=counter; counter_t++)
        {
            line_t.push_back(line[counter_t]);
            cout<<"debugging RTL : "<<line[counter_t]<<endl;
            if(ok_full(line_t))
            {
                node* head = new node;
                head->value = line_t[0];
                for(int i = 1; i<line_t.size(); i++)
                {
                    node* child = new node;
                    child->value = line_t[i];
                    node_connect(head,child);
                    cout<<"debugging R child ok_full: "<<child->value<<endl;
                }
                counter = counter_t-1;
                level.push_back(head);
                cout<<"debugging R head ok_full: "<<head->value<<endl;
            }
            else if(ok_partial(line_t)>=0)
            {
                node* head = new node;
                head->value = rules[ok_partial(line_t)][0];
                for(int i = 0; i<line_t.size(); i++)
                {
                    node* child = new node;
                    child->value = line_t[i];
                    node_connect(head,child);
                    cout<<"debugging R child ok_par: "<<child->value<<endl;
                }
                counter = counter_t-1;
                level.push_back(head);
                cout<<"debugging R head ok_par: "<<head->value<<endl;
            }
            else
            {
                node* head = new node;
                head->value = line[counter];
                level.push_back(head);
                cout<<"debugging R head not_ok: "<<head->value<<endl;
            }
        }
        line_t.clear();
    }
}

bool complete_cycle(vector<string> permutation)
{
    for(int counter = 0; counter<permutation.size(); counter++)
    {
        node* head = new node;
        head->value = permutation[counter];
        level.push_back(head);
        cout<<"debugging perm "<<level[counter]->value<<endl;
    }
    while(!level.empty())
    {
        if(level.size()>permutation.size())
            return false;
        if(level.size()==1&&level[0]->value=="s")
            return true;
        vector<string> line;
        for(int i=0; i<level.size(); i++)
            line.push_back(level[i]->value);
        level.clear();
        cout<<"debugging cycle\n";
        parse_LtR(line);
        if(level.size()>permutation.size())
            return false;
        if(level.size()==1&&level[0]->value=="s")
            return true;
        line.clear();
        for(int i=0; i<level.size(); i++)
            line.push_back(level[i]->value);
        level.clear();
        parse_RtL(line);
    }
    return false;
}

void cycle_through()
{
    vector<string> permutation;
    while(!permutations.empty())
    {
        permutation = permutations.front();
        permutations.pop();
        if(complete_cycle(permutation))
            return;
        permutation.clear();
    }
    cout<<"tree head not found!\n";
}
#endif
