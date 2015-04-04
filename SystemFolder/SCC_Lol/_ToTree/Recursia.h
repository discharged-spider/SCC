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

bool InFunc = false, InCycle = false;
void GetC_REC (newTree& Tree, newNodeData Program [], int& Size, int& i, int END, int offset = 0);
void GetC (newTree& Tree, newNodeData Program [], int& Size, int& i);

void GetBA (newTree& Tree, newNodeData Program [], int& Size, int& i);

void GetCA (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCB (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCC (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCD (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCF (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCG (newTree& Tree, newNodeData Program [], int& Size, int& i);
void GetCH (newTree& Tree, newNodeData Program [], int& Size, int& i);

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

// GetMain i < size GetC
void GetMain (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetMain, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    while (Program [i].Descriptor == N_END) i ++;

    if (Program [i].Descriptor != N_HAI) throw TH_ERROR "Expected HAI.");

    i ++;

    GetC_REC (Tree, Program, Size, i, N_KTHXBYE);

    if (i >= Size) throw TH_ERROR "SUDDEN END");
    if (Program [i].Descriptor != N_KTHXBYE) throw TH_ERROR "Expected KTHXBYE.");

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//******************************************************************************

//GetAA (SUM, SUB, MUL, DIV, MOD, AND, OR) OF GetAA AN GetAA || N_OPEQ SAEM GetAA AN GetAA || N_NONEQ GetAA AN GetAA || THS GetAA (MRE, LSE) THN GetAA || NOT GetAA || GetAB
void GetAA (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAA, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    NullTreeNode (Tree);

    bool two = false;

    int desc = Program [i].Descriptor;
    if (!two &&
        (desc == N_SUM ||
        desc == N_SUB ||
        desc == N_MUL ||
        desc == N_DIV ||
        desc == N_MOD ||
        desc == N_OR))
    {
        Tree.Set (Program[i]);

        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_OF) throw TH_ERROR "Expected OF");

        two = true;
    }
    if (!two && (desc == N_BOTH))
    {
        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_SAEM && Program [i].Descriptor != N_OF) throw TH_ERROR "Expected SAEM or OF");

        if (Program [i].Descriptor == N_SAEM) Program [i - 1].Descriptor = N_OPEQ;
        if (Program [i].Descriptor == N_OF) Program [i - 1].Descriptor = N_AND;

        Tree.Set (Program[i - 1]);

        Program [i - 1].Descriptor = N_BOTH;

        two = true;
    }
    if (!two && (desc == N_NONEQ))
    {
        Tree.Set (Program[i]);

        two = true;
    }


    if (two)
    {
        i ++;

        Tree.DownL ();
        GetAA (Tree, Program, Size, i);
        Tree.Up ();

        if (i >= Size) throw TH_ERROR "SUDDEN END");
        if (Program [i].Descriptor != N_AN) throw TH_ERROR "Expected AN");

        i ++;

        Tree.DownR ();
        GetAA (Tree, Program, Size, i);
        Tree.Up ();

        #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (desc == N_NOT)
    {
        Tree.Set (Program[i]);

        i ++;

        Tree.DownL ();
        GetAA (Tree, Program, Size, i);
        Tree.Up ();

        #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (desc == N_THS)
    {
        i ++;

        Tree.DownL ();
        GetAA (Tree, Program, Size, i);
        Tree.Up ();

        if (i >= Size) throw TH_ERROR "SUDDEN END");
        if (Program [i].Descriptor != N_MORE && Program [i].Descriptor != N_LESS) throw TH_ERROR "Expected MORE or LESS");

        Tree.Set (Program[i]);

        i ++;

        if (i >= Size) throw TH_ERROR "SUDDEN END");
        if (Program [i].Descriptor != N_THN) throw TH_ERROR "Expected THN");

        i ++;

        Tree.DownR ();
        GetAA (Tree, Program, Size, i);
        Tree.Up ();


        #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    GetAB (Tree, Program, Size, i);

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAB [0-9] || TRUE || FALSE || (+, -) GetAB || GetAC || GetCF
void GetAB (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAB, Pos = %d\n", i);
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
        GetAB (Tree, Program, Size, i);
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

        GetAB (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_I)
    {
        GetCF (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    GetAC (Tree, Program, Size, i);

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetAC Name
void GetAC (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetAC, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|");

    Program [i].Descriptor = N_VAR;

    Tree.Set (Program [i]);

    Program [i].Descriptor = N_NAME;

    i ++;

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//******************************************************************************
void GetC_REC (newTree& Tree, newNodeData Program [], int& Size, int& i, int END, int offset)
{
    #ifdef DEBUG
        printf ("GetC_REC, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (i + offset > Size || Program [i + offset].Descriptor == END)
    {
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

    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE");
    i ++;

    while (i + offset < Size && Program [i + offset].Descriptor != END)
    {
        if (Program [i].Descriptor == N_END) continue;

        Tree.Up ();
        Tree.DownR ();

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        GetC (Tree, Program, Size, i);

        if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE");
        i ++;
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}


//GetC empty || GetBA || GetCA || GetCB || GetCC || GetCD || GetCF || GetCG || GetCH
void GetC (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetC, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_END)
    {
        return;
    }

    if (i + 1 < Size && ((Program [i].Descriptor == N_I && Program [i + 1].Descriptor == N_HAS) || (Program [i].Descriptor == N_HOW && Program [i + 1].Descriptor == N_IZ)))
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

    if (i + 1 < Size && Program [i + 1].Descriptor == N_EQ)
    {
        GetCB (Tree, Program, Size, i);

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (i + 1 < Size && Program [i].Descriptor == N_I && Program [i + 1].Descriptor == N_IZ)
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

    if (Program [i].Descriptor == N_IM)
    {
        GetCD (Tree, Program, Size, i);

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

        Tree.Set (newNodeData (N_NUM, 0));

        Tree.Up ();

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (InFunc && Program [i].Descriptor == N_FOUND)
    {
        Program [i].Descriptor = N_RETURN;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_FOUND;

        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_YR) throw TH_ERROR "Expected YR");

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

    GetCC (Tree, Program, Size, i);

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//******************************************************************************

//GetBA I HAS A Name || I HAS A Name ITZ GetAA || HOW IZ I Name (YR Name AN YR Name, ...) (NVR MND || (GetC, GetC | GTFO | FOUND YR GetAA) IF U CAN SO)
void GetBA (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetBA, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_I)
    {
        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_HAS) throw TH_ERROR "Expected HAS");

        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_A) throw TH_ERROR "Expected A");

        Program [i].Descriptor = N_NEW;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_A;

        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|");

        Tree.DownL ();

        Program [i].Descriptor = N_VAR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        i ++;

        if (i < Size && Program [i].Descriptor == N_ITZ)
        {
            i ++;

            Tree.DownR ();

            GetAA (Tree, Program, Size, i);

            Tree.Up ();
        }

        Tree.Up ();

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    if (Program [i].Descriptor == N_HOW) throw TH_ERROR "Expected HOW.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_IZ) throw TH_ERROR "Expected IZ.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_I) throw TH_ERROR "Expected I.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    Tree.Set (newNodeData (N_NEW));
    Tree.DownL ();

    Program [i].Descriptor = N_FUN;
    Tree.Set (Program [i]);
    Program [i].Descriptor = N_NAME;

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_YR)
    {
        Tree.PushP ();

        Tree.DownL ();

        NullTreeNode (Tree);

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

        Tree.Set (newNodeData (N_NEW));

        Tree.DownL ();

        Program [i].Descriptor = N_VAR;
        Tree.Set (Program [i]);
        Program [i].Descriptor = N_NAME;

        Tree.Up ();

        i ++;

        if (i >= Size) throw TH_ERROR "SUDDEN END");

        while (Program [i].Descriptor == N_AN)
        {
            i ++;
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_YR) throw TH_ERROR "Expected YR.");

            i ++;

            Tree.Up ();
            Tree.DownR ();

            Tree.Set (newNodeData (N_NODE));

            Tree.DownL ();

            if (i >= Size) throw TH_ERROR "SUDDEN END");
            if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

            Tree.Set (newNodeData (N_NEW));

            Tree.DownL ();

            Program [i].Descriptor = N_VAR;
            Tree.Set (Program [i]);
            Program [i].Descriptor = N_NAME;

            Tree.Up ();

            i ++;
        }

        if (Tree.CanUp ()) UpBranch (Tree);

        Tree.PopP ();
    }

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor == N_NVR)
    {
        i ++;
        if (i >= Size) throw TH_ERROR "SUDDEN END");

        if (Program [i].Descriptor != N_MND) throw TH_ERROR "Expected MND.");

        Tree.Up ();

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    Tree.DownR ();

    if (InFunc)
    {
        GetC_REC (Tree, Program, Size, i, N_IF);
    }
    else
    {
        InFunc = true;

        GetC_REC (Tree, Program, Size, i, N_IF);

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

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_U) throw TH_ERROR "Expected U.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_CAN) throw TH_ERROR "Expected CAN.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_SO) throw TH_ERROR "Expected SO.");

    i ++;

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif

    return;
}

//******************************************************************************

//GetCA del Name
void GetCA (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCA, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_DEL) throw TH_ERROR "Expected FGT.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.Set (Program [i - 1]);

    Tree.DownL ();

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    Program [i].Descriptor = N_VAR;
    Tree.Set (Program [i]);
    Program [i].Descriptor = N_NAME;

    Tree.Up ();

    i ++;

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCB GetAC = GetAA
void GetCB (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCB, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    Tree.DownL ();

    GetAC (Tree, Program, Size, i);

    Tree.Up ();

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_EQ) throw TH_ERROR "Expected R.");

    Tree.Set (Program [i]);

    i ++;

    Tree.DownR ();

    GetAA (Tree, Program, Size, i);

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCC GetAA END O RLY? END YA RLY END GetC (NO WAI END GetC END) OIC
void GetCC (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCC, Pos = %d\n", i);
    #endif

    Tree.Set (newNodeData (N_IF));

    Tree.DownL ();

    GetAA (Tree, Program, Size, i);

    Tree.Up ();

    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_O) throw TH_ERROR "Expected O.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_RLY_Q) throw TH_ERROR "Expected RLY?.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_YA) throw TH_ERROR "Expected YA.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_RLY) throw TH_ERROR "Expected RLY.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE.");

    Tree.DownR ();

    Tree.PushP ();

    NullTreeNode (Tree);

    Tree.Set (newNodeData (N_NODE));

    Tree.DownL ();
    GetC (Tree, Program, Size, i);

    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE");
    i ++;

    while (i < Size && Program [i].Descriptor != N_OIC && Program [i].Descriptor != N_NO)
    {
        if (Program [i].Descriptor == N_END) continue;

        Tree.Up ();
        Tree.DownR ();

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        GetC (Tree, Program, Size, i);

        if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE");
        i ++;
    }

    if (Tree.CanUp ()) UpBranch (Tree);

    Tree.PopP ();

    if (Program [i].Descriptor != N_NO && Program [i].Descriptor != N_OIC) throw TH_ERROR "Expected OIC or NO.");

    if (Program [i].Descriptor == N_OIC)
    {
        Tree.Up ();

        i ++;

        #ifdef DEBUG
            printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
        #endif

        return;
    }

    Tree.DownL ();

    Tree.Up ();

    DownBranchL (Tree);

    Tree.Set (newNodeData (N_ELSE));

    Tree.DownR ();

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_WAI) throw TH_ERROR "Expected WAI.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE.");

    i ++;

    GetC_REC(Tree, Program, Size, i, N_OIC);

    Tree.Up ();
    Tree.Up ();

    if (Program [i].Descriptor != N_OIC) throw TH_ERROR "Expected OIC.");

    i ++;

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCD IM IN YR Name WILE GetAA END (GetC | GTFOHR) IM OUTTA YR Name
void GetCD (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCD, Pos = %d\n", i);
    #endif

    if (i + 1 >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_IM) throw TH_ERROR "Expected IM.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_IN) throw TH_ERROR "Expected IN.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_YR) throw TH_ERROR "Expected YR.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    newNodeData mark = Program [i];
    mark.Descriptor = N_MARK;

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_WHILE) throw TH_ERROR "Expected WILE.");

    Tree.Set (Program [i]);

    i ++;

    Tree.DownL ();

    GetAA (Tree, Program, Size, i);

    Tree.Up ();

    if (i >= Size) throw TH_ERROR "SUDDEN END");
    if (Program [i].Descriptor != N_END) throw TH_ERROR "Expected ',' or NEWLINE");
    i ++;

    Tree.DownR ();

    Tree.DownL ();

    Tree.Set (mark);

    Tree.Up ();

    Tree.DownR ();

    if (InCycle)
    {
        GetC_REC (Tree, Program, Size, i, N_OUTTA, 1);
    }
    else
    {
        InCycle = true;

        GetC_REC (Tree, Program, Size, i, N_OUTTA, 1);

        InCycle = false;
    }

    Tree.Up ();

    Tree.Up ();

    if (Program [i].Descriptor != N_IM) throw TH_ERROR "Expected IM.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_OUTTA) throw TH_ERROR "Expected OUTTA.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_YR) throw TH_ERROR "Expected YR.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");
    if (strcmp (Program [i].GetName(), mark.GetName()) != 0) throw TH_ERROR "End name of cycle must be simular with start name.");

    i ++;

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCF I IZ Name (YR GetAA AN YR GetAA, ...) MKAY
void GetCF (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCF, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_I) throw TH_ERROR "Expected I.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_IZ) throw TH_ERROR "Expected IZ.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    Program [i].Descriptor = N_FUN;
    Tree.Set (Program [i]);
    Program [i].Descriptor = N_NAME;

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_MKAY)
    {
        Tree.PushP ();

        Tree.DownL ();

        NullTreeNode (Tree);

        Tree.Set (newNodeData (N_NODE));

        Tree.DownL ();

        if (Program [i].Descriptor != N_YR) throw TH_ERROR "Expected YR.");

        i ++;

        GetAA (Tree, Program, Size, i);

        if (i >= Size) throw TH_ERROR "SUDDEN END");

        while (Program [i].Descriptor != N_MKAY)
        {
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_AN) throw TH_ERROR "Expected AN.");

            i ++;
            if (i >= Size) throw TH_ERROR "SUDDEN END");

            if (Program [i].Descriptor != N_YR) throw TH_ERROR "Expected YR.");

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

    i ++;
}

//GetCG echo GetAA || get GetAC
void GetCG (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCG, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_ECHO && Program [i].Descriptor != N_GET) throw TH_ERROR "Expected VISIBLE or GIMMEH.");

    Tree.Set (Program [i]);

    i ++;

    Tree.DownL ();

    if (Program [i - 1].Descriptor == N_ECHO) GetAA (Tree, Program, Size, i);
    else                                      GetAC (Tree, Program, Size, i);

    Tree.Up ();

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//GetCH : IS Name || goto Name
void GetCH (newTree& Tree, newNodeData Program [], int& Size, int& i)
{
    #ifdef DEBUG
        printf ("GetCG, Pos = %d\n", i);
    #endif

    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_MARK && Program [i].Descriptor != N_GOTO) throw TH_ERROR "Expected HR or GR.");

    i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i - 1].Descriptor == N_MARK && Program [i].Descriptor != N_IZ) throw TH_ERROR "Expected IZ.");

    int desc = Program [i - 1].Descriptor;

    if (Program [i - 1].Descriptor == N_MARK) i ++;
    if (i >= Size) throw TH_ERROR "SUDDEN END");

    if (Program [i].Descriptor != N_NAME) throw TH_ERROR "Expected |Name|.");

    Program [i].Descriptor = desc;

    Tree.Set (Program [i]);

    Program [i].Descriptor = N_NAME;

    i ++;

    #ifdef DEBUG
        printf ("Return from %s, Pos = %d\n", __PRETTY_FUNCTION__, i);
    #endif
}

//******************************************************************************
#endif
