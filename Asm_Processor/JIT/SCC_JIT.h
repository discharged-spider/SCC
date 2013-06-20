#ifndef SCC_JIT_H_
#define SCC_JIT_H_

#include "cstdio"

#include "iostream"

#include "vector"
#include "stack"

#include "windows.h"

#include <time.h>

#define GetPTinMS ((double)clock() * (double)1000 / (double)CLOCKS_PER_SEC)

#include "..\\..\\SystemFolder\\_SystemLibs\\Throw\\Throw.h"

#include "JITLinkFunc.h"

using namespace std;

void Load (FILE* file, vector <double>& Program, int* ProgramSize);

void Compile (double Program [], int ProgramSize, vector<unsigned char>& Output, unsigned int* OutputSize, stack <unsigned int>& ToLink);

#define STANDART_LINK_FUNCTIONS JITLinkFunctions::Echo, JITLinkFunctions::Get, JITLinkFunctions::JITMemory::PushMS, JITLinkFunctions::JITMemory::PopMS, JITLinkFunctions::JITMemory::Add, JITLinkFunctions::JITMemory::Rem
void Link (vector<unsigned char>& Program, unsigned int Size, stack <unsigned int> ToLink, void Echo (), void Get (), void PushMS (), void PopMS (), void Add (), void Rem ());

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

void Compile (double Program [], int ProgramSize, vector<unsigned char>& Output, unsigned int* OutputSize, stack <unsigned int>& ToLink)
{
    *OutputSize = 0;

    vector <int> Marks;
    stack  <unsigned int> ToReplace;

    #define TRASH 0

    #define PUSH_BYTE(B) Output.push_back ((unsigned char)(B)); *OutputSize += 1;

    #include "JITCodes.h"
    #include "LinkCodes.h"

    FINIT //Ony at start

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
        unsigned int ADDR = (int)DebugOutComand;
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

            default:
            {
                printf ("Unknown comand(%lg)!\n", Program [i]);

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

void Link (vector<unsigned char>& Program, unsigned int Size, stack <unsigned int> ToLink, void Echo (), void Get (), void PushMS (), void PopMS (), void Add (), void Rem ())
{
    while (!ToLink.empty ())
    {
        unsigned int LinkN = *((unsigned int*)&Program[ToLink.top ()]);

        int TEMP = (int)NULL;
        switch (LinkN)
        {
            case (LINK_PUSHMS):
            {
                TEMP = (int)(PushMS);

                break;
            }
            case (LINK_POPMS):
            {
                TEMP = (int)PopMS;

                break;
            }
            case (LINK_ADD):
            {
                TEMP = (int)Add;

                break;
            }
            case (LINK_REM):
            {
                TEMP = (int)Rem;

                break;
            }

            case (LINK_ECHO):
            {
                TEMP = (int)Echo;

                break;
            }
            case (LINK_GET):
            {
                TEMP = (int)Get;

                break;
            }

            default:
            {
                TEMP = (int)NULL;
                break;
            }
        };

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
    __asm
    (
        "call *%0"
        :
        : "r"(&Program [0])
        : "eax", "ebx", "ecx", "edx"
    );
    printf ("\n<<<<===================>>>>\n");
    double TimeEnd = GetPTinMS;
    printf ("Program done for %lg ms.\n", TimeEnd - TimeStart);
}

#endif
