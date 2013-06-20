#ifndef RECURSIA__
#define RECURSIA__

#include "..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\SCC_Tree\Lib\TreeOperations.h"
#include "..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "Assem\AssemDesc.h"
#include "AllDesc.h"
#include "..\..\_SystemLibs\Throw\Throw.h"

//==============================================================================

bool CreateTree (newTree& Tree, newNodeData Program [], int Size, FILE* ErrorOutput);

//==============================================================================

void NullTreeNode (newTree& Tree);

//==============================================================================

/*
Recursia realisation of file Syntax.cpp
*/

void GetMain (newTree& Tree, newNodeData Program [], int& Size, int& i);

void GetAA (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetAB (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetAC (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetAD (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetAE (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetAF (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetAG (newTree& Tree, newNodeData Program [], int& Size, int& i);

bool InFunc = false, InCycle = false;
void GetC (newTree& Tree, newNodeData Program [], int& Size, int& i);

void GetBA (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetBB (newTree& Tree, newNodeData Program [], int& Size, int& i);

void GetCA (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCB (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCC (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCD (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCE (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCF (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCG (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCH (newTree& Tree, newNodeData Program [], int& Size, int& i);

void GetArr (newTree& Tree, newNodeData Program [], int& Size, int& i);

//==============================================================================

bool CreateTree (newTree& Tree, newNodeData Program [], int Size, FILE* ErrorOutput)
{
    int i = 0;
    int S = Size;

    if (Size == 0) return true;

    try
    {
        GetMain (Tree, Program, S, i);
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "%s", Error.ErrorText_);

        for (int j = 0; j < Size; j ++)
        {
            if (i == j) fprintf (ErrorOutput, "|Error|-> ");
            GetDesc (ErrorOutput, Program [j]);
            fprintf (ErrorOutput, " ");
        }

        if (i >= Size) fprintf (ErrorOutput, "|Error|-> [END OF FILE]");

        return false;
    }
    catch (...)
    {
        fprintf (ErrorOutput, "I don't know, what they said!");

        return false;
    }

    #ifdef DEBUG
        printf ("Cutting\n");
    #endif

    RemoveFreeNodes (Tree);

    #ifdef DEBUG
        printf ("Done\n");
    #endif

    return true;
}

//==============================================================================

void NullTreeNode (newTree& Tree)
{
    Tree.Set (newNodeData (N_NODE));

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        Tree.Delete (true);
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        Tree.Delete (true);
    }
}

//==============================================================================

// GetMain i < size GetC;
void GetMain (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetMain, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.PushP ();

    NullTreeNode (Tree);

    Tree.Set (newNodeData (N_NODE));

    Tree.DownL ();
    GetC (Tree, Program, Size, i);

    if (Program [i - 1].Descriptor != N_CF)
    {
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
        else i ++;
    }
    else
    {
        if (Program [i].Descriptor == N_END) i ++;
    }

    while (i < Size)
    {
        Tree.Up ();
        Tree.DownR ();

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        GetC (Tree, Program, Size, i);

        if (Program [i - 1].Descriptor != N_CF)
        {
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
            else i ++;
        }
        else
        {
            if (Program [i].Descriptor == N_END) i ++;
        }
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//******************************************************************************

//GetAA GetAB '||' GetAB
void GetAA (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAA, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.PushP ();

    NullTreeNode (Tree);

    Tree.DownL ();
    GetAB (Tree, Program, Size, i);

    while (Program [i].Descriptor == N_OR)
    {
        Tree.Up ();
        Tree.Set (Program[i]);

        i ++;

        Tree.DownR ();
        Tree.DownL ();
        GetAB (Tree, Program, Size, i);
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAB GetAC && GetAC
void GetAB (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAB, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.PushP ();

    NullTreeNode (Tree);

    Tree.DownL ();
    GetAC (Tree, Program, Size, i);

    while (Program [i].Descriptor == N_AND)
    {
        Tree.Up ();
        Tree.Set (Program[i]);

        i ++;

        Tree.DownR ();
        Tree.DownL ();
        GetAC (Tree, Program, Size, i);
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();


    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAC !GetAA || GetAD || GetAA == GetAA || GetAA != GetAA || GetAA < GetAA || GetAA > GetAA || GetAA <= GetAA || GetAA >= GetAA
void GetAC (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAC, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_NOT)
    {
        Tree.Set (Program [i]);

        i ++;

        Tree.DownL ();

        GetAA (Tree, Program, Size, i);

        Tree.Up ();

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    Tree.DownL ();

    GetAD (Tree, Program, Size, i);

    Tree.Up ();

    if (Program [i].Descriptor == N_OPEQ || Program [i].Descriptor == N_NONEQ || Program [i].Descriptor == N_MORE || Program [i].Descriptor == N_LESS || Program [i].Descriptor == N_MREQ || Program [i].Descriptor == N_LSEQ)
    {
        Tree.Set (Program[i]);

        i ++;

        Tree.DownR ();

        GetAD (Tree, Program, Size, i);

        Tree.Up ();
    }
    else
    {
        Tree.DownL ();

        UpBranch (Tree);
    }

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAD GetAE + GetAE || GetAE - GetAE
void GetAD (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAD, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.PushP ();

    NullTreeNode (Tree);

    Tree.DownL ();
    GetAE (Tree, Program, Size, i);

    while (Program [i].Descriptor == N_SUM || Program [i].Descriptor == N_SUB)
    {
        Tree.Up ();
        Tree.Set (Program[i]);

        i ++;

        Tree.DownR ();
        Tree.DownL ();
        GetAE (Tree, Program, Size, i);
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAE GetAF * GetAF || GetAF / GetAF || GetAF % GetAF
void GetAE (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAE, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.PushP ();

    NullTreeNode (Tree);

    Tree.DownL ();
    GetAF (Tree, Program, Size, i);

    while (Program [i].Descriptor == N_MUL || Program [i].Descriptor == N_DIV || Program [i].Descriptor == N_MOD)
    {
        Tree.Up ();
        Tree.Set (Program[i]);

        i ++;

        Tree.DownR ();
        Tree.DownL ();
        GetAF (Tree, Program, Size, i);
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAF [0-9] || GetCF || GetAF || (GetAA) || - GetAF || + GetAF || true || false || GetAG
void GetAF (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAF, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_NUM)
    {
        Tree.Set (Program [i]);

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_SUB)
    {
        i ++;

        Tree.Set (newNodeData (N_MUL));

        Tree.DownL ();
        GetAF (Tree, Program, Size, i);
        Tree.Up ();

        Tree.DownR ();
        Tree.Set (newNodeData (N_NUM, -1));
        Tree.Up ();

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }
    if (Program [i].Descriptor == N_SUM)
    {
        i ++;

        GetAF (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (i + 1 < Size && Program [i].Descriptor == N_NAME && Program [i + 1].Descriptor == N_OB)
    {
        GetCF (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_OB)
    {
        i ++;

        GetAA (Tree, Program, Size, i);

        if (Program [i].Descriptor != N_CB) throw TH_ERROR "Expected |)|.");

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_TRUE || Program [i].Descriptor == N_FALSE)
    {
        Tree.Set (Program [i]);

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    GetAG (Tree, Program, Size, i);

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAG Name || Name[GetAA]
void GetAG (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAD, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (i + 1 < Size && Program [i].Descriptor == N_NAME && Program [i + 1].Descriptor == N_OS)
    {
        Program [i].Descriptor = N_ARR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i += 2;

        if (i >= Size) throw TH_ERROR "SUDDEN END");

        Tree.DownL ();
        GetAA (Tree, Program, Size, i);
        Tree.Up ();

        if (i >= Size) throw TH_ERROR "SUDDEN END");
        if (Program [i].Descriptor != N_CS) throw TH_ERROR "Expected |]|.");

        i ++;

        #ifdef DEBUG
           printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_NAME)
    {
        Program [i].Descriptor = N_VAR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i ++;

        #ifdef DEBUG
           printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    throw TH_ERROR "Expected |Name|.");
}

//******************************************************************************
//GetC {GetC, ...} || GetBA || GetCA || GetCB || GetCC || GetCD || GetCE || GetCF || GetCG || GetCH
void GetC (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetC, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_OF)
    {
        i ++;

        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor == N_CF)
        {
            i ++;

            #ifdef DEBUG
                printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
            #endif

            return;
        }

        Tree.PushP ();

        NullTreeNode (Tree);

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();
        GetC (Tree, Program, Size, i);

        if (Program [i - 1].Descriptor != N_CF)
        {
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
            else i ++;
        }
        else
        {
            if (Program [i].Descriptor == N_END) i ++;
        }

        while (Program [i].Descriptor != N_CF)
        {
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            Tree.Up ();
            Tree.DownR ();

            Tree.Set (newNodeData (N_NODE));

            Tree.DownL ();

            GetC (Tree, Program, Size, i);

            if (Program [i - 1].Descriptor != N_CF)
            {
                if (i >= Size) throw TH_ERROR "SUDDEN END");

                if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
                else i ++;
            }
            else
            {
                if (Program [i].Descriptor == N_END) i ++;
            }
        }

        if (Tree.CanUp ()) UpBranch (Tree);

        Tree.PopP ();

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_NEW)
    {
        GetBA (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_DEL)
    {
        GetCA (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_IF)
    {
        GetCC (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_WHILE)
    {
        GetCD (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_FOR)
    {
        GetCE (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (i + 1 < Size && Program [i].Descriptor == N_NAME && Program [i + 1].Descriptor == N_OB)
    {
        GetCF (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_ECHO || Program [i].Descriptor == N_GET)
    {
        GetCG (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_MARK || Program [i].Descriptor == N_GOTO)
    {
        GetCH (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (InFunc && Program [i].Descriptor == N_RETURN)
    {
        Tree.Set (Program [i]);

        i ++;

        Tree.DownL ();

        GetAA (Tree, Program, Size, i);

        Tree.Up ();

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (InCycle && Program [i].Descriptor == N_BREAK)
    {
        Tree.Set (Program [i]);

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }
    if (InCycle && Program [i].Descriptor == N_CONTINUE)
    {
        Tree.Set (Program [i]);

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    GetCB (Tree, Program, Size, i);

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//******************************************************************************

//GetBA new GetBB || new GetBB, ...
void GetBA (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetBA, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NEW) throw TH_ERROR "Expected |new|.");

    Tree.Set (Program [i]);

    i ++;

    Tree.DownL ();

    GetBB (Tree, Program, Size, i);

    Tree.Up ();

    if (i >= Size || Program [i].Descriptor != N_LIST)
    {
        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    i ++;

    Tree.PushP ();

    Tree.DownR ();

    NullTreeNode (Tree);

    Tree.Set (newNodeData (N_NODE));

    Tree.DownL ();
    GetBB (Tree, Program, Size, i);

    while (Program [i].Descriptor == N_LIST)
    {
        i ++;

        if (i >= Size) throw TH_ERROR "SUDDEN END");

        Tree.Up ();
        Tree.DownR ();

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        GetBB (Tree, Program, Size, i);
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetBB Name || Name = GetAA || Name [GetAA] || Name [GetAA] = {GetAA, ...} || &Name || Name (new GetBB, ...) = {GetC; || return GetAA; ...}
void GetBB (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetBB, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_LNK)
    {
        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

        Program [i].Descriptor = N_LNK;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i ++;

        return ;
    }

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    if (i + 1 < Size && Program [i + 1].Descriptor == N_OS)
    {
        Program [i].Descriptor = N_ARR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i += 2;

        bool EqSize = false;

        if (Program [i].Descriptor != N_CS)
        {
            Tree.DownL ();

            GetAA (Tree, Program, Size, i);

            Tree.Up ();
        }
        else
        {
            EqSize = true;
        }

        if (Program [i].Descriptor != N_CS) throw TH_ERROR "Expected |]|.");

        i ++;

        if (Program [i].Descriptor == N_EQ)
        {
            i ++;

            if (Program [i].Descriptor != N_OF) throw TH_ERROR "Expected |{|.");

            i ++;

            Tree.DownR ();

            GetArr (Tree, Program, Size, i);

            Tree.Up ();

            if (EqSize)
            {
                Tree.DownR ();
                Tree.DownR ();
                int _Size = GetListSize (Tree);
                Tree.Up ();
                Tree.Up ();

                Tree.DownL ();
                Tree.Set (newNodeData (N_NUM, _Size));
                Tree.Up ();
            }
        }
        else
        {
            //if (EqSize) throw TH_ERROR "Expected |instance|.", i--);
        }

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (i + 1 < Size && Program [i + 1].Descriptor == N_OB)
    {
        Program [i].Descriptor = N_FUN;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i += 2;

        if (Program [i].Descriptor != N_CB)
        {
            Tree.PushP ();

            Tree.DownL ();

            NullTreeNode (Tree);

            Tree.Set (newNodeData (N_NODE));

            Tree.DownL ();

            if (Program [i].Descriptor != N_NEW) throw TH_ERROR "Expected |new|.");

            Tree.Set (Program [i]);

            i ++;

            Tree.DownL ();

            GetBB (Tree, Program, Size, i);

            Tree.Up ();

            while (Program [i].Descriptor != N_CB)
            {
                if (i >= Size) throw TH_ERROR "SUDDEN END");

                if (Program [i].Descriptor != N_LIST) throw TH_ERROR "Expected |,|.");

                i ++;

                Tree.Up ();
                Tree.DownR ();

                Tree.Set (newNodeData (N_NODE));

                Tree.DownL ();

                if (Program [i].Descriptor != N_NEW) throw TH_ERROR "Expected |new|.");

                Tree.Set (Program [i]);

                i ++;

                Tree.DownL ();

                GetBB (Tree, Program, Size, i);

                Tree.Up ();
            }

            if (Tree.CanUp ()) UpBranch (Tree);

            Tree.PopP ();
        }

        i ++;

        if (Program [i].Descriptor == N_OF)
        {
            Tree.DownR ();

            if (InFunc)
            {
                GetC (Tree, Program, Size, i);
            }
            else
            {
                InFunc = true;

                GetC (Tree, Program, Size, i);

                InFunc = false;
            }

            if (!Tree.CanDownL () && !Tree.CanDownR ())
            {
                Tree.Set (newNodeData (N_RETURN));
                Tree.DownL ();
                Tree.Set (newNodeData (N_NUM, 0));
                Tree.Up ();
            }

            Tree.Up ();
        }

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_NAME)
    {
        Program [i].Descriptor = N_VAR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i ++;

        if (Program [i].Descriptor == N_EQ)
        {
            i ++;

            Tree.DownR ();

            GetAA (Tree, Program, Size, i);

            Tree.Up ();
        }

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }
}

//******************************************************************************

//GetCA del Name || del Name[]
void GetCA (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCA, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_DEL) throw TH_ERROR "Expected |del|.");

    if (i + 1 >= Size) throw TH_ERROR "SUDDEN END");

    i ++;

    if (i + 2 < Size && Program [i].Descriptor == N_NAME && Program [i + 1].Descriptor == N_OS && Program [i + 2].Descriptor == N_CS)
    {
        Tree.Set (Program [i - 1]);

        Tree.DownL ();

        Program [i].Descriptor = N_ARR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i += 3;

        Tree.Up ();
    }
    else
    {
        Tree.Set (Program [i - 1]);

        Tree.DownL ();

        if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

        Program [i].Descriptor = N_VAR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        Tree.Up ();

        i ++;
    }

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCB GetAG = GetAA
void GetCB (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCB, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.DownL ();

    GetAG (Tree, Program, Size, i);

    Tree.Up ();

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_EQ)  throw TH_ERROR "Expected |=|.");

    Tree.Set (Program [i]);

    i ++;

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.DownR ();

    GetAA (Tree, Program, Size, i);

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCC if (GetAA) GetC || if (GetAA) GetC else GetC
void GetCC (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCC, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");
    if (Program [i].Descriptor != N_IF) throw TH_ERROR "Expected |if|.");

    Tree.Set (Program [i]);

    i ++;

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_OB) throw TH_ERROR "Expected |(|.");

    i ++;

    Tree.DownL ();

    GetAA (Tree, Program, Size, i);

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_CB) throw TH_ERROR "Expected |)|.");

    i ++;

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.Up ();

    Tree.DownR ();

    GetC (Tree, Program, Size, i);

    if (Program [i - 1].Descriptor != N_CF)
    {
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
        else i ++;
    }
    else
    {
        if (Program [i].Descriptor == N_END) i ++;
    }

    if (Program [i].Descriptor != N_ELSE)
    {
        Tree.Up ();

        if (Program [i - 1].Descriptor == N_END) i --;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    Tree.DownL ();

    Tree.Up ();

    DownBranchL (Tree);

    Tree.Set (Program [i]);

    Tree.DownR ();

    i ++;

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    GetC (Tree, Program, Size, i);

    Tree.Up ();
    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCD while (GetAA) {GetC; || break;, ...}
void GetCD (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCD, Pos = %d\n", i);
    #endif

    if (i + 1 >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor     != N_WHILE)  throw TH_ERROR "Expected |while|.");
    if (Program [i + 1].Descriptor != N_OB   )  throw TH_ERROR "Expected |(|.");

    Tree.Set (Program [i]);

    i += 2;

    Tree.DownL ();

    GetAA (Tree, Program, Size, i);

    Tree.Up ();

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_CB)  throw TH_ERROR "Expected |)|.");

    i ++;

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.DownR ();

    if (InCycle)
    {
        GetC (Tree, Program, Size, i);
    }
    else
    {
        InCycle = true;

        GetC (Tree, Program, Size, i);

        InCycle = false;
    }

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCE for (GetBA; GetAA; GetC) {GetC; || break;, ...}
void GetCE (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCE, Pos = %d\n", i);
    #endif

    if (i + 1 >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor     != N_FOR)  throw TH_ERROR "Expected |for|.");
    if (Program [i + 1].Descriptor != N_OB )  throw TH_ERROR "Expected |(|.");

    Tree.Set (Program [i]);

    i += 2;

    Tree.DownL ();

    Tree.Set (newNodeData (N_NODE));

    Tree.Up ();

    Tree.DownR ();

    Tree.Set (newNodeData (N_NODE));

    Tree.Up ();

    Tree.DownL ();

    Tree.DownL ();

    GetBA (Tree, Program, Size, i);

    Tree.Up ();

    Tree.Up ();

    if (Program [i - 1].Descriptor != N_CF)
    {
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
        else i ++;
    }
    else
    {
        if (Program [i].Descriptor == N_END) i ++;
    }

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.DownR ();

    Tree.DownL ();

    GetAA (Tree, Program, Size, i);

    Tree.Up ();

    Tree.Up ();

    if (Program [i - 1].Descriptor != N_CF)
    {
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected |;|.");
        else i ++;
    }
    else
    {
        if (Program [i].Descriptor == N_END) i ++;
    }

    Tree.DownL ();

    Tree.DownR ();

/**/GetC (Tree, Program, Size, i);

    Tree.Up ();

    Tree.Up ();

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_CB)  throw TH_ERROR "Expected |)|.");

    i ++;

    Tree.DownR ();

    Tree.DownR ();

    if (InCycle)
    {
        GetC (Tree, Program, Size, i);
    }
    else
    {
        InCycle = true;

        GetC (Tree, Program, Size, i);

        InCycle = false;
    }

    Tree.Up ();

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCF Name (GetAA || Name [] || &Name || &Name [GetAA], ...)
void GetCF (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCF, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    Program [i].Descriptor = N_FUN;
    Tree.Set (Program [i]);
    Program [i].Descriptor = N_NAME;

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_OB) throw TH_ERROR "Expected |(|.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_CB)
    {
        Tree.PushP ();

        Tree.DownL ();

        NullTreeNode (Tree);

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        if (Program [i].Descriptor == N_OF)
        {
            i ++;

            GetArr (Tree, Program, Size, i);
        }
        else if (Program [i].Descriptor == N_LNK)
        {
            Tree.Set (newNodeData (N_LNK));
            Tree.DownL ();

            i ++;

            if (i >= Size) throw TH_ERROR "SUDDEN END");
            if (Program [i].Descriptor != N_NAME)  throw TH_ERROR "Expected |Name|.");

            if (i + 1 < Size && Program [i + 1].Descriptor == N_OS)
            {
                Program [i].Descriptor = N_ARR;
                Tree.Set (Program [i]);
                Program [i].Descriptor = N_NAME;

                i += 2;

                Tree.DownL ();
                GetAA (Tree, Program, Size, i);
                Tree.Up ();

                if (i >= Size) throw TH_ERROR "SUDDEN END");
                if (Program [i].Descriptor != N_CS)  throw TH_ERROR "Expected |]|.");
            }
            else
            {
                Program [i].Descriptor = N_VAR;
                Tree.Set (Program [i]);
                Program [i].Descriptor = N_NAME;
            }

            Tree.Up ();

            i ++;
        }
        else
        {
            if (i + 2 < Size && Program [i].Descriptor == N_NAME && Program [i + 1].Descriptor == N_OS && Program [i + 2].Descriptor == N_CS)
            {
                Program [i].Descriptor = N_ARR;
                Tree.Set (Program [i]);
                Program [i].Descriptor = N_NAME;

                i += 3;
            }
            else GetAA (Tree, Program, Size, i);
        }

        while (Program [i].Descriptor != N_CB)
        {
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_LIST) throw TH_ERROR "Expected |,|.");

            i ++;

            Tree.Up ();
            Tree.DownR ();

            Tree.Set (newNodeData (N_NODE));

            Tree.DownL ();

            if (Program [i].Descriptor == N_OF)
            {
                i ++;

                GetArr (Tree, Program, Size, i);
            }
            else if (Program [i].Descriptor == N_LNK)
            {
                Tree.Set (newNodeData (N_LNK));
                Tree.DownL ();

                i ++;

                if (i >= Size) throw TH_ERROR "SUDDEN END");
                if (Program [i].Descriptor != N_NAME)  throw TH_ERROR "Expected |Name|.");

                if (i + 1 < Size && Program [i + 1].Descriptor == N_OS)
                {
                    Program [i].Descriptor = N_ARR;
                    Tree.Set (Program [i]);
                    Program [i].Descriptor = N_NAME;

                    i += 2;

                    Tree.DownL ();
                    GetAA (Tree, Program, Size, i);
                    Tree.Up ();

                    if (i >= Size) throw TH_ERROR "SUDDEN END");
                    if (Program [i].Descriptor != N_CS)  throw TH_ERROR "Expected |]|.");
                }
                else
                {
                    Program [i].Descriptor = N_VAR;
                    Tree.Set (Program [i]);
                    Program [i].Descriptor = N_NAME;
                }

                Tree.Up ();

                i ++;
            }
            else
            {
                if (i + 2 < Size && Program [i].Descriptor == N_NAME && Program [i + 1].Descriptor == N_OS && Program [i + 2].Descriptor == N_CS)
                {
                    Program [i].Descriptor = N_ARR;
                    Tree.Set (Program [i]);
                    Program [i].Descriptor = N_NAME;

                    i += 3;
                }
                else GetAA (Tree, Program, Size, i);
            }
        }

        if (Tree.CanUp ()) UpBranch (Tree);

        Tree.PopP ();
    }

    i ++;
}

//GetCG echo GetAA || get GetAG
void GetCG (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCG, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_ECHO && Program [i].Descriptor != N_GET) throw TH_ERROR "Expected |echo| or |get|.");

    Tree.Set (Program [i]);

    i ++;

    Tree.DownL ();

    if (Program [i - 1].Descriptor == N_ECHO) GetAA (Tree, Program, Size, i);
    else                                      GetAG (Tree, Program, Size, i);

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCH : Name || goto Name
void GetCH (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCG, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_MARK && Program [i].Descriptor != N_GOTO) throw TH_ERROR "Expected |:| or |goto|.");

    i ++;

    if (Program [i].Descriptor != N_NAME)  throw TH_ERROR "Expected |Name|.");

    Program [i].Descriptor = Program [i - 1].Descriptor;

    Tree.Set (Program [i]);

    Program [i].Descriptor = N_NAME;

    i ++;

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

void GetArr (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    Tree.Set (newNodeData (N_NODE));

    Tree.DownR ();

    if (Program [i].Descriptor != N_CF)
    {
        Tree.PushP ();

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        GetAA (Tree, Program, Size, i);

        while (Program [i].Descriptor != N_CF)
        {
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_LIST) throw TH_ERROR "Expected |,|.");

            i ++;

            Tree.Up ();
            Tree.DownR ();

            Tree.Set (newNodeData (N_NODE));

            Tree.DownL ();

            GetAA (Tree, Program, Size, i);
        }

        if (Tree.CanUp ()) UpBranch (Tree);

        Tree.PopP ();
    }
    else Tree.Set (newNodeData (N_NUM, 0));

    i ++;

    Tree.Up ();
}


//******************************************************************************
#endif
