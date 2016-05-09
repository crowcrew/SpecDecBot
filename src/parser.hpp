#ifndef PARSER_HPP
#define PARSER_HPP

#include <bits/stdc++.h>
#include "tree.hpp"
#include "global.hpp"
using namespace std;

node* tree_head = new node;
vector<node*> level;

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

void parse_LtR()
{
    /**
    Parsing Left to Right
    */
    vector<node*> level_t;
    for(int counter = 0; counter<=level.size()-1; counter++)
    {
        vector<string> line_t;
        vector<node*> level_tt;
        for(int counter_t = counter; counter_t<level.size(); counter_t++)
        {
            line_t.push_back(level[counter_t]->value);
            level_tt.push_back(level[counter_t]);
            logging("debugging LtR : ",level[counter_t]->value);
        }
        if(ok_full(line_t))
        {
            //logging("ok_full\n",NULL);
            node* head = new node;
            head = level_tt[0];
            for(int i = 1; i<line_t.size(); i++)
            {
                node* child = new node;
                child = level_tt[i];
                node_connect(head,child);
                logging("debugging L child ok_full: ",child->value);
            }
            level_t.push_back(head);
            logging("debugging L head ok_full: ",head->value);
        }
        else if(ok_partial(line_t)>=0)
        {
            //logging("ok_par\n",NULL);
            node* head = new node;
            head->value = rules[ok_partial(line_t)][0];
            for(int i = 0; i<line_t.size(); i++)
            {
                node* child = new node;
                child  = level_tt[i];
                node_connect(head,child);
                logging("debugging L child ok_par: ",child->value);
            }
            level_t.push_back(head);
            logging("debugging L head ok_par: ",head->value);
        }
        else
        {
            //logging("not_ok\n",NULL);
            node* head = new node;
            head = level[counter];
            level_t.push_back(head);
            logging("debugging L head not_ok: ",head->value);
        }
    }
    level.clear();
    for(int i=0; i<level_t.size(); i++)
        level.push_back(level_t[i]);
    logging("LTR complete","!");
}

void parse_RtL()
{
    /**
    Parsing Right to Left
    */
    vector<node*> level_t;
    stack <node*> node_stk;
    string temp;
    for(int j=0; j<level.size(); j++)
    {
        temp+=level[j]->value;
        temp+=" ";
    }
    logging(temp,"!");
    int crawler=0;
    for(int counter = level.size()-1; counter>=0; counter--)
    {
        vector<node*> level_tt;
        vector<string> line_t;
        for(int counter_t = crawler; counter_t<=counter; counter_t++)
        {
            line_t.push_back(level[counter_t]->value);
            level_tt.push_back(level[counter_t]);
            logging("debugging RTL : ",level[counter_t]->value);
            if(ok_full(line_t))
            {
                node* head = new node;
                head = level_tt[0];
                for(int i = 1; i<line_t.size(); i++)
                {
                    node* child = new node;
                    child = level_tt[i];
                    node_connect(head,child);
                    logging("debugging R child ok_full: ",child->value);
                }
                crawler = counter_t+1;
                level_t.push_back(head);
                line_t.clear();
                level_tt.clear();
                logging("debugging R head ok_full: ",head->value);
            }
            else if(ok_partial(line_t)>=0)
            {
                node* head = new node;
                head->value = rules[ok_partial(line_t)][0];
                for(int i = 0; i<line_t.size(); i++)
                {
                    node* child = new node;
                    child = level_tt[i];
                    node_connect(head,child);
                    logging("debugging R child ok_par: ",child->value);
                }
                crawler = counter_t+1;
                level_t.push_back(head);
                line_t.clear();
                level_tt.clear();
                logging("debugging R head ok_par: ",head->value);
            }
            else
            {
                if(counter_t==counter)
                {
                    node* head = new node;
                    head = level[counter];
                    node_stk.push(head);
                    logging("debugging R head not_ok: ",head->value);
                    line_t.clear();
                }
            }
        }
    }
    while(!node_stk.empty())
    {
        node* temp = new node;
        temp = node_stk.top();
        level_t.push_back(temp);
        node_stk.pop();
    }
    level.clear();
    for(int i=0; i<level_t.size(); i++)
        level.push_back(level_t[i]);
    logging("RTL complete","!");
}

bool complete_cycle(vector<string> permutation)
{
    level.clear();
    logging("next cycle","!");
    for(int counter = 0; counter<permutation.size(); counter++)
    {
        node* head = new node;
        head->value = permutation[counter];
        level.push_back(head);
        logging("debugging perm ",level[counter]->value);
    }
    while(!level.empty())
    {
        if(level.size()>permutation.size())
            return false;
        if(level.size()==1&&level[0]->value=="s")
            return true;
        bool flag=0;
        for(int i=0; i<level.size(); i++)
        {
            if(flag==0&&level[i]->value=="s")
                flag=1;
            if(flag==1&&level[i]->value=="s")
                return false;
        }
        parse_LtR();
        if(level.size()>permutation.size())
            return false;
        if(level.size()==1&&level[0]->value=="s")
            return true;
        flag=0;
        for(int i=0; i<level.size(); i++)
        {
            if(flag==0&&level[i]->value=="s")
                flag=1;
            if(flag==1&&level[i]->value=="s")
                return false;
        }
        parse_RtL();
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

    logging("tree head not found!","");
}
#endif
