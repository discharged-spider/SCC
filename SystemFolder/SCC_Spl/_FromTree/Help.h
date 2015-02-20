#ifndef HELP_H__
#define HELP_H__

#include "iostream"

#include "stdio.h"
#include "stdlib.h"
#include "strings.h"

#include "map"

//------------------------------------------------------------------------------

int ListSize (const char ListName [], string Way);
void AddListItem (const char ListName [], int i, FILE* To, string Way);
void AddRandListItem (const char ListName [], FILE* To, string Way);

int randint (int min, int max);

//will generate random number from 1 to states_num, and will return it for next (states_num - 1) executions
int rand_state (int states_num);

bool coin_toss ();

//------------------------------------------------------------------------------

template <typename T>
T Mod (T a)
{
    return (a > 0)? a : -a;
}

struct Shuffler
{
    int max;

    std::map <int, int> new_i;

    Shuffler ()
    {
        max = -1;
    }

    int shuffle (int i)
    {
        assert (max != -1);

        if (new_i.count (i) == 0)
        {
            while (true)
            {
                new_i [i] = randint (0, max - 1);

                bool ok = true;
                for (std::map<int, int>::iterator it = new_i.begin(); it != new_i.end(); ++it)
                {
                    if (i != (*it).first && new_i [i] == (*it).second)
                    {
                        ok = false;
                        break;
                    }
                }

                if (ok) break;
            }
        }

        return new_i [i];
    }
};

//------------------------------------------------------------------------------

int ListSize (const char ListName [], string Way)
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
    #ifndef DEBUG

    AddListItem (ListName, randint (0, ListSize (ListName, Way) - 1), To, Way);

    #else

    fprintf (To, "%s", ListName);

    #endif
}

int randint (int min, int max)
{
    return min + rand () % (max - min + 1);
}

int rand_state (int states_num)
{
    static int last_num   = 0;
    static int calls_num  = 0;

    if (calls_num == 0)
    {
        last_num = randint (1, states_num);
        calls_num = states_num;
    }

    calls_num --;

    return last_num;
}

bool coin_toss ()
{
    return randint (0, 1) == 0;
}

#endif
