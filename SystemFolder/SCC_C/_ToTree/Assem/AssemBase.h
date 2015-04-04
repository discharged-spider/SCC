#ifndef ASSEM_BASE__
#define ASSEM_BASE__

#include "..\..\..\_SystemLibs\Stack\Stack.h"
#include "..\..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "string"
#include "..\..\..\_SystemLibs\Throw\Throw.h"

using std::string;

//==============================================================================

const int  ExeptSymbolsSize = 5;
const char ExeptSymbols [ExeptSymbolsSize] = {' ', '\t', '\n', '\r', '\0'};

bool Exept (char Symbol);

//==============================================================================

const int  SpecialSymbolsSize = 8;
const char SpecialSymbols [SpecialSymbolsSize] = {'(', ')', '{', '}', '[', ']', ';', ','};

bool Special (char Symbol);

//==============================================================================

const int  OperatorsSize = 11;
const char Operators [OperatorsSize] = {'+', '-', '/', '*', '%', '=', '!', '>', '<', ':', '&'};

bool Operator (char Symbol);

//==============================================================================

bool ReadWord (FILE* file, string& Word, int* line = 0, int* spaces = 0);

//==============================================================================

bool TryToNum (string& Word, newNodeData* Data);

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size, bool TryToKey (string& Word, newNodeData* Data));

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

bool Special (char Symbol)
{
    for (int i = 0; i < SpecialSymbolsSize; i ++)
    {
        if (Symbol == SpecialSymbols [i]) return true;
    }

    return false;
}

//==============================================================================

bool Operator (char Symbol)
{
    for (int i = 0; i < OperatorsSize; i ++)
    {
        if (Symbol == Operators [i]) return true;
    }

    return false;
}

//==============================================================================

bool ReadWord (FILE* file, string& Word, int* line, int* spaces)
{
    Word.clear ();

    bool Start = false;
    char c     = 0;
    if (fread (&c, sizeof (c), 1, file) != 1) return 0;

    bool seek = false;

    while (true)
    {
        if (!Start)
        {
            if (c == '\n' && line != NULL)
            {
                (*line) = *line + 1;

                if (spaces != NULL) (*spaces) = 0;
            }
            if (c == ' ' && spaces != NULL)
            {
                (*spaces) = *spaces + 1;
            }
        }

        if (Special (c))
        {
            if (Word.size () == 0)
            {
                Word.push_back (c);
            }
            else
            {
                seek = true;
            }

            break;
        }

        if (Operator (c))
        {
            if (Word.size () == 0)
            {
                while (Operator (c))
                {
                    Word.push_back (c);

                    if (!fread (&c, sizeof (c), 1, file)) break;
                }
            }

            seek = true;

            break;
        }

        if (Exept (c))
        {
            if (Start)
            {
                seek = true;

                break;
            }
        }
        else
        {
            Start = true;

            Word.push_back (c);
        }

        if (!fread (&c, sizeof (c), 1, file)) break;
    }

    if (seek) fseek (file, -1, SEEK_CUR);

    return Word.size () > 0;
}

//==============================================================================

bool TryToNum (string& Word, newNodeData* Data)
{
    (*Data).Data = 0;

    int i = 0;

    bool Invert = false;
    if (Word [0] == '-')
    {
        Invert = true;

        i = 1;
    }

    for (; Word [i] != 0; i ++)
    {
        if (Word [i] >= '0' && Word [i] <= '9')
        {
            (*Data).Data *= 10;
            (*Data).Data += Word [i] - '0';
        }
        else if (Word [i] == '.' || Word [i] == ',')
        {
            i ++;
            for (int j = 10; Word [i] != 0; j *= 10, i ++)
            {
                if (Word [i] < '0' || Word [i] > '9') return false;

                (*Data).Data += (double)(Word [i] - '0') / j;
            }

            break;
        }
        else return false;
    }

    if (Invert) (*Data).Data *= -1;

    (*Data).Descriptor = N_NUM;

    return true;
}

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size, bool TryToKey (string& Word, newNodeData* Data))
{
    string Word;
    *Size = 0;

    string currentFile;
    int line = 1;
    int spaces = 0;

    while (true)
    {
        if (!ReadWord (file, Word, &line, &spaces)) break;

        newNodeData Data;

        if (TryToKey (Word, &Data))
        {
            Tokens [*Size] = Data;

            *Size += 1;

            continue;
        }
        if (TryToNum (Word, &Data))
        {
            Tokens [*Size] = Data;

            *Size += 1;

            continue;
        }
        if (Word == "#")
        {
            fscanf (file, "%d", &line);
            ReadWord (file, currentFile);

            continue;
        }

        Tokens [*Size].Descriptor = N_NAME;
        Tokens [*Size].SetName (Word.c_str ());

        *Size += 1;
    }
}

#endif
