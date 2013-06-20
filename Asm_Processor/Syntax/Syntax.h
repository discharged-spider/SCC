#ifndef SYNTAX__
#define SYNTAX__

namespace Syntax
{
    const int  ComandNumber                                   = 27;
    const int  ComandMaxSize                                  = 7;

    struct NewComand
    {
        const char* Name;
        const int   Descriptor;
        const int   Params;
    };

    const NewComand Comands [ComandNumber] =
    {
        #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) {NAME, DESCRIPTOR, PARAMS},
        #include "ComandDef.h"
        #undef COMAND_DEF
    };
}

#endif
