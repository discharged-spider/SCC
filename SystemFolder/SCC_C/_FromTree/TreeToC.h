#ifndef TREETOC__
#define TREETOC__

#include "..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "..\..\_SystemLibs\Throw\Throw.h"

//==============================================================================

bool CreateC (newTree& Tree, FILE* To);

void PrintInd (FILE* To, int ind);

void CreateNode (newTree& Tree, FILE* To, int ind);

void CreateInstance (newTree& Tree, FILE* To);

bool New (newTree& Tree, FILE* To, int ind = 0, bool First = true);

void CreateArr (newTree& Tree, FILE* To, bool First = true);

void If (newTree& Tree, FILE* To, int ind);

void While (newTree& Tree, FILE* To, int ind);

void For (newTree& Tree, FILE* To, int ind);

bool NewFunc (newTree& Tree, FILE* To, int ind);
void CreateNewParams (newTree& Tree, FILE* To, bool First = true);

void CallFunc (newTree& Tree, FILE* To);
void CreateParams (newTree& Tree, FILE* To, bool First = true);

//==============================================================================

bool CreateC (newTree& Tree, FILE* To)
{
    if (!To) throw TH_ERROR "No output file");

    try
    {
        CreateNode (Tree, To, 0);
    }
    catch (newThrowError& Error)
    {
        printf ("\n%s", Error.ErrorText_);

        return false;
    }
    catch (const char Str [])
    {
        printf ("They say:%s\n", Str);

        return false;
    }
    catch (...)
    {
        printf ("I don't know, what they said!");

        return false;
    }

    return true;
}

void PrintInd (FILE* To, int ind)
{
    for (int i = 0; i < ind; i ++)
    {
        fprintf (To, " ");
    }
}

void CreateNode (newTree& Tree, FILE* To, int ind)
{
    if (!To) throw TH_ERROR "No output file");

    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NEW || Data.Descriptor == N_NATIVE)
    {
        PrintInd (To, ind);
        if (Data.Descriptor == N_NEW) fprintf (To, "new ");
        else                          fprintf (To, "native ");

        Tree.DownL ();
        bool EndSymbol = New (Tree, To, ind);
        Tree.Up ();

        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            EndSymbol = EndSymbol && New (Tree, To, ind, false);
            Tree.Up ();
        }

        if (EndSymbol == true) fprintf (To, ";\n");

        return ;
    }
    if (Data.Descriptor == N_DEL)
    {
        PrintInd (To, ind);

        fprintf (To, "del ");

        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();

        fprintf (To, ";\n");

        return ;
    }

    if (Data.Descriptor == N_INCR ||
        Data.Descriptor == N_DECR)
    {
        PrintInd (To, ind);

        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();
        fprintf (To, " ");

        if (Data.Descriptor == N_INCR) fprintf (To, "++");
        if (Data.Descriptor == N_DECR) fprintf (To, "--");

        fprintf (To, ";\n");
    }

    if (Data.Descriptor == N_EQ ||
        Data.Descriptor == N_SUM_EQ ||
        Data.Descriptor == N_SUB_EQ ||
        Data.Descriptor == N_MUL_EQ ||
        Data.Descriptor == N_DIV_EQ ||
        Data.Descriptor == N_MOD_EQ)
    {
        PrintInd (To, ind);

        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();

        fprintf (To, " ");

        if (Data.Descriptor == N_SUM_EQ) fprintf (To, "+");
        if (Data.Descriptor == N_SUB_EQ) fprintf (To, "-");
        if (Data.Descriptor == N_MUL_EQ) fprintf (To, "*");
        if (Data.Descriptor == N_DIV_EQ) fprintf (To, "/");
        if (Data.Descriptor == N_MOD_EQ) fprintf (To, "%%");

        fprintf (To, "= ");

        Tree.DownR ();
        CreateInstance (Tree, To);
        Tree.Up ();

        fprintf (To, ";\n");

        return ;
    }

    if (Data.Descriptor == N_ECHO)
    {
        PrintInd (To, ind);

        fprintf (To, "echo ");

        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();

        fprintf (To, ";\n");

        return ;
    }
    if (Data.Descriptor == N_GET)
    {
        PrintInd (To, ind);

        fprintf (To, "get ");

        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();

        fprintf (To, ";\n");

        return ;
    }

    if (Data.Descriptor == N_MARK)
    {
        PrintInd (To, ind);

        fprintf (To, ": %s;\n", Data.GetName ());

        return ;
    }
    if (Data.Descriptor == N_GOTO)
    {
        PrintInd (To, ind);

        fprintf (To, "goto %s;\n", Data.GetName ());

        return ;
    }

    if (Data.Descriptor == N_IF)
    {
        PrintInd (To, ind);

        If (Tree, To, ind);

        return ;
    }

    if (Data.Descriptor == N_WHILE)
    {
        PrintInd (To, ind);

        While (Tree, To, ind);

        return ;
    }
    if (Data.Descriptor == N_FOR)
    {
        PrintInd (To, ind);

        For (Tree, To, ind);

        return ;
    }

    if (Data.Descriptor == N_BREAK)
    {
        PrintInd (To, ind);

        fprintf (To, "break;\n");

        return ;
    }
    if (Data.Descriptor == N_CONTINUE)
    {
        PrintInd (To, ind);

        fprintf (To, "continue;\n");

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        PrintInd (To, ind);

        CallFunc (Tree, To);
        fprintf (To, ";\n");

        return ;
    }

    if (Data.Descriptor == N_RETURN)
    {
        PrintInd (To, ind);

        fprintf (To, "return ");
        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();
        fprintf (To, ";\n");

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateNode (Tree, To, ind);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        CreateNode (Tree, To, ind);
        Tree.Up ();
    }
}

void CreateInstance (newTree& Tree, FILE* To)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_TRUE)
    {
        fprintf (To, "true");

        return;
    }
    if (Data.Descriptor == N_FALSE)
    {
        fprintf (To, "false");

        return;
    }
    if (Data.Descriptor == N_NUM)
    {
        fprintf (To, "%g", Data.Data);

        return;
    }

    if (Data.Descriptor == N_VAR)
    {
        fprintf (To, "%s", Data.GetName ());

        return;
    }
    if (Data.Descriptor == N_LNK)
    {
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            fprintf (To, "&");
            CreateInstance (Tree, To);
            Tree.Up ();
        }
        else fprintf (To, "&%s", Data.GetName ());

        return;
    }
    if (Data.Descriptor == N_FUN)
    {
        CallFunc (Tree, To);

        return;
    }
    if (Data.Descriptor == N_ARR)
    {
        fprintf (To, "%s [", Data.GetName ());

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            CreateInstance (Tree, To);
            Tree.Up ();
        }

        fprintf (To, "]");

        return;
    }

    if (Data.Descriptor == N_NOT)
    {
        fprintf (To, "!");

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            CreateInstance (Tree, To);
            Tree.Up ();
        }

        return;
    }

    fprintf (To, "(");
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateInstance (Tree, To);
        Tree.Up ();
    }

    if (Data.Descriptor == N_SUM)
    {
        fprintf (To, " + ");
    }
    if (Data.Descriptor == N_SUB)
    {
        fprintf (To, " - ");
    }
    if (Data.Descriptor == N_MUL)
    {
        fprintf (To, " * ");
    }
    if (Data.Descriptor == N_DIV)
    {
        fprintf (To, " / ");
    }
    if (Data.Descriptor == N_MOD)
    {
        fprintf (To, " %% ");
    }

    if (Data.Descriptor == N_AND)
    {
        fprintf (To, " && ");
    }
    if (Data.Descriptor == N_OR)
    {
        fprintf (To, " || ");
    }

    if (Data.Descriptor == N_OPEQ)
    {
        fprintf (To, " == ");
    }
    if (Data.Descriptor == N_NONEQ)
    {
        fprintf (To, " != ");
    }
    if (Data.Descriptor == N_MORE)
    {
        fprintf (To, " > ");
    }
    if (Data.Descriptor == N_LESS)
    {
        fprintf (To, " < ");
    }
    if (Data.Descriptor == N_MREQ)
    {
        fprintf (To, " >= ");
    }
    if (Data.Descriptor == N_LSEQ)
    {
        fprintf (To, " <= ");
    }

    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        CreateInstance (Tree, To);
        Tree.Up ();
    }
    fprintf (To, ")");
}

bool New (newTree& Tree, FILE* To, int ind, bool First)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR || Data.Descriptor == N_LNK)
    {
        if (!First) fprintf (To, ", ");

        CreateInstance (Tree, To);

        if (Tree.CanDownR ())
        {
            fprintf (To, " = ");

            Tree.DownR ();

            CreateInstance (Tree, To);

            Tree.Up ();
        }

        return true;
    }
    if (Data.Descriptor == N_ARR)
    {
        if (!First) fprintf (To, ", ");

        CreateInstance (Tree, To);

        if (Tree.CanDownR ())
        {
            fprintf (To, " = {");

            Tree.DownR ();
            Tree.DownR ();

            CreateArr (Tree, To);

            Tree.Up ();
            Tree.Up ();

            fprintf (To, "}");
        }

        return true;
    }

    if (Data.Descriptor == N_FUN)
    {
        return NewFunc (Tree, To, ind);
    }

    bool Return = true;
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        Return = Return && New (Tree, To, ind, First);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        Return = Return && New (Tree, To, ind, false);
        Tree.Up ();
    }

    return Return;
}

void CreateArr (newTree& Tree, FILE* To, bool First)
{
    if (Tree.Get ().Descriptor != N_NODE)
    {
        if (!First) fprintf (To, ", ");

        CreateInstance (Tree, To);

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateArr (Tree, To, First);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        CreateArr (Tree, To, false);
        Tree.Up ();
    }
}

void If (newTree& Tree, FILE* To, int ind)
{
    fprintf (To, "if (");
    Tree.DownL ();
    CreateInstance (Tree, To);
    Tree.Up ();
    fprintf (To, ")\n");

    PrintInd (To, ind);
    fprintf (To, "{\n");

    Tree.DownR ();

    if (Tree.Get ().Descriptor == N_ELSE)
    {
        Tree.DownL ();
        CreateNode (Tree, To, ind + 4);
        Tree.Up ();

        PrintInd (To, ind);
        fprintf (To, "}\n");

        PrintInd (To, ind);
        fprintf (To, "else\n");
        PrintInd (To, ind);
        fprintf (To, "{\n");

        Tree.DownR ();
        CreateNode (Tree, To, ind + 4);
        Tree.Up ();
    }
    else
    {
        CreateNode (Tree, To, ind + 4);
    }

    Tree.Up ();

    PrintInd (To, ind);
    fprintf (To, "}\n");
}

void While (newTree& Tree, FILE* To, int ind)
{
    fprintf (To, "while (");
    Tree.DownL ();
    CreateInstance (Tree, To);
    Tree.Up ();
    fprintf (To, ")\n");

    PrintInd (To, ind);
    fprintf (To, "{\n");

    Tree.DownR ();
    CreateNode (Tree, To, ind + 4);
    Tree.Up ();

    PrintInd (To, ind);
    fprintf (To, "}\n");
}

void For (newTree& Tree, FILE* To, int ind)
{
    fprintf (To, "for (new ");
    Tree.DownL ();
    Tree.DownL ();
    New (Tree, To);
    Tree.Up ();
    Tree.Up ();

    fprintf (To, "; ");

    Tree.DownR ();
    Tree.DownL ();
    CreateInstance (Tree, To);
    Tree.Up ();
    Tree.Up ();

    fprintf (To, "; ");

    Tree.DownL ();
    Tree.DownR ();
    CreateNode (Tree, To, 0);
    Tree.Up ();
    Tree.Up ();

    fseek (To, -2, SEEK_CUR);

    fprintf (To, ")\n");

    PrintInd (To, ind);
    fprintf (To, "{\n");

    Tree.DownR ();
    Tree.DownR ();
    CreateNode (Tree, To, ind + 4);
    Tree.Up ();
    Tree.Up ();

    PrintInd (To, ind);
    fprintf (To, "}\n");
}

bool NewFunc (newTree& Tree, FILE* To, int ind)
{
    fprintf (To, "%s (", Tree.Get ().GetName ());
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateNewParams (Tree, To);
        Tree.Up ();
    }
    fprintf (To, ")");

    if (Tree.CanDownR ())
    {
        fprintf (To, "\n");

        PrintInd (To, ind);
        fprintf (To, "{\n");

        Tree.DownR ();
        CreateNode (Tree, To, ind + 4);
        Tree.Up ();

        PrintInd (To, ind);
        fprintf (To, "}\n");

        return false;
    }
    else return true;
}

void CreateNewParams (newTree& Tree, FILE* To, bool First)
{
    if (Tree.Get ().Descriptor != N_NODE)
    {
        if (!First) fprintf (To, ", ");

        fprintf (To, "new ");

        Tree.DownL ();
        New (Tree, To);
        Tree.Up ();

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateNewParams (Tree, To, First);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        CreateNewParams (Tree, To, false);
        Tree.Up ();
    }
}

void CallFunc (newTree& Tree, FILE* To)
{
    fprintf (To, "%s (", Tree.Get ().GetName ());
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateParams (Tree, To);
        Tree.Up ();
    }
    fprintf (To, ")");

}

void CreateParams (newTree& Tree, FILE* To, bool First)
{
    if (Tree.Get ().Descriptor != N_NODE)
    {
        if (!First) fprintf (To, ", ");

        CreateInstance (Tree, To);

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        CreateParams (Tree, To, First);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        CreateParams (Tree, To, false);
        Tree.Up ();
    }
}

#endif
