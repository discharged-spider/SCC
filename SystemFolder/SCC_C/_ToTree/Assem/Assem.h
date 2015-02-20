#ifndef ASSEM__
#define ASSEM__

#include "..\..\..\_SystemLibs\Stack\Stack.h"
#include "..\..\..\SCC_Tree\Lib\Tree.h"
#include "AssemDesc.h"
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

bool TryToKey (string& Word, newNodeData* Data);

bool TryToNum (string& Word, newNodeData* Data);

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size);

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

bool TryToKey (string& Word, newNodeData* Data)
{
    if (Word == "(")
    {
        (*Data).Descriptor = N_OB;

        return true;
    }
    if (Word == ")")
    {
        (*Data).Descriptor = N_CB;

        return true;
    }
     if (Word == "{")
    {
        (*Data).Descriptor = N_OF;

        return true;
    }
    if (Word == "}")
    {
        (*Data).Descriptor = N_CF;

        return true;
    }
    if (Word == "[")
    {
        (*Data).Descriptor = N_OS;

        return true;
    }
    if (Word == "]")
    {
        (*Data).Descriptor = N_CS;

        return true;
    }

    if (Word == "new")
    {
        (*Data).Descriptor = N_NEW;

        return true;
    }
    if (Word == "native")
    {
        (*Data).Descriptor = N_NATIVE;

        return true;
    }
    if (Word == "del")
    {
        (*Data).Descriptor = N_DEL;

        return true;
    }
    if (Word == "=")
    {
        (*Data).Descriptor = N_EQ;

        return true;
    }
    if (Word == "++")
    {
        (*Data).Descriptor = N_INCR;

        return true;
    }
    if (Word == "--")
    {
        (*Data).Descriptor = N_DECR;

        return true;
    }
    if (Word == "+=")
    {
        (*Data).Descriptor = N_SUM_EQ;

        return true;
    }
    if (Word == "-=")
    {
        (*Data).Descriptor = N_SUB_EQ;

        return true;
    }
    if (Word == "*=")
    {
        (*Data).Descriptor = N_MUL_EQ;

        return true;
    }
    if (Word == "/=")
    {
        (*Data).Descriptor = N_DIV_EQ;

        return true;
    }
    if (Word == "%=")
    {
        (*Data).Descriptor = N_MOD_EQ;

        return true;
    }
    if (Word == "&")
    {
        (*Data).Descriptor = N_LNK;

        return true;
    }
    if (Word == ";")
    {
        (*Data).Descriptor = N_END;

        return true;
    }

    if (Word == "+")
    {
        (*Data).Descriptor = N_SUM;

        return true;
    }
    if (Word == "-")
    {
        (*Data).Descriptor = N_SUB;

        return true;
    }
    if (Word == "*")
    {
        (*Data).Descriptor = N_MUL;

        return true;
    }
    if (Word == "/")
    {
        (*Data).Descriptor = N_DIV;

        return true;
    }
    if (Word == "%")
    {
        (*Data).Descriptor = N_MOD;

        return true;
    }

    if (Word == "true")
    {
        (*Data).Descriptor = N_TRUE;

        return true;
    }
    if (Word == "false")
    {
        (*Data).Descriptor = N_FALSE;

        return true;
    }

    if (Word == "&&")
    {
        (*Data).Descriptor = N_AND;

        return true;
    }
    if (Word == "||")
    {
        (*Data).Descriptor = N_OR;

        return true;
    }

    if (Word == "==")
    {
        (*Data).Descriptor = N_OPEQ;

        return true;
    }
    if (Word == "!=")
    {
        (*Data).Descriptor = N_NONEQ;

        return true;
    }
    if (Word == "!")
    {
        (*Data).Descriptor = N_NOT;

        return true;
    }
    if (Word == ">")
    {
        (*Data).Descriptor = N_MORE;

        return true;
    }
    if (Word == "<")
    {
        (*Data).Descriptor = N_LESS;

        return true;
    }
    if (Word == ">=")
    {
        (*Data).Descriptor = N_MREQ;

        return true;
    }
    if (Word == "<=")
    {
        (*Data).Descriptor = N_LSEQ;

        return true;
    }

    if (Word == "if")
    {
        (*Data).Descriptor = N_IF;

        return true;
    }
    if (Word == "else")
    {
        (*Data).Descriptor = N_ELSE;

        return true;
    }

    if (Word == "while")
    {
        (*Data).Descriptor = N_WHILE;

        return true;
    }

    if (Word == "for")
    {
        (*Data).Descriptor = N_FOR;

        return true;
    }

    if (Word == "break")
    {
        (*Data).Descriptor = N_BREAK;

        return true;
    }

    if (Word == "continue")
    {
        (*Data).Descriptor = N_CONTINUE;

        return true;
    }

    if (Word == "return")
    {
        (*Data).Descriptor = N_RETURN;

        return true;
    }

    if (Word == "echo")
    {
        (*Data).Descriptor = N_ECHO;

        return true;
    }
    if (Word == "get")
    {
        (*Data).Descriptor = N_GET;

        return true;
    }

    if (Word == ",")
    {
        (*Data).Descriptor = N_LIST;

        return true;
    }

    if (Word == ":")
    {
        (*Data).Descriptor = N_MARK;

        return true;
    }

    if (Word == "goto")
    {
        (*Data).Descriptor = N_GOTO;

        return true;
    }

    return false;
}

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

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size)
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
