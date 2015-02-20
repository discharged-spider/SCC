#ifndef ALLDESC
#define ALLDESC

#include "..\..\SCC_TREE\Lib\BaseTreeDesc.h"
#include "Assem\AssemDesc.h"

//==============================================================================

void GetDesc (FILE* file, newNodeData Data);

//==============================================================================

void GetDesc (FILE* file, newNodeData Data)
{
    if (Data.Descriptor == N_NUM)
    {
        fprintf (file, "%g", Data.Data);
    }
    if (Data.Descriptor == N_VAR)
    {
        fprintf (file, "%s", Data.GetName ());
    }
    if (Data.Descriptor == N_ARR)
    {
        fprintf (file, "%s", Data.GetName ());
    }
    if (Data.Descriptor == N_FUN)
    {
        fprintf (file, "%s", Data.GetName ());
    }

    if (Data.Descriptor == N_NEW)
    {
        fprintf (file, "new");
    }
    if (Data.Descriptor == N_DEL)
    {
        fprintf (file, "del");
    }

    if (Data.Descriptor == N_EQ)
    {
        fprintf (file, "=");
    }
    if (Data.Descriptor == N_INCR)
    {
        fprintf (file, "++");
    }
    if (Data.Descriptor == N_DECR)
    {
        fprintf (file, "--");
    }
    if (Data.Descriptor == N_SUM_EQ)
    {
        fprintf (file, "+=");
    }
    if (Data.Descriptor == N_SUB_EQ)
    {
        fprintf (file, "-=");
    }
    if (Data.Descriptor == N_MUL_EQ)
    {
        fprintf (file, "*=");
    }
    if (Data.Descriptor == N_DIV_EQ)
    {
        fprintf (file, "/=");
    }
    if (Data.Descriptor == N_MOD_EQ)
    {
        fprintf (file, "%%=");
    }

    if (Data.Descriptor == N_SUM)
    {
        fprintf (file, "+");
    }
    if (Data.Descriptor == N_SUB)
    {
        fprintf (file, "-");
    }
    if (Data.Descriptor == N_MUL)
    {
        fprintf (file, "*");
    }
    if (Data.Descriptor == N_DIV)
    {
        fprintf (file, "/");
    }
    if (Data.Descriptor == N_MOD)
    {
        fprintf (file, "%%");
    }

    if (Data.Descriptor == N_TRUE)
    {
        fprintf (file, "true");
    }
    if (Data.Descriptor == N_FALSE)
    {
        fprintf (file, "false");
    }

    if (Data.Descriptor == N_AND)
    {
        fprintf (file, "&&");
    }
    if (Data.Descriptor == N_OR)
    {
        fprintf (file, "||");
    }

    if (Data.Descriptor == N_NOT)
    {
        fprintf (file, "!");
    }
    if (Data.Descriptor == N_OPEQ)
    {
        fprintf (file, "==");
    }
    if (Data.Descriptor == N_NONEQ)
    {
        fprintf (file, "!=");
    }
    if (Data.Descriptor == N_MORE)
    {
        fprintf (file, ">");
    }
    if (Data.Descriptor == N_LESS)
    {
        fprintf (file, "<");
    }

    if (Data.Descriptor == N_IF)
    {
        fprintf (file, "if");
    }
    if (Data.Descriptor == N_ELSE)
    {
        fprintf (file, "else");
    }

    if (Data.Descriptor == N_WHILE)
    {
        fprintf (file, "while");
    }
    if (Data.Descriptor == N_FOR)
    {
        fprintf (file, "for");
    }
    if (Data.Descriptor == N_BREAK)
    {
        fprintf (file, "break");
    }
    if (Data.Descriptor == N_CONTINUE)
    {
        fprintf (file, "continue");
    }

    if (Data.Descriptor == N_RETURN)
    {
        fprintf (file, "return");
    }

    if (Data.Descriptor == N_ECHO)
    {
        fprintf (file, "echo");
    }
    if (Data.Descriptor == N_GET)
    {
        fprintf (file, "get");
    }

    if (Data.Descriptor == N_MARK)
    {
        fprintf (file, ":");
    }
    if (Data.Descriptor == N_GOTO)
    {
        fprintf (file, "goto");
    }

    //--------------------------------------------------------------------------

    if (Data.Descriptor == N_OB)
    {
        fprintf (file, "(");
    }
    if (Data.Descriptor == N_CB)
    {
        fprintf (file, ")");
    }

    if (Data.Descriptor == N_OF)
    {
        fprintf (file, "{\n");
    }
    if (Data.Descriptor == N_CF)
    {
        fprintf (file, "}\n");
    }

    if (Data.Descriptor == N_OS)
    {
        fprintf (file, "[");
    }
    if (Data.Descriptor == N_CS)
    {
        fprintf (file, "]");
    }

    if (Data.Descriptor == N_END)
    {
        fprintf (file, ";\n");
    }

    if (Data.Descriptor == N_NAME)
    {
        fprintf (file, "%s(N_NAME)", Data.GetName ());
    }

    if (Data.Descriptor == N_LIST)
    {
        fprintf (file, ",");
    }
}

#endif
