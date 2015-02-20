#ifndef TREE_TO_MYASM_H
#define TREE_TO_MYASM_H

#include "stdio.h"
#include "..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "..\..\SCC_Tree\Lib\TreeOperations.h"
#include "..\..\SCC_Tree\InfoMaker\InfoMaker.h"
#include "AsmMemoryCounter.h"

#define MEMORY_OFFSET_ 1

struct newTreeToMyAsmData
{
    int CurrentMark;

    int InCycle;

    int InFunction;

    int ArrsToDelete;

    newAsmMemoryCounter MC;

    bool UseInspector;
};

void CreateAsm    (FILE* To, newTree& Tree, newTreeInfo Info, newVector <newNodeInfo> NodeInfo);
void CreateAsmRec (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

//------------------------------------------------------------------------------

void PushInstance (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void New (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);
void PushArr (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData, newNodeInfo& ArrInfo, int i = 0);

void Equal (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);
void SelfEqual (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void Echo (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);
void Get (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void If (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void While (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);
void For (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void NewFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);
void CallFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void PushParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData, int& i);
void PopParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData);

void PushParamArr (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData, int i = 0);

//------------------------------------------------------------------------------

void CreateAsm (FILE* To, newTree& Tree, newTreeInfo Info, newVector <newNodeInfo> NodeInfo, bool UseInspector)
{
    newTreeToMyAsmData AsmData;

    AsmData.CurrentMark = Info.FuncNumber * 2 + Info.MarkNumber;

    AsmData.InCycle = -1;

    AsmData.InFunction = -1;

    AsmData.ArrsToDelete = 0;

    AsmData.UseInspector = UseInspector;

    fprintf (To, "push %d //Size\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * Info.ArrMemory);
    fprintf (To, "add //Reserve memory for addreses\n");

    AsmData.MC.PushMemory ();

    CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);

    AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);

    fprintf (To, ": -1 //End of programm! \n");

    fprintf (To, "push %d //Size\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * Info.ArrMemory);
    fprintf (To, "swap //Addr must be first\n");
    fprintf (To, "rem //Free memory for addreses");
}

void CreateAsmRec (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("Asm Node %d\n", Tree.CurrentNode ());

    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NEW)
    {
        New (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }
    if (Data.Descriptor == N_DEL)
    {
        if (AsmData.InFunction != -1)
        {
            Tree.DownL ();

            if (Tree.Get ().Descriptor == N_VAR) AsmData.MC.Rem (MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr, To, MEMORY_OFFSET_ + Info.VarMemory);
            if (Tree.Get ().Descriptor == N_ARR) AsmData.MC.Rem (MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, To, MEMORY_OFFSET_ + Info.VarMemory);

            Tree.Up ();
        }

        return ;
    }
    if (Data.Descriptor == N_NATIVE)
    {
        New (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Data.Descriptor == N_EQ)
    {
        Equal (To, Tree, Info, NodeInfo, AsmData);

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
        SelfEqual (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Data.Descriptor == N_ECHO)
    {
        Echo (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }
    if (Data.Descriptor == N_GET)
    {
        Get (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Data.Descriptor == N_IF)
    {
        If (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Data.Descriptor == N_BREAK && AsmData.InCycle != -1)
    {
        fprintf (To, "goto %d //break\n", AsmData.InCycle + 1);

        return ;
    }
    if (Data.Descriptor == N_CONTINUE && AsmData.InCycle != -1)
    {
        fprintf (To, "goto %d //continue\n", AsmData.InCycle);

        return ;
    }

    if (Data.Descriptor == N_RETURN && AsmData.InFunction != -1)
    {
        fprintf (To, "//Return\n");
        fprintf (To, "//{\n");

        Tree.DownL ();

        PushInstance (To, Tree, Info, NodeInfo, AsmData);

        fprintf (To, "popm 0\n");
        fprintf (To, "\n");
        fprintf (To, "//Pop function memory\n");
        fprintf (To, "//{\n");
        AsmData.MC.PrintfPopFunctionMemory (To, MEMORY_OFFSET_ + Info.VarMemory, AsmData.InFunction / 2);
        fprintf (To, "//}\n");
        fprintf (To, "\n");
        fprintf (To, "end\n");

        Tree.Up ();

        fprintf (To, "//}\n");

        return ;
    }

    if (Data.Descriptor == N_WHILE)
    {
        While (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }
    if (Data.Descriptor == N_FOR)
    {
        For (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        CallFunc (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Data.Descriptor == N_GOTO)
    {
        fprintf (To, "goto %d //%s\n", Info.FuncNumber * 2 + NodeInfo[Tree.CurrentNode ()].ID, Data.GetName ());

        return ;
    }
    if (Data.Descriptor == N_MARK)
    {
        fprintf (To, ": %d //%s\n", Info.FuncNumber * 2 + NodeInfo[Tree.CurrentNode ()].ID, Data.GetName ());

        return ;
    }

    if (Tree.CanDownL())
    {
        Tree.DownL ();
        CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
    if (Tree.CanDownR())
    {
        Tree.DownR ();
        CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
}

//------------------------------------------------------------------------------

void PushInstance (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("PushInstance\n");
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_TRUE)
    {
        fprintf (To, "push 1\n");

        return;
    }
    if (Data.Descriptor == N_FALSE)
    {
        fprintf (To, "push 0\n");

        return;
    }

    if (Data.Descriptor == N_NUM)
    {
        fprintf (To, "push %g\n", Data.Data);

        return;
    }

    if (Data.Descriptor == N_VAR)
    {
        fprintf (To, "pushm %d //Address %s\n", MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());
        fprintf (To, "pushms\n");
        fprintf (To, "\n");

        if (AsmData.UseInspector) fprintf (To, "log_get_var %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Data.GetName ());

        return;
    }

    if (Data.Descriptor == N_ARR)
    {
        fprintf (To, "//Push arr cell\n");
        fprintf (To, "//{\n");
        fprintf (To, "pushm %d //Adress %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());
        fprintf (To, "//%s index\n", Data.GetName ());

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up  ();

        fprintf (To, "sum\n");
        fprintf (To, "pushms //%s\n", Data.GetName ());
        fprintf (To, "//}\n");
        fprintf (To, "\n");

        if (AsmData.UseInspector) fprintf (To, "log_get_arr %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Data.GetName ());

        return;
    }

    if (Data.Descriptor == N_FUN)
    {
        CallFunc (To, Tree, Info, NodeInfo, AsmData);
        fprintf (To, "pushm 0 //return value\n");

        return;
    }

    if (Data.Descriptor == N_AND)
    {
        fprintf (To, "//AND\n");
        fprintf (To, "//{\n");

        int CM = AsmData.CurrentMark;
        AsmData.CurrentMark ++;

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "dup\n");
        fprintf (To, "ifnot %d\n", CM);

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "mul\n");

        fprintf (To, ": %d\n", CM);
        fprintf (To, "//}\n");
        return ;
    }
    if (Data.Descriptor == N_OR)
    {
        fprintf (To, "//OR\n");
        fprintf (To, "//{\n");

        int CM = AsmData.CurrentMark;
        AsmData.CurrentMark ++;

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "dup\n");
        fprintf (To, "not\n");
        fprintf (To, "ifnot %d\n", CM);

        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "sum\n");

        fprintf (To, "push 0\n");
        fprintf (To, "less\n");
        fprintf (To, ": %d", CM);
        fprintf (To, "//}\n");
        return ;
    }

    if (Data.Descriptor == N_NOT)
    {
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "not\n");

        return ;
    }
    if (Data.Descriptor == N_OPEQ)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "equal\n");

        return ;
    }
    if (Data.Descriptor == N_NONEQ)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "equal\n");
        fprintf (To, "not\n");

        return ;
    }
    if (Data.Descriptor == N_MORE)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "more\n");

        return ;
    }
    if (Data.Descriptor == N_LESS)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "less\n");

        return ;
    }
    if (Data.Descriptor == N_MREQ)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "less\n");

        fprintf (To, "not\n");

        return ;
    }
    if (Data.Descriptor == N_LSEQ)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "more\n");

        fprintf (To, "not\n");

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }

    if (Data.Descriptor == N_SUM)
    {
        fprintf (To, "sum\n");

        return ;
    }
    if (Data.Descriptor == N_SUB)
    {
        fprintf (To, "swap\n");
        fprintf (To, "sub\n");

        return ;
    }
    if (Data.Descriptor == N_MUL)
    {
        fprintf (To, "mul\n");

        return ;
    }
    if (Data.Descriptor == N_DIV)
    {
        fprintf (To, "swap\n");
        fprintf (To, "div\n");

        return ;
    }
    if (Data.Descriptor == N_MOD)
    {
        fprintf (To, "swap\n");
        fprintf (To, "mod\n");

        return ;
    }
}

void New (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("New\n");
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR)
    {
        fprintf (To, "//New variable %s\n", Data.GetName ());
        fprintf (To, "//{\n");

        fprintf (To, "push 1 //Size of %s\n", Data.GetName ());
        fprintf (To, "add //Reserve memory\n");
        fprintf (To, "popm %d //Push Addr to %s's cell\n", MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());

        AsmData.MC.Add (MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr);

        if (Tree.CanDownR ())
        {
            fprintf (To, "\n");
            fprintf (To, "//Push value\n");
            fprintf (To, "//{\n");
            Tree.DownR ();
            PushInstance (To, Tree, Info, NodeInfo, AsmData);
            Tree.Up ();

            fprintf (To, "\n");
            fprintf (To, "pushm %d //Address of %s\n", MEMORY_OFFSET_ + NodeInfo [Tree.CurrentNode ()].Addr, Data.GetName ());
            fprintf (To, "popms\n");

            //if (AsmData.UseInspector) fprintf (To, "log_set_var %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());

            fprintf (To, "//}\n");
        }

        fprintf (To, "//}\n");
        fprintf (To, "\n");

        return ;
    }
    if (Data.Descriptor == N_ARR)
    {
        fprintf (To, "//New arr %s\n", Data.GetName ());
        fprintf (To, "//{\n");

        int Size = 0;

        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            Tree.DownR ();
            Size = GetListSize (Tree);
            Tree.Up ();
            Tree.Up ();
        }

        fprintf (To, "//Push size\n");
        fprintf (To, "//{\n");

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            PushInstance (To, Tree, Info, NodeInfo, AsmData);
            Tree.Up ();
        }
        else fprintf (To, "push %d\n", Size);
        fprintf (To, "//}\n");

        fprintf (To, "dup\n");
        fprintf (To, "popm %d //Push size\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr + 1);
        fprintf (To, "\n");

        fprintf (To, "add //Reserve memory\n");
        fprintf (To, "popm %d //Push Addr to %s's cell\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());

        AsmData.MC.Add (MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr);

        newNodeInfo& ArrInfo = NodeInfo [Tree.CurrentNode ()];

        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            Tree.DownR ();
            PushArr (To, Tree, Info, NodeInfo, AsmData, ArrInfo);
            Tree.Up ();
            Tree.Up ();
        }

        fprintf (To, "//}\n");
        fprintf (To, "\n");

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        if (Tree.CanDownR ()) NewFunc (To, Tree, Info, NodeInfo, AsmData);

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        New (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        New (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
}

void PushArr (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData, newNodeInfo& ArrInfo, int i)
{
    //printf ("PushArr\n");
    if (Tree.Get ().Descriptor != N_NODE)
    {
        fprintf (To, "//Push arr cell %d\n", i);
        fprintf (To, "//{\n");

        PushInstance (To, Tree, Info, NodeInfo, AsmData);

        fprintf (To, "\n");

        fprintf (To, "pushm %d\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * ArrInfo.Addr);
        fprintf (To, "push  %d //i\n", i);
        fprintf (To, "sum\n");
        fprintf (To, "popms\n");

        if (AsmData.UseInspector) fprintf (To, "log_set_arr %d //ID of arr\n", ArrInfo.ID);

        fprintf (To, "//}\n");
        fprintf (To, "\n");

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PushArr (To, Tree, Info, NodeInfo, AsmData, ArrInfo, i);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PushArr (To, Tree, Info, NodeInfo, AsmData, ArrInfo, i + 1);
        Tree.Up ();
    }
}

void Equal (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("Equal\n");
    fprintf (To, "//Equal\n");
    fprintf (To, "//{\n");

    fprintf (To, "//What\n");
    Tree.DownR ();
    PushInstance (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();

    fprintf (To, "//Whoom\n");

    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        fprintf (To, "pushm %d //Adress of %s\n", MEMORY_OFFSET_ + NodeInfo [Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
        fprintf (To, "popms\n");

        if (AsmData.UseInspector) fprintf (To, "log_set_var %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        fprintf (To, "//[]\n");
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "pushm %d //Adress of %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
        fprintf (To, "sum\n");

        fprintf (To, "popms\n");

        if (AsmData.UseInspector) fprintf (To, "log_set_arr %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());
    }

    Tree.Up ();

    fprintf (To, "//}\n");
}

void SelfEqual (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("Equal\n");
    fprintf (To, "//SelfEqual\n");
    fprintf (To, "//{\n");

    fprintf (To, "//Who\n");

    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        fprintf (To, "pushm %d //Adress of %s\n", MEMORY_OFFSET_ + NodeInfo [Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());

        if (AsmData.UseInspector) fprintf (To, "log_set_var %d //ID of %s (it will be set later)\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        fprintf (To, "//[]\n");
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "pushm %d //Adress of %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
        fprintf (To, "sum\n");

        if (AsmData.UseInspector) fprintf (To, "log_set_arr %d //ID of %s (it will be set later)\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());
    }

    Tree.Up ();

    fprintf (To, "dup\n");
    fprintf (To, "pushms //To make something with who\n");

    fprintf (To, "//What\n");
    if (Tree.Get ().Descriptor != N_INCR && Tree.Get ().Descriptor != N_DECR)
    {
        Tree.DownR ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
    else
    {
        fprintf (To, "push 1 //To increase/decrease by 1\n");
    }

    fprintf (To, "swap\n");

    if (Tree.Get ().Descriptor == N_INCR)   fprintf (To, "sum //Special ingredient\n");
    if (Tree.Get ().Descriptor == N_DECR)   fprintf (To, "sub //Special ingredient\n");
    if (Tree.Get ().Descriptor == N_SUM_EQ) fprintf (To, "sum //Special ingredient\n");
    if (Tree.Get ().Descriptor == N_SUB_EQ) fprintf (To, "sub //Special ingredient\n");
    if (Tree.Get ().Descriptor == N_MUL_EQ) fprintf (To, "mul //Special ingredient\n");
    if (Tree.Get ().Descriptor == N_DIV_EQ) fprintf (To, "div //Special ingredient\n");
    if (Tree.Get ().Descriptor == N_MOD_EQ) fprintf (To, "mod //Special ingredient\n");

    fprintf (To, "swap\n");
    fprintf (To, "popms //Done\n");

    fprintf (To, "//}\n");
}

void Echo (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("Echo\n");
    fprintf (To, "//echo\n");
    fprintf (To, "//{\n");

    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();

    fprintf (To, "\n");
    fprintf (To, "@\n");

    fprintf (To, "//}\n");
    fprintf (To, "\n");
}

void Get (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("Get\n");
    fprintf (To, "//Get\n");
    fprintf (To, "//{\n");

    fprintf (To, "&\n");
    fprintf (To, "\n");

    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        fprintf (To, "pushm %d //Adress of %s\n", MEMORY_OFFSET_ + NodeInfo [Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
        fprintf (To, "popms\n");

        if (AsmData.UseInspector) fprintf (To, "log_set_var %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        fprintf (To, "//[]\n");
        Tree.DownL ();
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();

        fprintf (To, "pushm %d //Adress of %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
        fprintf (To, "sum\n");

        fprintf (To, "popms\n");

        if (AsmData.UseInspector) fprintf (To, "log_set_arr %d //ID of %s\n", NodeInfo[Tree.CurrentNode ()].ID, Tree.Get ().GetName ());
    }

    Tree.Up ();

    fprintf (To, "//}\n");
}

void If (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("If\n");
    fprintf (To, "//If\n");
    fprintf (To, "//{\n");

    int IfM = AsmData.CurrentMark;

    AsmData.CurrentMark ++;

    fprintf (To, "//Equation\n");
    fprintf (To, "//{\n");
    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    fprintf (To, "//}\n");

    fprintf (To, "ifnot %d\n", IfM);

    Tree.DownR ();
    if (Tree.Get ().Descriptor == N_ELSE)
    {
        int CM = AsmData.CurrentMark;
        AsmData.CurrentMark ++;

        AsmData.MC.PushMemory ();
        Tree.DownL ();
        CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
        AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);
        fprintf (To, "goto %d //End\n", CM);

        fprintf (To, "//Else\n");
        fprintf (To, "//{\n");
        fprintf (To, ": %d\n", IfM);
        AsmData.MC.PushMemory ();
        Tree.DownR ();
        CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
        AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);
        fprintf (To, "//}\n");

        fprintf (To, ": %d //End\n", CM);
    }
    else
    {
        AsmData.MC.PushMemory ();
        CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
        AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);

        fprintf (To, ": %d //End\n", IfM);
    }

    Tree.Up ();

    fprintf (To, "//}\n");
}

void While (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("While\n");
    fprintf (To, "//While\n");
    fprintf (To, "//{\n");

    fprintf (To, ": %d\n", AsmData.CurrentMark);
    int IC = AsmData.InCycle;
    AsmData.InCycle = AsmData.CurrentMark;
    AsmData.CurrentMark += 2;

    fprintf (To, "//Equation\n");
    fprintf (To, "//{\n");
    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    fprintf (To, "//}\n");

    fprintf (To, "ifnot %d \n", AsmData.InCycle + 1);

    AsmData.MC.PushMemory ();
    Tree.DownR ();
    CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);

    fprintf (To, "goto %d\n", AsmData.InCycle);
    fprintf (To, ": %d\n", AsmData.InCycle + 1);

    AsmData.InCycle = IC;

    fprintf (To, "//}\n");
}

void For (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("For\n");
    fprintf (To, "//For\n");
    fprintf (To, "//{\n");

    AsmData.MC.PushMemory ();
    fprintf (To, "//Variables\n");
    fprintf (To, "//{\n");
    Tree.DownL ();
    Tree.DownL ();
    CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    Tree.Up ();
    fprintf (To, "//}\n");

    fprintf (To, "goto %d\n", AsmData.CurrentMark + 2);
    fprintf (To, ": %d\n", AsmData.CurrentMark);
    int IC = AsmData.InCycle;
    AsmData.InCycle = AsmData.CurrentMark;
    AsmData.CurrentMark += 3;

    fprintf (To, "//Dynamic\n");
    fprintf (To, "//{\n");
    Tree.DownL ();
    Tree.DownR ();
    CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    Tree.Up ();
    fprintf (To, "//}\n");

    fprintf (To, ": %d\n", AsmData.InCycle + 2);
    fprintf (To, "//Equation\n");
    fprintf (To, "//{\n");
    Tree.DownR ();
    Tree.DownL ();
    PushInstance (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    Tree.Up ();
    fprintf (To, "//}\n");

    fprintf (To, "ifnot %d \n", AsmData.InCycle + 1);

    AsmData.MC.PushMemory ();
    Tree.DownR ();
    Tree.DownR ();
    CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);
    Tree.Up ();
    Tree.Up ();
    AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);

    fprintf (To, "goto %d\n", AsmData.InCycle);
    fprintf (To, ": %d\n", AsmData.InCycle + 1);

    AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);

    AsmData.InCycle = IC;

    fprintf (To, "//}\n");
}

void NewFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("NewFunc\n");

    newNodeData Data = Tree.Get ();
    int CN = Tree.CurrentNode();

    fprintf (To, "//New func %s\n", Data.GetName ());
    fprintf (To, "//{\n");

    fprintf (To, "goto %d //End\n", NodeInfo[CN].ID * 2 + 1);
    fprintf (To, ": %d\n", NodeInfo[CN].ID * 2);

    AsmData.InFunction = NodeInfo[CN].ID * 2;

    AsmData.MC.AddFunc (NodeInfo[CN].ID);

    AsmData.MC.PushMemory ();

    if (Tree.CanDownL ())
    {
        Tree.DownL ();

        fprintf (To, "down %d //Down return pointer\n", GetListSize (Tree));

        fprintf (To, "//Pop function params\n");
        fprintf (To, "//{\n");

        PopParams (To, Tree, Info, NodeInfo, AsmData);

        fprintf (To, "//}\n");

        Tree.Up ();
    }

    Tree.DownR ();

    CreateAsmRec (To, Tree, Info, NodeInfo, AsmData);

    Tree.Up ();

    AsmData.MC.PopMemory (To, MEMORY_OFFSET_ + Info.VarMemory);

    AsmData.InFunction = -1;

    fprintf (To, "end\n");
    fprintf (To, ": %d //End\n", NodeInfo[CN].ID * 2 + 1);
    if (strcmp (Data.GetName (), "main") == 0)
    {
        fprintf (To, "//Main special\n");
        fprintf (To, "//{\n");
        fprintf (To, "call %d // Call main :)\n", NodeInfo[CN].ID * 2);
        fprintf (To, "\n");
        AsmData.MC.PrintfPopFunctionMemory (To, MEMORY_OFFSET_ + Info.VarMemory, -2); //Function = -2 mean pop to previous memory push
        fprintf (To, "\n");
        fprintf (To, "goto -1 //End of programm\n");
        fprintf (To, "//}\n");
    }
    fprintf (To, "//}\n");
}

void CallFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("CallFunc\n");
    newNodeData Data = Tree.Get ();

    fprintf (To, "//Call function %s\n", Data.GetName ());
    fprintf (To, "//{\n");

    printf ("GetName () = %s\n", Data.GetName ());

    AsmData.MC.PrintfPush (To, NodeInfo[Tree.CurrentNode()].ID);

    if (Tree.CanDownL ())
    {
        Tree.DownL ();

        fprintf (To, "//Push function params\n");
        fprintf (To, "//{\n");

        AsmData.ArrsToDelete = 0;
        int i = 0;
        PushParams (To, Tree, Info, NodeInfo, AsmData, i);

        fprintf (To, "//}\n");

        Tree.Up ();
    }

    if (NodeInfo[Tree.CurrentNode()].Addr == -2) fprintf (To, "//NATIVE CALL! Will cause error, if function not exist. Function GetName (): \"%s\".\n", Tree.Get ().GetName ());
    fprintf (To, "call %d\n", NodeInfo[Tree.CurrentNode()].ID * 2);

    if (AsmData.ArrsToDelete > 0)
    {
        while (AsmData.ArrsToDelete > 0)
        {
            fprintf (To, "rem //Delete temp arrs\n");

            AsmData.ArrsToDelete --;
        }
    }

    AsmData.MC.PrintfPop (To, NodeInfo[Tree.CurrentNode()].ID);

    fprintf (To, "//}\n");
}

void PushParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData, int& i)
{
    //printf ("PushParams\n");

    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NUM || Data.Descriptor == N_VAR || (Data.Descriptor == N_ARR && Tree.CanDownL ()) || Data.Descriptor == N_FUN || Data.Descriptor == N_SUM || Data.Descriptor == N_SUB || Data.Descriptor == N_MUL || Data.Descriptor == N_DIV || Data.Descriptor == N_MOD)
    {
        fprintf (To, "//Push instace\n");
        fprintf (To, "//{\n");
        PushInstance (To, Tree, Info, NodeInfo, AsmData);
        fprintf (To, "//}\n");

        i ++;

        return ;
    }

    if (Data.Descriptor == N_ARR)
    {
        fprintf (To, "pushm %d //Address of arr %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());

        i ++;

        return ;
    }

    if (Data.Descriptor == N_LNK)
    {
        Tree.DownL ();

        if (Tree.Get ().Descriptor == N_VAR)
        {
            fprintf (To, "pushm %d //Address of link(var) %s\n", MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());
        }
        else
        {
            fprintf (To, "//Push arr link\n");
            fprintf (To, "//{\n");
            fprintf (To, "pushm %d //Address of link(arr) %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());
            Tree.DownL ();
            fprintf (To, "//Push instace\n");
            fprintf (To, "//{\n");
            PushInstance (To, Tree, Info, NodeInfo, AsmData);
            fprintf (To, "//}\n");
            Tree.Up ();
            fprintf (To, "sum\n");
            fprintf (To, "//}\n");
        }

        Tree.Up ();

        i ++;

        return ;
    }

    if (Data.Descriptor == N_NODE && !Tree.CanDownL () && Tree.CanDownR ())
    {
        fprintf (To, "//Create temp arr\n");
        fprintf (To, "//{\n");

        AsmData.ArrsToDelete ++;

        Tree.DownR ();

        fprintf (To, "push %d //Push size\n", GetListSize (Tree));
        fprintf (To, "dup\n");
        fprintf (To, "\n");
        fprintf (To, "down %d //Down size to delete temp arr next\n", i + 1);
        fprintf (To, "\n");

        fprintf (To, "add //Reserve memory\n");
        fprintf (To, "dup\n");
        fprintf (To, "\n");
        fprintf (To, "down %d //Down addr to delete it next\n", i + 1);
        fprintf (To, "\n");

        PushParamArr (To, Tree, Info, NodeInfo, AsmData);

        fprintf (To, "//}\n");
        Tree.Up ();

        i ++;

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PushParams (To, Tree, Info, NodeInfo, AsmData, i);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PushParams (To, Tree, Info, NodeInfo, AsmData, i);
        Tree.Up ();
    }
}

void PopParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData)
{
    //printf ("PopParams\n");
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR)
    {
        fprintf (To, "push 1 //Size of %s\n", Data.GetName ());
        fprintf (To, "add //Reserve memory\n");

        fprintf (To, "popm %d //Push Addr to %s's cell\n", MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());

        AsmData.MC.Add (MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr);

        fprintf (To, "pushm %d //Push Addr to %s's cell\n", MEMORY_OFFSET_ + NodeInfo[Tree.CurrentNode ()].Addr, Data.GetName ());

        fprintf (To, "popms\n");
    }
    if (Data.Descriptor == N_LNK)
    {
        fprintf (To, "popm %d //Set adress of link %s\n", MEMORY_OFFSET_ + NodeInfo [Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
    }
    if (Data.Descriptor == N_ARR)
    {
        fprintf (To, "popm %d //Set adress of arr %s\n", MEMORY_OFFSET_ + Info.VarMemory + 2 * NodeInfo [Tree.CurrentNode ()].Addr, Tree.Get ().GetName ());
    }

    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PopParams (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PopParams (To, Tree, Info, NodeInfo, AsmData);
        Tree.Up ();
    }
}

void PushParamArr (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToMyAsmData& AsmData, int i)
{
    //printf ("PushParamArr\n");
    if (Tree.Get ().Descriptor != N_NODE)
    {
        fprintf (To, "//Push arr cell %d\n", i);
        fprintf (To, "//{\n");

        fprintf (To, "dup\n");

        PushInstance (To, Tree, Info, NodeInfo, AsmData);

        fprintf (To, "\n");

        fprintf (To, "swap\n");
        fprintf (To, "push  %d //i\n", i);
        fprintf (To, "sum\n");
        fprintf (To, "popms\n");

        fprintf (To, "//}\n");
        fprintf (To, "\n");

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        PushParamArr (To, Tree, Info, NodeInfo, AsmData, i);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        PushParamArr (To, Tree, Info, NodeInfo, AsmData, i + 1);
        Tree.Up ();
    }
}

#endif
