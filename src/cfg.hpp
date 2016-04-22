#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>

using namespace std;

#ifndef CFG_HPP
#define CFG_HPP

class cfg
{
public:
    vector< vector<string> > rules;
    cfg();
    void debug();
};

cfg::cfg()
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

void cfg::debug()
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

#endif // CFG_HPP
