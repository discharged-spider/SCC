#ifndef ASSEM__
#define ASSEM__

#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "vector"
#include "..\\Syntax\\Syntax.h"
#include "..\\..\\SystemFolder\\_SystemLibs\\Stack\\Stack.h"
#include "..\\..\\SystemFolder\\_SystemLibs\\Throw\\Throw.h"

using namespace std;

#define __Size 30

//==============================================================================

const int  ExeptSymbolsSize = 4;
const char ExeptSymbols [ExeptSymbolsSize] = {' ', '\n', '\r', '\0'};

bool Exept (char Symbol);

//==============================================================================

bool ReadWord (FILE* file, char Word []);
bool ReadNum  (FILE* file, double* n);

//==============================================================================

int GetWordNum (const char Word []);
int GetDescNum (const int  Desc);

//==============================================================================

bool ReadProgram  (FILE* From, vector<double>& To, int* Size, FILE* ErrorOutput);
void WriteProgram (FILE* To, double Program[], int Size);

bool Assemble (vector<double> From, int FromSize, vector<double>& Program, int* Size, FILE* ErrorOutput);

bool JITPrepare (vector<double>& Program, int* Size, FILE* ErrorOutput);

//==============================================================================

bool Exept (char Symbol)
{
    for (int i = 0; i < ExeptSymbolsSize; i ++)
    {
        if (Symbol == ExeptSymbols [i]) return true;
    }

    return false;
}

//==============================================================================

bool ReadWord (FILE* file, char Word [])
{
    bool Start = false;
    char c     = 0;
    if (fread (&c, sizeof (c), 1, file) != 1) return false;

    int i = 0;
    for (; ; i ++)
    {
        if (i >= __Size) return false;

        if (Exept (c))
        {
           if (Start)
           {
               Word [i] = 0;

               if (i == 0) return false;

               break;
           }
           else i --;
        }
        else
        {
            if (!Start) Start = true;
            Word [i] = c;
        }

        if (fread (&c, sizeof (c), 1, file) != 1)
        {
            i ++;
            Word [i] = 0;
            break;
        }
    }

    return i > 0;
}

//==============================================================================

int GetWordDescNum (const char Word [])
{
    #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (strcmp (NAME, Word) == 0) return NUM;
    #include "..\Syntax\ComandDef.h"
    #undef COMAND_DEF

    return -1;
}
int GetDescWordNum (const int  Desc)
{
    #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (DESCRIPTOR == Desc) return NUM;
    #include "..\Syntax\ComandDef.h"
    #undef COMAND_DEF

    return -1;
}

//==============================================================================

bool ReadProgram (FILE* From, vector<double>& To, int* Size, FILE* ErrorOutput)
{
    try
    {
        *Size = 0;

        char Word [__Size] = "";
        int WordNum = 0;
        while (true)
        {
            if (!ReadWord (From, Word)) break;

            WordNum = GetWordDescNum (Word);
            if (WordNum == -1) throw TH_ERROR "Unknown comand |%s|", Word);

            To.push_back(Syntax::Comands[WordNum].Descriptor);
            *Size += 1;

            if (Syntax::Comands[WordNum].Params > 0)
            {
                for (int i = 0; i < Syntax::Comands[WordNum].Params; i ++)
                {
                    double n = 0;
                    if (!ReadWord (From, Word)) throw TH_ERROR "No number after %s!", Syntax::Comands[WordNum].Name);

                    if (!sscanf (Word, "%lg", &n)) throw TH_ERROR "%s - not a number!", Word);

                    To.push_back (n);
                    *Size += 1;
                }
            }
        }
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "%s", Error.ErrorText_);

        return false;
    }
    catch (...)
    {
        fprintf (ErrorOutput, "I don't know, what they said!");

        return false;
    }

    return true;
}

void WriteProgram (FILE* To, double Program[], int Size)
{
    for (int i = 0; i < Size; i ++)
    {
        fwrite (&Program [i], sizeof (double), 1, To);
    }
}

bool Assemble (vector<double> From, int FromSize, vector<double>& Program, int* Size, FILE* ErrorOutput)
{
    try
    {
        int MarkNumber = 0;

        vector<double> MarkNumbers;
        vector<int>    MarkPos;

        for (int i = 0; i < FromSize; i ++)
        {
            if (From [i] == 20)
            {
                i ++;

                for (int j = 0; j < MarkNumber; j ++)
                {
                    if (MarkNumbers[j] == From [i]) throw TH_ERROR "Can't set mark in two different places");
                }

                MarkNumbers.push_back (From [i]);
                MarkPos.    push_back (i - 2 * MarkNumber - 1 - 1 - 1);

                MarkNumber ++;

                continue;
            }

            #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (DESCRIPTOR == From[i]) {i += PARAMS; continue; }
            #include "..\Syntax\ComandDef.h"
            #undef COMAND_DEF
        }

        *Size = 0;
        for (int i = 0; i < FromSize; i ++)
        {
            if (From [i] == 20)
            {
                i ++;

                continue;
            }

            Program.push_back (From [i]);
            *Size += 1;

            if (From [i] == 21 || From [i] == 22 || From [i] == 34)
            {
                i ++;

                bool Done = false;
                for (int j = 0; j < MarkNumber; j ++)
                {
                    if (From [i] == MarkNumbers [j])
                    {
                        Program.push_back (MarkPos [j]);
                        Done = true;

                        break;
                    }
                }
                if (!Done) throw TH_ERROR "Can't find mark \"%lg\"", From [i]);

                *Size += 1;

                continue;
            }

            #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (DESCRIPTOR == From[i]) {for (int j = 0; j < PARAMS; j ++) {i ++; Program.push_back (From [i]); *Size += 1; } continue; }
            #include "..\Syntax\ComandDef.h"
            #undef COMAND_DEF
        }
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "%s", Error.ErrorText_);

        return false;
    }
    catch (...)
    {
        fprintf (ErrorOutput, "I don't know, what they said!");

        return false;
    }

    return true;
}

bool JITPrepare (vector<double>& Program, int* Size, FILE* ErrorOutput)
{
    try
    {
        int MarkNumber = 0;

        vector<double> MarkNumbers;

        for (int i = 0; i < *Size; i ++)
        {
            if (Program [i] == 20)
            {
                i ++;

                for (int j = 0; j < MarkNumber; j ++)
                {
                    if (MarkNumbers[j] == Program [i]) throw TH_ERROR "Can't set mark in two different places");
                }

                MarkNumbers.push_back (Program [i]);

                Program [i] = MarkNumber;

                MarkNumber ++;

                continue;
            }

            #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (DESCRIPTOR == Program[i]) {i += PARAMS; continue; }
            #include "..\Syntax\ComandDef.h"
            #undef COMAND_DEF
        }

        for (int i = 0; i < *Size; i ++)
        {
            if (Program [i] == 21 || Program [i] == 22 || Program [i] == 34)
            {
                i ++;

                bool Done = false;
                for (int j = 0; j < MarkNumber; j ++)
                {
                    if (Program [i] == MarkNumbers [j])
                    {
                        Program [i] = j;
                        Done = true;

                        break;
                    }
                }
                if (!Done) throw TH_ERROR "Can't find mark \"%lg\"", Program [i]);

                continue;
            }

            #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) if (DESCRIPTOR == Program[i]) {i += PARAMS; continue; }
            #include "..\Syntax\ComandDef.h"
            #undef COMAND_DEF
        }
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "%s", Error.ErrorText_);

        return false;
    }
    catch (...)
    {
        fprintf (ErrorOutput, "I don't know, what they said!");

        return false;
    }

    return true;
}

#endif
