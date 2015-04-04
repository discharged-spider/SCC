#ifndef TREE_TO_LOLCODE_H
#define TREE_TO_LOLCODE_H

#include "stdio.h"
#include "..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "..\..\SCC_Tree\Lib\TreeOperations.h"
#include "..\..\SCC_Tree\InfoMaker\InfoMaker.h"

#include "vector"

#define OUT_PREP \
for (int out_counter = 0; out_counter < LData.offset; out_counter ++) \
    fprintf (To, "  ");

#define OUT_(...) fprintf (To, __VA_ARGS__)

#define OUT(...) \
OUT_PREP \
OUT_(__VA_ARGS__); \
fprintf (To, "\n")

struct newTreeToLolcodeData
{
    int offset;

    int CurrentMark;

    int InCycle;

    int InFunction;

    bool fillVars;    std::vector <int> variables;
};

void Create    (FILE* To, newTree& Tree, newTreeInfo Info, newVector <newNodeInfo> NodeInfo);
void CreateRec (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

//------------------------------------------------------------------------------

void PushInstance (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

void New (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);
void PushArr (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData, int ArrAddr, int i = 0);

void Equal (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);
void SelfEqual (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

void Echo (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);
void Get (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

void If (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

void While (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);
void For (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

void NewFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);
void CallFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LLData);

void PushParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData, int& i);
void PopParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData, bool first = true);

//------------------------------------------------------------------------------

void Create (FILE* To, newTree& Tree, newTreeInfo Info, newVector <newNodeInfo> NodeInfo)
{
    if (Info.ArrNumber > 0) throw TH_ERROR "Arrs not support, sorry. :(");
    if (Info.UseNatives) throw TH_ERROR "Use natives?");

    newTreeToLolcodeData LData;

    LData.offset = 0;

    LData.CurrentMark = 1;

    LData.InCycle = -1;

    LData.fillVars = false;

    OUT ("HAI");

    LData.offset ++;

    CreateRec (To, Tree, Info, NodeInfo, LData);

    LData.offset --;

    OUT ("KTHXBYE");
}

void CreateRec (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NEW)
    {
        New (To, Tree, Info, NodeInfo, LData);

        return ;
    }
    if (Data.Descriptor == N_DEL)
    {
        Tree.DownL ();

        OUT ("FGT %s", Tree.Get ().GetName());

        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_NATIVE)
    {
        OUT_PREP
        New (To, Tree, Info, NodeInfo, LData);
        OUT_ ("\n");

        return ;
    }

    if (Data.Descriptor == N_EQ)
    {
        OUT_PREP
        Equal (To, Tree, Info, NodeInfo, LData);
        OUT_ ("\n");

        return ;
    }

    if (
       Data.Descriptor == N_INCR   ||
       Data.Descriptor == N_DECR   ||
       Data.Descriptor == N_SUM_EQ ||
       Data.Descriptor == N_SUB_EQ ||
       Data.Descriptor == N_MUL_EQ ||
       Data.Descriptor == N_DIV_EQ ||
       Data.Descriptor == N_MOD_EQ
       )
    {
        OUT_PREP
        SelfEqual (To, Tree, Info, NodeInfo, LData);
        OUT_ ("\n");

        return ;
    }

    if (Data.Descriptor == N_ECHO)
    {
        OUT_PREP
        Echo (To, Tree, Info, NodeInfo, LData);
        OUT_ ("\n");

        return ;
    }
    if (Data.Descriptor == N_GET)
    {
        OUT_PREP
        Get (To, Tree, Info, NodeInfo, LData);
        OUT_ ("\n");

        return ;
    }

    if (Data.Descriptor == N_IF)
    {
        If (To, Tree, Info, NodeInfo, LData);

        return ;
    }

    if (Data.Descriptor == N_BREAK && LData.InCycle != -1)
    {
        OUT ("GTFOHR");

        return ;
    }
    if (Data.Descriptor == N_CONTINUE && LData.InCycle != -1)
    {
        OUT ("GT LOOP_%d", LData.InCycle);

        return ;
    }

    if (Data.Descriptor == N_RETURN)
    {
        if (!Tree.CanDownL())
        {
            OUT ("GTFO");

            return;
        }

        Tree.DownL ();

        OUT_PREP
        OUT_ ("FOUND YR ");

        PushInstance (To, Tree, Info, NodeInfo, LData);

        OUT_ ("\n");

        Tree.Up ();

        return ;
    }

    if (Data.Descriptor == N_WHILE)
    {
        While (To, Tree, Info, NodeInfo, LData);

        return ;
    }
    if (Data.Descriptor == N_FOR)
    {
        For (To, Tree, Info, NodeInfo, LData);

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        OUT_PREP
        CallFunc (To, Tree, Info, NodeInfo, LData);
        OUT_ ("\n");

        return ;
    }

    if (Data.Descriptor == N_GOTO)
    {
        OUT ("GT MY_%s", Data.GetName());

        return ;
    }
    if (Data.Descriptor == N_MARK)
    {
        fprintf (To, "HR IZ MY_%s\n", Data.GetName ());

        return ;
    }

    if (Tree.CanDownL())
    {
        Tree.DownL ();
        CreateRec (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }
    if (Tree.CanDownR())
    {
        Tree.DownR ();
        CreateRec (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }
}

//------------------------------------------------------------------------------

void PushInstance (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_TRUE)
    {
        OUT_ ("WIN");

        return;
    }
    if (Data.Descriptor == N_FALSE)
    {
        OUT_ ("FAIL");

        return;
    }

    if (Data.Descriptor == N_NUM)
    {
        OUT_ ("%g", Data.Data);

        return;
    }

    if (Data.Descriptor == N_VAR)
    {
        OUT_ ("%s", Data.GetName());

        return;
    }

    if (Data.Descriptor == N_ARR)
    {
        //NOT SUPPORTED IN LOLCODE!
        return;
    }

    if (Data.Descriptor == N_FUN)
    {
        CallFunc (To, Tree, Info, NodeInfo, LData);

        return;
    }

    if (Data.Descriptor == N_AND)
    {
        OUT_ ("BOTH OF ");

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" AN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_OR)
    {
        OUT_ ("EITHER OF ");

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" AN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }

    if (Data.Descriptor == N_NOT)
    {
        OUT_ ("NOT ");

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_OPEQ)
    {
        OUT_ ("BOTH SAEM ");

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" AN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_NONEQ)
    {
        OUT_ ("DIFFRINT ");

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" AN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_MORE)
    {
        OUT_ ("THS ");
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" MRE THN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_LESS)
    {
        OUT_ ("THS ");
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" LSE THN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_MREQ)
    {        OUT_ ("NOT THS ");

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" LSE THN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_LSEQ)
    {        OUT_ ("NOT THS ");

         Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        OUT_ (" MRE THN ");

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();

        return ;
    }

    if (Data.Descriptor == N_SUM) OUT_ ("SUM");
    if (Data.Descriptor == N_SUB) OUT_ ("DIFF");
    if (Data.Descriptor == N_MUL) OUT_ ("PRODUKT");
    if (Data.Descriptor == N_DIV) OUT_ ("QUOSHUNT");
    if (Data.Descriptor == N_MOD) OUT_ ("MOD");

    OUT_ (" OF ");

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }

    OUT_ (" AN ");

    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }
}

void New (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR)
    {
        if (LData.fillVars)
        {
            LData.variables.push_back (Tree.CurrentNode ());
        }

        OUT_PREP
        OUT_ ("I HAS A %s", Data.GetName ());

        if (Tree.CanDownR ())
        {
            OUT_ (" ITZ ", Data.GetName ());

            Tree.DownR ();
            PushInstance (To, Tree, Info, NodeInfo, LData);
            Tree.Up ();
        }

        OUT_ ("\n");

        return ;
    }
    if (Data.Descriptor == N_ARR)
    {
        //NOT SUPORTED

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        if (Tree.CanDownR ())
        {
            NewFunc (To, Tree, Info, NodeInfo, LData);
        }
        else
        {
            OUT_ ("HOW IZ I %s NVR MND", Data.GetName ());
        }

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        New (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        New (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }
}

void Equal (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        OUT_ ("%s R ", Tree.Get ().GetName());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        //NOT SUPPORTED
    }

    Tree.Up ();

    Tree.DownR ();
    PushInstance (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();
}

void SelfEqual (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        OUT_ ("%s R ", Tree.Get ().GetName());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        //NOT SUPPORTED
    }

    Tree.Up ();

    if (Tree.Get ().Descriptor == N_INCR)   OUT_ ("SUM");
    if (Tree.Get ().Descriptor == N_DECR)   OUT_ ("DIFF");
    if (Tree.Get ().Descriptor == N_SUM_EQ) OUT_ ("SUM");
    if (Tree.Get ().Descriptor == N_SUB_EQ) OUT_ ("DIFF");
    if (Tree.Get ().Descriptor == N_MUL_EQ) OUT_ ("PRODUKT");
    if (Tree.Get ().Descriptor == N_DIV_EQ) OUT_ ("QUOSHUNT");
    if (Tree.Get ().Descriptor == N_MOD_EQ) OUT_ ("MOD");

    OUT_ (" OF ");

    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        OUT_ ("%s", Tree.Get ().GetName());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        //NOT SUPPORTED
    }

    Tree.Up ();

    OUT_ (" AN ");

    if (Tree.Get ().Descriptor != N_INCR && Tree.Get ().Descriptor != N_DECR)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, LData);
        Tree.Up ();
    }
    else
    {
        OUT_ ("1");
    }
}

void Echo (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    OUT_ ("VISIBLE ");

    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();
}

void Get (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    OUT_ ("GIMMEH ");

    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        OUT_ ("%s", Tree.Get ().GetName());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        //NOT SUPPORTED
    }

    Tree.Up ();
}

void If (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    OUT_PREP

    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();

    OUT_ ("\n");

    OUT ("O RLY?");

    Tree.DownR ();

    if (Tree.Get ().Descriptor == N_ELSE)
    {
        Tree.DownL ();
    }

    LData.offset ++;

    OUT ("YA RLY");

    LData.offset ++;

    CreateRec (To, Tree, Info, NodeInfo, LData);

    LData.offset --;

    Tree.Up ();

    if (Tree.Get ().Descriptor == N_ELSE)
    {
        Tree.DownR ();

        OUT ("NO WAI");

        LData.offset ++;

        CreateRec (To, Tree, Info, NodeInfo, LData);

        LData.offset --;

        Tree.Up ();

        Tree.Up ();
    }

    LData.offset --;

    OUT ("OIC");
}

void While (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    int IC = LData.InCycle;
    LData.InCycle = LData.CurrentMark;
    LData.CurrentMark ++;

    OUT_PREP
    OUT_ ("IM IN YR LOOP_%d WILE ", LData.InCycle);

    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();

    OUT_ ("\n");

    LData.offset ++;

    Tree.DownR ();
    CreateRec (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();

    LData.offset --;

    OUT ("IM OUTTA YR LOOP_%d", LData.InCycle);

    LData.InCycle = IC;
}

void For (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    int IC = LData.InCycle;
    LData.InCycle = LData.CurrentMark;
    LData.CurrentMark ++;

    //Preparation

    int offset = LData.variables.size ();
    bool temp = LData.fillVars;    LData.fillVars = true;

    Tree.DownL ();
    Tree.DownL ();
    CreateRec (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();
    Tree.Up ();

    LData.fillVars = temp;

    OUT_PREP
    OUT_ ("IM IN YR LOOP_%d WILE ", LData.InCycle);

    Tree.DownR ();
    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();
    Tree.Up ();

    OUT_ ("\n");

    LData.offset ++;

    Tree.DownR ();
    Tree.DownR ();
    CreateRec (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();
    Tree.Up ();

    OUT (""); //Empty line

    Tree.DownL ();
    Tree.DownR ();
    CreateRec (To, Tree, Info, NodeInfo, LData);
    Tree.Up ();
    Tree.Up ();

    LData.offset --;

    OUT ("IM OUTTA YR LOOP_%d", LData.InCycle);
    LData.InCycle = IC;

    while (LData.variables.size () > offset)
    {
        OUT ("FGT %s", Tree.GetNode (LData.variables.back ()).Data.GetName ());
        LData.variables.pop_back ();
    }
}

void NewFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
    newNodeData Data = Tree.Get ();
    int CN = Tree.CurrentNode();

    OUT_PREP
    OUT_ ("HOW IZ I %s ", Data.GetName());

    if (Tree.CanDownL ())
    {
        Tree.DownL ();

        PopParams (To, Tree, Info, NodeInfo, LData);

        Tree.Up ();
    }

    OUT_ ("\n");

    LData.offset ++;

    Tree.DownR ();

    CreateRec (To, Tree, Info, NodeInfo, LData);

    Tree.Up ();

    LData.offset --;

    OUT ("IF U CAN SO");

    if (strcmp (Data.GetName (), "main") == 0)
    {
        OUT ("I IZ main MKAY");
    }
}

void CallFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData)
{
   newNodeData Data = Tree.Get ();

    OUT_ ("I IZ %s ", Data.GetName());

    if (Tree.CanDownL ())
    {
        Tree.DownL ();

        int i = 0;
        PushParams (To, Tree, Info, NodeInfo, LData, i);

        Tree.Up ();
    }

    OUT_ (" MKAY");
}

void PushParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData, int& i)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NUM || Data.Descriptor == N_VAR || (Data.Descriptor == N_ARR && Tree.CanDownL ()) || Data.Descriptor == N_FUN || Data.Descriptor == N_SUM || Data.Descriptor == N_SUB || Data.Descriptor == N_MUL || Data.Descriptor == N_DIV || Data.Descriptor == N_MOD)
    {
        if (i > 0) OUT (" AN ");

        OUT_ ("YR ");

        PushInstance (To, Tree, Info, NodeInfo, LData);

        i ++;

        return ;
    }

    if (Data.Descriptor == N_ARR)
    {
        //NOT SUPPURTED

        return ;
    }

    if (Data.Descriptor == N_LNK)
    {
        OUT ("NOT SUPPORTED, SHIT!");

        i ++;

        return ;
    }

    if (Data.Descriptor == N_NODE && !Tree.CanDownL () && Tree.CanDownR ())
    {
        //ARRS NOT SUPPORTED

        i ++;

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PushParams (To, Tree, Info, NodeInfo, LData, i);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PushParams (To, Tree, Info, NodeInfo, LData, i);
        Tree.Up ();
    }
}

void PopParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToLolcodeData& LData, bool first)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR)
    {
        if (!first) OUT_ (" AN ");

        OUT_ ("YR %s", Data.GetName());

        first = false;
    }
    if (Data.Descriptor == N_LNK)
    {
        OUT ("NOT SUPPORTED, SHIT!");
    }
    if (Data.Descriptor == N_ARR)
    {
        //NOT SUPPORTED
    }

    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PopParams (To, Tree, Info, NodeInfo, LData, first);
        Tree.Up ();
    }
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PopParams (To, Tree, Info, NodeInfo, LData, first);
        Tree.Up ();
    }
}

#endif
