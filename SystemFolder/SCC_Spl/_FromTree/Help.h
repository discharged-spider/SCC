#ifndef HELP_H__
#define HELP_H__

#include "stdio.h"
#include "stdlib.h"
#include "strings.h"

int  ListSize (const char ListName [], string Way);
void AddListItem (const char ListName [], int i, FILE* To, string Way);
void AddRandListItem (const char ListName [], FILE* To, string Way);

bool Ver (int percens);

template <typename T>
T Mod (T a)
{
    return (a > 0)? a : -a;
}

//------------------------------------------------------------------------------

int  ListSize (const char ListName [], string Way)
{
    Way += "..\\Lists\\";
    Way += ListName;
    Way += ".wordlist";

    FILE* List = fopen (Way.c_str (), "rb");

    if (!List) throw TH_ERROR "List %s don't exist!", ListName);

    int Result = 0;
    char c;
    while (fread (&c, 1, 1, List) == 1)
    {
        if (c == '\n' || c == '\r')
        {
            Result ++;

            while (fread (&c, 1, 1, List) == 1 && (c == '\n' || c == '\r')) ;
        }
    }

    fclose (List);

    return Result;
}
void AddListItem (const char ListName [], int i, FILE* To, string Way)
{
    Way += "..\\Lists\\";
    Way += ListName;
    Way += ".wordlist";
    FILE* List = fopen (Way.c_str (), "rb");

    if (!List) throw TH_ERROR "List %s don't exist!", ListName);

    int Num = 0;
    char c;
    while (true)
    {
        if (fread (&c, 1, 1, List) != 1) break;
        if (c == '\n' || c == '\r')
        {
            if (Num == i) break;
            Num ++;

            while (fread (&c, 1, 1, List) == 1 && (c == '\n' || c == '\r')) ;
        }
        if (Num == i)
        {
            fwrite (&c, 1, 1, To);
        }
    }

    fclose (List);
}

void AddRandListItem (const char ListName [], FILE* To, string Way)
{
    AddListItem (ListName, rand () % ListSize (ListName, Way), To, Way);
}

bool Ver (int percens)
{
    return (rand () % 100) < percens;
}

#endif
