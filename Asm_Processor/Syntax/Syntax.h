#ifndef SYNTAX__
#define SYNTAX__

namespace Syntax
{
    const int JIT_MODE  = 'J' + 'I' + 'T';
    const int PROC_MODE = 'P' + 'R' + 'O' + 'C';

    const int  CommandsNumber = 0
        #define COMMAND_DEF(NUM, NAME, STR, DESCRIPTOR, PARAMS, CODE) + 1
        #include "CommandDef.h"
        #undef COMMAND_DEF
    ;


    const int  CommandMaxSize = 7;

    enum COMMANDS
    {
        #define COMMAND_DEF(NUM, NAME, STR, DESCRIPTOR, PARAMS, CODE) NAME = DESCRIPTOR,
        #include "CommandDef.h"
        #undef COMMAND_DEF
    };

    struct NewCommand
    {
        const char* Name;
        const int   Descriptor;
        const int   Params;
    };

    const NewCommand Commands [CommandsNumber] =
    {
        #define COMMAND_DEF(NUM, NAME, STR, DESCRIPTOR, PARAMS, CODE) {STR, DESCRIPTOR, PARAMS},
        #include "CommandDef.h"
        #undef COMMAND_DEF
    };
}

#endif
