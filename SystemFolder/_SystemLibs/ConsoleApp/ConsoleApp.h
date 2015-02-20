#ifndef CONSOLE_APP_H_
#define CONSOLE_APP_H_

#include "io.h"
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "string"

#include "..\Throw\Throw.h"

#define PARAMS_FILE_OPEN(var, name, mode)\
if (var) fclose (var);\
var = fopen (name, mode);


using namespace std;

string GetMyWay (const char FirstArgument []);

bool ModuleExist (const char ModuleName [], string Way);

string GetMyWay (const char FirstArgument [])
{
    string Return = FirstArgument;

    for (int i = Return.size () - 1; Return [i] != '\\' && i > -1; i --) Return.resize (i);

    return Return;
}

bool ModuleExist (const char ModuleName [], string Way)
{
    Way += ModuleName;
    return access (Way.c_str (), 0) != -1;
}

#endif
