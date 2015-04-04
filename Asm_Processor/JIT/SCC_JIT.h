#ifndef SCC_JIT_H_
#define SCC_JIT_H_

#include "cstdio"

#include "iostream"

#include "vector"
#include "stack"

#include "windows.h"

#include <time.h>

#include "map"

#define GetPTinMS ((double)clock() * (double)1000 / (double)CLOCKS_PER_SEC)

#include "..\..\SystemFolder\_SystemLibs\Throw\Throw.h"

#include "..\Syntax\Syntax.h"

#include "JITLinkFunc.h"

using std::map;
using std::vector;

void Load (FILE* file, vector <double>& Program, int* ProgramSize);

void Compile (double Program [], int ProgramSize, vector<unsigned char>& Output, unsigned int* OutputSize, stack <unsigned int>& ToLink, bool UseInspector);

void InitFPU (vector<unsigned char>& Output, unsigned int* OutputSize);

#define STANDART_LINK_FUNCTIONS JITLinkFunctions::getFunctions ()
void Link (vector<unsigned char>& Program, unsigned int Size, stack <unsigned int> ToLink, map <int, void*> functions);

#define ST_EXECUTE(Program) Execute (Program, JITLinkFunctions::JITMemory::Clear)
void Execute (vector<unsigned char> Program, void ClearMemory ());

//-------------------------------------------------------------------------------------------------

void Load (FILE* file, vector <double>& Program, int* ProgramSize)
{
    *ProgramSize = 0;

    while (true)
    {
        double TEMP;
        if (fread (&TEMP, sizeof (double), 1, file) != 1) break;

        Program.push_back (TEMP);
        *ProgramSize += 1;
    }
}

void Compile (double Program [], int ProgramSize, vector<unsigned char>& Output, unsigned int* OutputSize, stack <unsigned int>& ToLink, bool UseInspector)
{
    *OutputSize = 0;

    vector <int> Marks;
    stack  <unsigned int> ToReplace;

    #define TRASH 0

    #define PUSH_BYTE(B) Output.push_back ((unsigned char)(B)); *OutputSize += 1;

    #include "JITCodes.h"
    #include "LinkCodes.h"

    InitFPU (Output, OutputSize);

    MOV_32_REG (EAX)
    int PROTECTOR = -1;
    unsigned char* PROTECTOR_TO_CHAR = (unsigned char*)&PROTECTOR;
    PUSH_BYTE (PROTECTOR_TO_CHAR[0])
    PUSH_BYTE (PROTECTOR_TO_CHAR[1])
    PUSH_BYTE (PROTECTOR_TO_CHAR[2])
    PUSH_BYTE (PROTECTOR_TO_CHAR[3])

    PUSH_32_REG (EAX) //PROTECTOR
    PUSH_32_REG (EAX) //PROTECTOR
    PUSH_32_REG (EAX) //PROTECTOR
    PUSH_32_REG (EAX) //PROTECTOR

    int DownPos = -1; //Function not created yet

    for (int i = 0; i < ProgramSize; i ++)
    {
        #ifdef DEBUG
        {
        MOV_32_REG (EBX)
        unsigned int DATA = (unsigned int)Program [i];
        unsigned char* DATA_TO_CHAR = (unsigned char*)&DATA;
        PUSH_BYTE (DATA_TO_CHAR[0])
        PUSH_BYTE (DATA_TO_CHAR[1])
        PUSH_BYTE (DATA_TO_CHAR[2])
        PUSH_BYTE (DATA_TO_CHAR[3])

        MOV_32_REG (EAX)
        unsigned int ADDR = (int)DebugOutCommand;
        unsigned char* ADDR_TO_CHAR = (unsigned char*)&ADDR;
        PUSH_BYTE (ADDR_TO_CHAR[0])
        PUSH_BYTE (ADDR_TO_CHAR[1])
        PUSH_BYTE (ADDR_TO_CHAR[2])
        PUSH_BYTE (ADDR_TO_CHAR[3])

        CALL_32_ADDR_EAX
        }
        #endif

        switch ((int)Program [i])
        {
            #include "JIT_SWITCH/JIT_SWITCH_0_.h" //All comands < 10

            #include "JIT_SWITCH/JIT_SWITCH_1_.h" //All comands >= 10 < 20

            #include "JIT_SWITCH/JIT_SWITCH_2_.h" //All comands >= 20 < 30

            #include "JIT_SWITCH/JIT_SWITCH_3_.h" //All comands >= 30 < 40

            #include "JIT_SWITCH/JIT_SWITCH_4_.h" //All comands >= 40 < 50

            #include "JIT_SWITCH/JIT_SWITCH_5_.h" //All comands >= 50 < 60

            #include "JIT_SWITCH/JIT_SWITCH_6_.h" //All comands >= 60 < 70

            default:
            {
                printf ("Unknown command(%g)!\n", Program [i]);

                break;
            }
        }
    }

    POP_32_REG(EAX)
    POP_32_REG(EAX)
    POP_32_REG(EAX)
    POP_32_REG(EAX)

    RET_NEAR

    while (!ToReplace.empty ())
    {
        unsigned int MarkN = *((unsigned int*)&Output[ToReplace.top ()]);

        if (MarkN >= Marks.size () || Marks [MarkN] == -1) printf ("Can't find mark!\n");

        int TEMP = Marks [MarkN] - (ToReplace.top () + 4);
        unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
        Output [ToReplace.top () + 0] = TEMP_TO_CHAR[0];
        Output [ToReplace.top () + 1] = TEMP_TO_CHAR[1];
        Output [ToReplace.top () + 2] = TEMP_TO_CHAR[2];
        Output [ToReplace.top () + 3] = TEMP_TO_CHAR[3];

        ToReplace.pop ();
    }
}

void InitFPU (vector<unsigned char>& Output, unsigned int* OutputSize)
{
    FINIT //Only at start

    FCLEX

    /*
    Set FPU control word (for right %):

    push AX

    fstcw word ptr [ESP]
    and word ptr [ESP], 1001111111111b
    or  word ptr [ESP], 0110000000000b
    fldcw word ptr [ESP]

    pop AX

    */
    //{

    PUSH_BYTE (0x66)
    PUSH_BYTE (0x50)

    PUSH_BYTE (0xD9)
    PUSH_BYTE (0x3C)
    PUSH_BYTE (0x24)
    PUSH_BYTE (0x66)
    PUSH_BYTE (0x81)
    PUSH_BYTE (0x24)
    PUSH_BYTE (0x24)
    PUSH_BYTE (0xFF)
    PUSH_BYTE (0x13)
    PUSH_BYTE (0x66)
    PUSH_BYTE (0x81)
    PUSH_BYTE (0x0C)
    PUSH_BYTE (0x24)
    PUSH_BYTE (0x00)
    PUSH_BYTE (0x0C)
    PUSH_BYTE (0xD9)
    PUSH_BYTE (0x2C)
    PUSH_BYTE (0x24)

    PUSH_BYTE (0x66)
    PUSH_BYTE (0x58)

    //}
}

void Link (vector<unsigned char>& Program, unsigned int Size, stack <unsigned int> ToLink, map <int, void*> functions)
{
    while (!ToLink.empty ())
    {
        unsigned int LinkN = *((unsigned int*)&Program[ToLink.top ()]);

        int TEMP = (int)NULL;
        TEMP = (int)functions [LinkN];

        unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
        Program [ToLink.top () + 0] = TEMP_TO_CHAR[0];
        Program [ToLink.top () + 1] = TEMP_TO_CHAR[1];
        Program [ToLink.top () + 2] = TEMP_TO_CHAR[2];
        Program [ToLink.top () + 3] = TEMP_TO_CHAR[3];

        ToLink.pop ();
    }
}

void Execute (vector<unsigned char> Program, void ClearMemory ())
{
    ClearMemory ();

    double TimeStart = GetPTinMS;
    printf ("<<<<===================>>>>\n");
    __asm __volatile__
    (
        "call *%0"
        :
        : "r"(&Program [0])
        : "eax", "ebx", "ecx", "edx"
    );
    printf ("\n<<<<===================>>>>\n");
    double TimeEnd = GetPTinMS;
    printf ("Program done for %g ms.\n", TimeEnd - TimeStart);
}

#endif
