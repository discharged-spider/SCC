#ifndef JIT_LINK_FUNC_H_
#define JIT_LINK_FUNC_H_

#include "cstdio"

#include "iostream"

#include "vector"
#include "map"
#include "stack"

#include "windows.h"

#include "..\Syntax\VariablesInspector.h"

#include "LinkCodes.h"

using std::vector;
using std::map;
using std::stack;

namespace JITLinkFunctions
{
    FILE* input;
    FILE* output;

    map <int, void*> functions_;

    #include "JITMemory.h"

    newVariablesInspector Inspector;

    map <int, void*> getFunctions ();

    void Echo ();
    void Get ();

    void LogGetVar ();
    void LogGetArr ();
    void LogSetVar ();
    void LogSetArr ();
}

//-------------------------------------------------------------------------------------------------

int getEBX ();void DebugOutCommand ();

//-------------------------------------------------------------------------------------------------

int getEBX ()
{
    int result = 0;

    __asm __volatile__
    (
        "mov %%ebx, %0"
        : "=m"(result)
        :
        : "ebx"
    );

    return result;
}

void DebugOutCommand ()
{
    int Code;
    double* ESP;

    __asm __volatile__
    (
        "mov %%ebx, %0 \n"
        "mov %%esp, %1 \n"
        : "=m"(Code), "=m"(ESP)
        :
        : "ebx", "esp"
    );

    ESP += 8;

    static int BASE_ESP = -1;
    if (BASE_ESP == -1) BASE_ESP = (int)ESP + 16;

    int ESPSize = (BASE_ESP - (int)ESP) / 8;

    printf ("[============]\n");
    printf ("ESP = %d\n", ESPSize);

    printf ("Comand to execute = ");
    #define COMMAND_DEF(NUM, NAME, STR, DESCRIPTOR, PARAMS, CODE) if (Code == DESCRIPTOR) printf ("%s(\"%s\")\n", #NAME, STR);
    #include "..\Syntax\CommandDef.h"
    #undef COMMAND_DEF

    printf ("Stack dump\n");
    for (int i = 0; i < ESPSize; i ++)
    {
        double Value = *(ESP + i);
        int* IValue = (int*)&Value;
        printf ("%g(%d_%d) ", Value, IValue [0], IValue [1]);
    }
    printf ("\n");

    printf ("[============]\n");
    system ("pause");
}

//-------------------------------------------------------------------------------------------------

map <int, void*> JITLinkFunctions::getFunctions ()
{
    if (functions_.size () > 0) return functions_;

    functions_ [LINK_ECHO]   = (void*)Echo;
    functions_ [LINK_GET]    = (void*)Get;

    functions_ [LINK_PUSHMS] = (void*)JITMemory::PushMS;
    functions_ [LINK_POPMS]  = (void*)JITMemory::PopMS;
    functions_ [LINK_ADD]    = (void*)JITMemory::Add;
    functions_ [LINK_REM]    = (void*)JITMemory::Rem;

    functions_ [LINK_LOG_GET_VAR] = (void*)LogGetVar;
    functions_ [LINK_LOG_GET_ARR] = (void*)LogGetArr;
    functions_ [LINK_LOG_SET_VAR] = (void*)LogSetVar;
    functions_ [LINK_LOG_SET_ARR] = (void*)LogSetArr;

    return functions_;
}

//-------------------------------------------------------------------------------------------------

void JITLinkFunctions::Echo ()
{
    int ToPrintf = 0;

    __asm __volatile__
    (
        "mov %%ebx, %0"
        : "=m"(ToPrintf)
        :
        : "ebx"
    );

    if (output == NULL)
    {
        if (ToPrintf == -1) system ("cls");
        else                printf ("%c", (char)ToPrintf);
    }
    else
    {
        fprintf (output, "%c", (char)ToPrintf);
    }
}
void JITLinkFunctions::Get ()
{
    char GetValue;
    if (input == NULL) scanf ("%c", &GetValue);
    else               fscanf (input, "%c", &GetValue);


    double Value = GetValue;
    unsigned int* IValue = (unsigned int*)&Value;
    __asm __volatile__
    (
        "mov %0, %%ecx \n"
        "mov %1, %%edx \n"
        :
        : "m"(IValue[0]), "m"(IValue[1])
    );
}

//-------------------------------------------------------------------------------------------------

void JITLinkFunctions::LogGetVar ()
{
    Inspector.get_var (getEBX ());
}
void JITLinkFunctions::LogGetArr ()
{
    Inspector.get_arr (getEBX ());
}
void JITLinkFunctions::LogSetVar ()
{
    Inspector.set_var (getEBX ());
}
void JITLinkFunctions::LogSetArr ()
{
    Inspector.set_arr (getEBX ());
}

//-------------------------------------------------------------------------------------------------

#endif
