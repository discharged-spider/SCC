#ifndef ASSEM__
#define ASSEM__

#include "..\..\..\_SystemLibs\Stack\Stack.h"
#include "..\..\..\SCC_Tree\Lib\Tree.h"
#include "AssemDesc.h"
#include "..\..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "..\..\..\_SystemLibs\Throw\Throw.h"

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

bool ReadWord (FILE* file, char Word []);

//==============================================================================

bool TryToKey (char Word [], newNodeData* Data);

bool TryToNum (char Word [], newNodeData* Data);

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

bool ReadWord (FILE* file, char Word [])
{
    bool Start = false;
    char c     = 0;
    if (fread (&c, sizeof (c), 1, file) != 1) return false;

    int i = 0;
    for (; ; i ++)
    {
        if (i >= 30) return false;

        if (Special (c))
        {
            if (i == 0)
            {
                Word [i] = c;

                i ++;

                Word [i] = 0;
            }
            else
            {
                fseek (file, -1, SEEK_CUR);

                Word [i] = 0;
            }

            break;
        }

        if (Operator (c))
        {
            if (i == 0)
            {
                while (Operator (c))
                {
                    Word [i] = c;

                    i ++;

                    if (fread (&c, sizeof (c), 1, file) != 1) break;
                }

                fseek (file, -1, SEEK_CUR);

                Word [i] = 0;

                break;
            }
            else
            {
                fseek (file, -1, SEEK_CUR);

                Word [i] = 0;
            }

            break;
        }

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

bool TryToKey (char Word [], newNodeData* Data)
{
    if (strcmp (Word, "(") == 0)
    {
        (*Data).Descriptor = N_OB;

        return true;
    }
    if (strcmp (Word, ")") == 0)
    {
        (*Data).Descriptor = N_CB;

        return true;
    }
     if (strcmp (Word, "{") == 0)
    {
        (*Data).Descriptor = N_OF;

        return true;
    }
    if (strcmp (Word, "}") == 0)
    {
        (*Data).Descriptor = N_CF;

        return true;
    }
    if (strcmp (Word, "[") == 0)
    {
        (*Data).Descriptor = N_OS;

        return true;
    }
    if (strcmp (Word, "]") == 0)
    {
        (*Data).Descriptor = N_CS;

        return true;
    }

    if (strcmp (Word, "new") == 0)
    {
        (*Data).Descriptor = N_NEW;

        return true;
    }
    if (strcmp (Word, "native") == 0)
    {
        (*Data).Descriptor = N_NATIVE;

        return true;
    }
    if (strcmp (Word, "del") == 0)
    {
        (*Data).Descriptor = N_DEL;

        return true;
    }
    if (strcmp (Word, "=") == 0)
    {
        (*Data).Descriptor = N_EQ;

        return true;
    }
    if (strcmp (Word, "&") == 0)
    {
        (*Data).Descriptor = N_LNK;

        return true;
    }
    if (strcmp (Word, ";") == 0)
    {
        (*Data).Descriptor = N_END;

        return true;
    }

    if (strcmp (Word, "+") == 0)
    {
        (*Data).Descriptor = N_SUM;

        return true;
    }
    if (strcmp (Word, "-") == 0)
    {
        (*Data).Descriptor = N_SUB;

        return true;
    }
    if (strcmp (Word, "*") == 0)
    {
        (*Data).Descriptor = N_MUL;

        return true;
    }
    if (strcmp (Word, "/") == 0)
    {
        (*Data).Descriptor = N_DIV;

        return true;
    }
    if (strcmp (Word, "%") == 0)
    {
        (*Data).Descriptor = N_MOD;

        return true;
    }

    if (strcmp (Word, "true") == 0)
    {
        (*Data).Descriptor = N_TRUE;

        return true;
    }
    if (strcmp (Word, "false") == 0)
    {
        (*Data).Descriptor = N_FALSE;

        return true;
    }

    if (strcmp (Word, "&&") == 0)
    {
        (*Data).Descriptor = N_AND;

        return true;
    }
    if (strcmp (Word, "||") == 0)
    {
        (*Data).Descriptor = N_OR;

        return true;
    }

    if (strcmp (Word, "==") == 0)
    {
        (*Data).Descriptor = N_OPEQ;

        return true;
    }
    if (strcmp (Word, "!=") == 0)
    {
        (*Data).Descriptor = N_NONEQ;

        return true;
    }
    if (strcmp (Word, "!") == 0)
    {
        (*Data).Descriptor = N_NOT;

        return true;
    }
    if (strcmp (Word, ">") == 0)
    {
        (*Data).Descriptor = N_MORE;

        return true;
    }
    if (strcmp (Word, "<") == 0)
    {
        (*Data).Descriptor = N_LESS;

        return true;
    }
    if (strcmp (Word, ">=") == 0)
    {
        (*Data).Descriptor = N_MREQ;

        return true;
    }
    if (strcmp (Word, "<=") == 0)
    {
        (*Data).Descriptor = N_LSEQ;

        return true;
    }

    if (strcmp (Word, "if") == 0)
    {
        (*Data).Descriptor = N_IF;

        return true;
    }
    if (strcmp (Word, "else") == 0)
    {
        (*Data).Descriptor = N_ELSE;

        return true;
    }

    if (strcmp (Word, "while") == 0)
    {
        (*Data).Descriptor = N_WHILE;

        return true;
    }

    if (strcmp (Word, "for") == 0)
    {
        (*Data).Descriptor = N_FOR;

        return true;
    }

    if (strcmp (Word, "break") == 0)
    {
        (*Data).Descriptor = N_BREAK;

        return true;
    }

    if (strcmp (Word, "continue") == 0)
    {
        (*Data).Descriptor = N_CONTINUE;

        return true;
    }

    if (strcmp (Word, "return") == 0)
    {
        (*Data).Descriptor = N_RETURN;

        return true;
    }

    if (strcmp (Word, "echo") == 0)
    {
        (*Data).Descriptor = N_ECHO;

        return true;
    }
    if (strcmp (Word, "get") == 0)
    {
        (*Data).Descriptor = N_GET;

        return true;
    }

    if (strcmp (Word, ",") == 0)
    {
        (*Data).Descriptor = N_LIST;

        return true;
    }

    if (strcmp (Word, ":") == 0)
    {
        (*Data).Descriptor = N_MARK;

        return true;
    }

    if (strcmp (Word, "goto") == 0)
    {
        (*Data).Descriptor = N_GOTO;

        return true;
    }

    return false;
}

bool TryToNum (char Word [], newNodeData* Data)
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
    char Word [30] = "";
    *Size = 0;

    while (true)
    {
        if (!ReadWord (file, Word)) break;

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

        Tokens [*Size].Descriptor = N_NAME;
        strcpy (Tokens [*Size].Name, Word);

        *Size += 1;
    }
}

#endif
