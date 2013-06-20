#ifndef CONSOLE_APP_H_
#define CONSOLE_APP_H_

#include "io.h"
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "string"

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
