#ifndef PROCESSOR__
#define PROCESSOR__

#include "math.h"
#include "..\Syntax\Syntax.h"
#include "..\Compiler\Assem.h"
#include "..\\..\\SystemFolder\\_SystemLibs\\Stack\\Stack.h"
#include "..\\..\\SystemFolder\\_SystemLibs\\Throw\\Throw.h"

//==============================================================================

class newProcessor
{
    public:
    int                MemorySize_;
    newVector <bool>   FreeMemory_;
    newVector <double> Memory_;

    newStack  <double> Stack_;

    double* Program_;
    int     Size_;

    void SetProgram (double Program [], int Size);
    int  ExecuteProgram ();

    void Add ();
    void Rem ();

    void MemoryDump ();
};

//------------------------------------------------------------------------------

void newProcessor::SetProgram (double Program [], int Size)
{
    Program_ = Program;
    Size_    = Size;

    MemorySize_ = 0;
}

int newProcessor::ExecuteProgram ()
{
    int Addr = rand ();
    Stack_.Push (Addr);

    int CommandsN = 0;

    for (int i = 0; i < Size_; i ++)
    {
        CommandsN ++;

        #ifdef DEBUG
            #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if ((int)Program_ [i] == DESCRIPTOR) printf ("Comand to execut = %s\n", NAME);
            #include "..\Syntax\ComandDef.h"
            #undef COMAND_DEF

            Stack_.Dump();
            system ("pause");
        #endif

        switch ((int)Program_ [i])
        {
            #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) \
            case (DESCRIPTOR): \
            { \
                CODE; \
                i += PARAMS; \
                break; \
            }

            #include "..\Syntax\ComandDef.h"
            #undef COMAND_DEF

            default:
            {
                printf ("Unknown comand(%lg)!\n", Program_ [i]);

                break;
            }
        }
    }

    if (Stack_.Pop () != Addr) printf ("__!!!ERROR!!!__\"can't return from program, check stack\"__\n");

    return CommandsN;
}

void newProcessor::Add ()
{
    int Size = (int)Stack_.Pop ();

    for (int i = 0, c = 0; i < MemorySize_; i ++)
    {
        if (FreeMemory_ [i])
        {
            c ++;
        }
        else c = 0;

        if (c == Size)
        {
            for (int j = i - c + 1; j < i + 1; j ++)
            {
                FreeMemory_ [j] = false;
            }

            Stack_.Push (i - c + 1);

            return ;
        }
    }

    for (int j = MemorySize_; j < MemorySize_ + Size; j ++)
    {
        FreeMemory_ [j] = false;
    }

    Stack_.Push (MemorySize_);

    MemorySize_ += Size;
}

void newProcessor::Rem ()
{
    int Addr = (int)Stack_.Pop ();
    int Size = (int)Stack_.Pop ();

    for (int i = 0; i < Size; i ++)
    {
        FreeMemory_ [Addr + i] = true;
    }
}

void newProcessor::MemoryDump ()
{
    for (int i = 0; i < MemorySize_; i ++)
    {
        printf ("%c ", (FreeMemory_[i])? 'X' : 'V');
    }
    printf ("\n");
    for (int i = 0; i < MemorySize_; i ++)
    {
        printf ("%lg ", Memory_[i]);
    }
    printf ("\n");
}
//==============================================================================

void CreateProcessorProgram (FILE* file, newVector <double>& Program, int* Size)
{
    *Size = 0;

    while (true)
    {
        Program.SetSize (*Size + 1);
        if (fread (&Program[*Size], sizeof (double), 1, file) != 1) break;

        *Size += 1;
    }
}

#endif
