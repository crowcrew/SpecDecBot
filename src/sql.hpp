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

class sql
{
public:
    sql(); // constructor
    void connect(); // connects to the database on-disk and maps it to the sqlite3* MemoryDatabase object in memory
    void query(string queried_statement); // queries the statement at hand at GLOBAL_HPP class
private:
    sqlite3 *MemoryDatabase; // sqlite3 object, an on-memory map of the on-disk databse.db file
    char *error; // error message to be passed to the sqlite3_exec() function
    int state; // state returned from sqlite3 functions
    const char* message; // message to be passed to the callback() function
};

sql::sql()
{
    connect();
}

void sql::connect()
{
    error = 0;
    state = sqlite3_open("../src/database.db", &MemoryDatabase);
    if ( state )
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(MemoryDatabase));
        exit(0);
    }
}

string field; // to store the query result, using the callback function

int callback(void *message, int argc, char **argv, char **col_name)
{
    field.assign(argv[0]);
    return 0;
}

void sql::query(string queried_statement)
{
    state=sqlite3_exec(MemoryDatabase,queried_statement.c_str(),callback,(void*)message,&error);
    if(state!=SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
    }
    queried_statement.clear();
}

#endif // SQL_HPP
