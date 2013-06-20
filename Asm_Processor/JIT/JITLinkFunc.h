#ifndef JIT_LINK_FUNC_H_
#define JIT_LINK_FUNC_H_

#include "cstdio"

#include "iostream"

#include "vector"
#include "stack"

#include "windows.h"

using namespace std;

namespace JITLinkFunctions
{
    #include "JITMemory.h"

    void Echo ();
    void Get ();
};

//-------------------------------------------------------------------------------------------------

void DebugOutComand ()
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
    #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (Code == DESCRIPTOR) printf ("%s\n", NAME);
    #include "..//Syntax//ComandDef.h"
    #undef COMAND_DEF

    printf ("Stack dump\n");
    for (int i = 0; i < ESPSize; i ++)
    {
        double Value = *(ESP + i);
        int* IValue = (int*)&Value;
        printf ("%lg(%d_%d) ", Value, IValue [0], IValue [1]);
    }
    printf ("\n");

    printf ("[============]\n");
    system ("pause");
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

    if (ToPrintf == -1) system ("cls");
    else                printf ("%c", (char)ToPrintf);
}
void JITLinkFunctions::Get ()
{
    int GetValue = getchar ();

    __asm __volatile__
    (
        "mov %0, %%ebx"
        :
        : "m"(GetValue)
    );
}

//-------------------------------------------------------------------------------------------------

#endif
