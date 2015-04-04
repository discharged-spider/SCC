#ifndef ASSEM__
#define ASSEM__

#include "AssemBase.h"

#include "AssemDesc.h"

using std::string;

//==============================================================================

bool TryToKey (string& Word, newNodeData* Data);

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size);

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

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size)
{
    LexemProgram (file, Tokens, Size, &TryToKey);
}

#endif
