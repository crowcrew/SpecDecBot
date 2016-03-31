/*
SpecDecBot (modified zlib) License

Copyright (c) 2015 SpecDecBot Project (Aly Shmahell, Alya Salman, Elias Soud, Mohammad Shbani, Ruaa Sleiman, Sarah Ammar).

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation is required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <bits/stdc++.h>
#include <sqlite3.h>
using namespace std;

#ifndef SQL_HPP
#define SQL_HPP


sqlite3 *db; // sqlite3 object
char *ERROR; // ERROR message
int STATE; // state returned from sqlite3 functions
string statement; // SQL statement to be queried
string field;
const char* message; // callback function message



void connect()
{
    ERROR = 0;
    STATE=sqlite3_open("database.db", &db);
    if( STATE )
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
}

int callback(void *message, int argc, char **argv, char **col_name)
{
    int i;
    for(i=0; i<argc; i++)
    {
        printf("%s = %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
        field = argv[i];
    }
    printf("\n");
    return 0;
}
void query(const char *statement,const char *message)
{
    STATE=sqlite3_exec(db,statement,callback,(void*)message,&ERROR);
    if(STATE!=SQLITE_OK)
    {
        fprintf(stderr, "SQL ERROR: %s\n", ERROR);
        sqlite3_free(ERROR);
    }
}
string select(string str)
{
    message = "peep";
    statement="SELECT POS1 FROM DIC WHERE WORD = '";
    statement+=str;
    statement+="'";
    query(statement.c_str(),message);
    statement.clear();
}
#endif // LEXER_HPP
