#ifndef INFOMAKER_H_
#define INFOMAKER_H_

#include "cstdio"
#include "..\Lib\Tree.h"
#include "..\Lib\BaseTreeDesc.h"
#include "..\Lib\TreeOperations.h"
#include "..\..\_SystemLibs\Stack\Stack.h"
#include "Counter.h"

#define MAX(a, b) ((a) > (b))? (a) : (b)

struct newTreeInfo
{
    int VarNumber;
    int VarMemory;

    int ArrNumber;
    int ArrMemory;

    bool UseNatives;
    int FuncNumber;

    int MarkNumber;
};

struct newNodeInfo
{
    int ID;
    int Addr;
};

//------------------------------------------------------------------------------

bool CreateInfoFile (newTree& Tree, FILE* File, FILE* ErrorOutput);
void ReadInfoFile   (newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, FILE* File);

//------------------------------------------------------------------------------

bool Link (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, FILE* ErrorOutput);

//------------------------------------------------------------------------------

struct newVarMID //MakeInfoData
{
    newCounter Counter;

    newVector <int> IDs;
};
struct newArrMID //MakeInfoData
{
    newCounter Counter;

    newVector <int> IDs;
};
struct newFunMID //MakeInfoData
{
    newCounter Counter;

    bool InFunc;
    bool InParams;

    newVector <newTree> Params;
    newVector <int> Headers;
};
struct newMarMID //MakeInfoData
{
    newCounter Counter;
};

//------------------------------------------------------------------------------

void LinkNode (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID);

void LinkVarArrLnk (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID);

void LinkMark (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID);

void LinkFunc (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID);

bool FuncParamsComp (newTree& Params1, newTree& Params2, newVector <newNodeInfo>& NodeInfo);

void OptimizeParamTree (newTree& Params);

//------------------------------------------------------------------------------

void PostLinkMarkFunc (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID);

//------------------------------------------------------------------------------

bool CreateInfoFile (newTree& Tree, FILE* File, FILE* ErrorOutput)
{
    newTreeInfo Info;
    newVector <newNodeInfo> NodeInfo;

    if (!Link (Tree, Info, NodeInfo, ErrorOutput)) return false;

    try
    {
        int Version = TREE_VERSION;
        if (fwrite (&Version, sizeof (int), 1, File) != 1) throw TH_ERROR "Can't write");

        if (fwrite (&Info,    sizeof (newTreeInfo), 1, File) != 1) throw TH_ERROR "Can't write");

        int Size = Tree.Size ();
        if (fwrite (&Size,    sizeof (int), 1, File) != 1) throw TH_ERROR "Can't write");

        for (int i = 0; i < Size; i ++)
        {
            if (fwrite (&NodeInfo [i], sizeof (newNodeInfo), 1, File) != 1) throw TH_ERROR "Can't write");
        }
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "%s", Error.ErrorText_);

        return false;
    }
    catch (...)
    {
        fprintf (ErrorOutput, "I don't know, what they said!");

        return false;
    }


    return true;
}

void ReadInfoFile   (newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, FILE* File)
{
    int Version = TREE_VERSION;
    if (fread (&Version, sizeof (int), 1, File) != 1) throw TH_ERROR "Can't read");
    if (Version != TREE_VERSION)                      throw TH_ERROR "Uncompatible versions");

    if (fread (&Info,    sizeof (newTreeInfo), 1, File) != 1) throw TH_ERROR "Can't read");

    int Size;
    if (fread (&Size,    sizeof (int), 1, File) != 1) throw TH_ERROR "Can't read");

    for (int i = 0; i < Size; i ++)
    {
        if (fread (&NodeInfo [i], sizeof (newNodeInfo), 1, File) != 1) throw TH_ERROR "Can't read");
    }

}

//------------------------------------------------------------------------------

bool Link (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, FILE* ErrorOutput)
{
    newVarMID VarMID;
    newArrMID ArrMID;
    newFunMID FunMID;
    FunMID.InFunc   = false;
    FunMID.InParams = false;
    newMarMID MarMID;

    Info.VarNumber      = 0;
    Info.VarMemory      = 0;

    Info.ArrNumber      = 0;
    Info.ArrMemory      = 0;

    Info.UseNatives     = false;
    Info.FuncNumber     = 0;

    Info.MarkNumber     = 0;

    for (int i = 0; i < Tree.Size (); i ++)
    {
        NodeInfo [i].ID   = -1;
        NodeInfo [i].Addr = -1;
    }

    int State = 0;

    try
    {
        LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);

        PostLinkMarkFunc (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "%s", Error.ErrorText_);

        return false;
    }
    catch (...)
    {
        fprintf (ErrorOutput, "I don't know, what they said!");

        return false;
    }

    return true;
}

//------------------------------------------------------------------------------

void LinkNode (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID)
{
    //printf ("LinkNode %d, State = %d\n", Tree.CurrentNode (), State);
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NEW)
    {
        State = 1;
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = 1;
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = 0;

        return;
    }
    if (Data.Descriptor == N_DEL)
    {
        if (FunMID.InFunc == false) throw TH_ERROR "Can't delete global values");

        State = 2;
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = 2;
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = 0;

        return;
    }
    if (Data.Descriptor == N_NATIVE)
    {
        Info.UseNatives = true;

        State = 3;
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = 3;
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = 0;

        return;
    }

    if (Data.Descriptor == N_NODE)
    {
        int StateCopy = State;
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        State = StateCopy;
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        return;
    }

    if (Data.Descriptor == N_VAR || Data.Descriptor == N_ARR || Data.Descriptor == N_LNK)
    {
        LinkVarArrLnk (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);

        State = 0;

        return;
    }
    if (Data.Descriptor == N_MARK || Data.Descriptor == N_GOTO)
    {
        LinkMark (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);

        return;
    }
    if (Data.Descriptor == N_FUN)
    {
        LinkFunc (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);

        State = 0;

        return;
    }

    if (Data.Descriptor == N_WHILE || Data.Descriptor == N_FOR || Data.Descriptor == N_IF)
    {
        VarMID.Counter.PushMemory ();
        ArrMID.Counter.PushMemory ();
        FunMID.Counter.PushMemory ();

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        VarMID.Counter.PopMemory ();
        ArrMID.Counter.PopMemory ();
        FunMID.Counter.PopMemory ();

        return ;
    }

    if (Data.Descriptor == N_ELSE)
    {
        VarMID.Counter.PushMemory ();
        ArrMID.Counter.PushMemory ();
        FunMID.Counter.PushMemory ();

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        VarMID.Counter.PopMemory ();
        ArrMID.Counter.PopMemory ();
        FunMID.Counter.PopMemory ();

        VarMID.Counter.PushMemory ();
        ArrMID.Counter.PushMemory ();
        FunMID.Counter.PushMemory ();

        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        VarMID.Counter.PopMemory ();
        ArrMID.Counter.PopMemory ();
        FunMID.Counter.PopMemory ();

        return ;
    }

    if (Data.Descriptor == N_GET)
    {
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
        Tree.Up ();
    }
}

void LinkVarArrLnk (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID)
{
    newNodeData Data = Tree.Get ();

    if (State == 1)
    {
        if (Data.Descriptor == N_LNK && FunMID.InParams == false) throw TH_ERROR "You can use links only in functions!");

        if (Data.Descriptor == N_VAR) if (VarMID.Counter.Find (Data.Name) != -1) throw TH_ERROR "This var name was already used(%s)!", Data.GetName ());
        if (Data.Descriptor == N_ARR) if (ArrMID.Counter.Find (Data.Name) != -1) throw TH_ERROR "This arr name was already used(%s)!", Data.GetName ());
        if (Data.Descriptor == N_LNK) if (VarMID.Counter.Find (Data.Name) != -1) throw TH_ERROR "This var name was already used(%s)!", Data.GetName ());

        if (Data.Descriptor == N_VAR) NodeInfo [Tree.CurrentNode ()].Addr = VarMID.Counter.Add (Data.Name);
        if (Data.Descriptor == N_ARR) NodeInfo [Tree.CurrentNode ()].Addr = ArrMID.Counter.Add (Data.Name);
        if (Data.Descriptor == N_LNK) NodeInfo [Tree.CurrentNode ()].Addr = VarMID.Counter.Add (Data.Name);

        if (Data.Descriptor == N_VAR) NodeInfo [Tree.CurrentNode ()].ID = Info.VarNumber;
        if (Data.Descriptor == N_ARR) NodeInfo [Tree.CurrentNode ()].ID = Info.ArrNumber;
        if (Data.Descriptor == N_LNK) NodeInfo [Tree.CurrentNode ()].ID = Info.VarNumber;

        if (Data.Descriptor == N_VAR) VarMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr] = Info.VarNumber;
        if (Data.Descriptor == N_ARR) ArrMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr] = Info.ArrNumber;
        if (Data.Descriptor == N_LNK) VarMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr] = Info.VarNumber;

        if (Data.Descriptor == N_VAR) Info.VarNumber ++;
        if (Data.Descriptor == N_ARR) Info.ArrNumber ++;
        if (Data.Descriptor == N_LNK) Info.VarNumber ++;

        State = 0;

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        Info.VarMemory = MAX(Info.VarMemory, VarMID.Counter.Size ());
        Info.ArrMemory = MAX(Info.ArrMemory, ArrMID.Counter.Size ());

        return;
    }
    if (State == 2)
    {
        if (Data.Descriptor == N_VAR) if (VarMID.Counter.Find (Data.Name) == -1) throw TH_ERROR "Can't delete unknown var name!");
        if (Data.Descriptor == N_ARR) if (ArrMID.Counter.Find (Data.Name) == -1) throw TH_ERROR "Can't delete unknown arr name!");

        if (Data.Descriptor == N_VAR) if (VarMID.Counter.LevelRem (Data.Name) > 0) throw TH_ERROR "Can't delete var, created in the other block!");
        if (Data.Descriptor == N_ARR) if (ArrMID.Counter.LevelRem (Data.Name) > 0) throw TH_ERROR "Can't delete arr, created in the other block!");

        if (Data.Descriptor == N_VAR) NodeInfo [Tree.CurrentNode ()].Addr = VarMID.Counter.Find (Data.Name);
        if (Data.Descriptor == N_ARR) NodeInfo [Tree.CurrentNode ()].Addr = ArrMID.Counter.Find (Data.Name);

        if (Data.Descriptor == N_VAR) NodeInfo [Tree.CurrentNode ()].ID = VarMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr];
        if (Data.Descriptor == N_ARR) NodeInfo [Tree.CurrentNode ()].ID = ArrMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr];

        if (Data.Descriptor == N_VAR) VarMID.Counter.Rem (NodeInfo [Tree.CurrentNode ()].Addr);
        if (Data.Descriptor == N_ARR) ArrMID.Counter.Rem (NodeInfo [Tree.CurrentNode ()].Addr);

        Info.VarMemory = MAX(Info.VarMemory, VarMID.Counter.Size ());
        Info.ArrMemory = MAX(Info.ArrMemory, ArrMID.Counter.Size ());

        return;
    }
    if (State == 0)
    {
        if (Data.Descriptor == N_LNK)
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        if (Data.Descriptor == N_VAR) if (VarMID.Counter.Find (Data.Name) == -1) throw TH_ERROR "Unknown var name(%s)!", Data.GetName ());
        if (Data.Descriptor == N_ARR) if (ArrMID.Counter.Find (Data.Name) == -1) throw TH_ERROR "Unknown arr name(%s)!", Data.GetName ());

        if (Data.Descriptor == N_VAR) NodeInfo [Tree.CurrentNode ()].Addr = VarMID.Counter.Find (Data.Name);
        if (Data.Descriptor == N_ARR) NodeInfo [Tree.CurrentNode ()].Addr = ArrMID.Counter.Find (Data.Name);

        if (Data.Descriptor == N_VAR) NodeInfo [Tree.CurrentNode ()].ID = VarMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr];
        if (Data.Descriptor == N_ARR) NodeInfo [Tree.CurrentNode ()].ID = ArrMID.IDs [NodeInfo [Tree.CurrentNode ()].Addr];

        State = 0;

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
            Tree.Up ();
        }

        Info.VarMemory = MAX(Info.VarMemory, VarMID.Counter.Size ());
        Info.ArrMemory = MAX(Info.ArrMemory, ArrMID.Counter.Size ());

        return;
    }
}

void LinkMark (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_MARK)
    {
        Info.MarkNumber ++;

        if (MarMID.Counter.Find (Data.Name) != -1) throw TH_ERROR "This mark name was already used(%s)!", Data.GetName ());

        NodeInfo [Tree.CurrentNode ()].ID = MarMID.Counter.Add (Data.Name);
    }
}

void LinkFunc (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID)
{
    newNodeData Data = Tree.Get ();

    if (State == 1 || State == 3)
    {
        printf ("Add %s\n", Data.GetName ());

        if (State == 1 && FunMID.InFunc == true) throw TH_ERROR "Don't try to create func in func (%s)", Data.GetName ());

        if (State == 1 && !Tree.CanDownR ())
        {
            if (FunMID.Counter.Find (Data.Name) != -1) throw TH_ERROR "This func name was already used(%s)!", Data.GetName ());

            int ID = FunMID.Counter.Add (Data.Name);

            NodeInfo [Tree.CurrentNode ()].ID = ID;
            FunMID.Headers [ID] = -1;

            if (Tree.CanDownL ())
            {
                Tree.DownL ();
                CopyTreeToTree (Tree, FunMID.Params [ID]);
                OptimizeParamTree (FunMID.Params [ID]);
                Tree.Up ();
            }
        }
        else
        {
            Info.FuncNumber ++;

            int ID = FunMID.Counter.Find (Data.Name);
            if (ID == -1)
            {
                ID = FunMID.Counter.Add (Data.Name);

                NodeInfo [Tree.CurrentNode ()].ID = ID;

                if (State == 1) FunMID.Headers [ID] = Tree.CurrentNode ();
                else            FunMID.Headers [ID] = -2;

                if (Tree.CanDownL ())
                {
                    Tree.DownL ();
                    CopyTreeToTree (Tree, FunMID.Params [ID]);
                    OptimizeParamTree (FunMID.Params [ID]);
                    Tree.Up ();
                }

            }
            else
            {
                NodeInfo [Tree.CurrentNode ()].ID = ID;

                if (State == 1) FunMID.Headers [ID] = Tree.CurrentNode ();
                else            FunMID.Headers [ID] = -2;

                if (Tree.CanDownL ())
                {
                    Tree.DownL ();
                    newTree Params;
                    CopyTreeToTree (Tree, Params);
                    OptimizeParamTree (Params);
                    if (!FuncParamsComp (FunMID.Params [ID], Params, NodeInfo))  throw TH_ERROR "Function (%s) must have simular params!", Data.GetName ());
                    Tree.Up ();
                }

            }

            int StateSave = State;

            VarMID.Counter.PushMemory ();
            ArrMID.Counter.PushMemory ();
            FunMID.Counter.PushMemory ();

            State = 0;
            FunMID.InParams = true;

            if (Tree.CanDownL ())
            {
                Tree.DownL ();
                LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
                Tree.Up ();
            }

            FunMID.InParams = false;

            if (StateSave == 1)
            {
                FunMID.InFunc = true;

                VarMID.Counter.PushMemory ();
                ArrMID.Counter.PushMemory ();
                FunMID.Counter.PushMemory ();

                Tree.DownR ();
                LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);
                Tree.Up ();

                FunMID.InFunc = false;

                VarMID.Counter.PopMemory ();
                ArrMID.Counter.PopMemory ();
                FunMID.Counter.PopMemory ();
            }

            VarMID.Counter.PopMemory ();
            ArrMID.Counter.PopMemory ();
            FunMID.Counter.PopMemory ();
        }

        return;
    }
    if (State == 2)
    {
        throw TH_ERROR "You can't delete functions");
    }
    if (State == 0)
    {
        if (FunMID.Counter.Find (Data.Name) == -1) throw TH_ERROR "Unknown func name(%s)!", Data.GetName ());

        NodeInfo [Tree.CurrentNode ()].ID = FunMID.Counter.Find (Data.Name);

        int ID = NodeInfo [Tree.CurrentNode ()].ID;

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            if (!FuncParamsComp (FunMID.Params [ID], Tree, NodeInfo))  throw TH_ERROR "Function (%s) must have simular params!", Data.GetName ());

            LinkNode (Tree, Info, NodeInfo, State, VarMID, ArrMID, FunMID, MarMID);

            Tree.Up ();
        }
        else
        {
            if (!FunMID.Params [ID].Empty ()) throw TH_ERROR "Function (%s) must have simular params!", Data.GetName ());
        }

        return;
    }
}

bool FuncParamsComp (newTree& Params1, newTree& Params2, newVector <newNodeInfo>& NodeInfo)
{
    newNodeData Data1 = Params1.Get (), Data2 = Params2.Get ();

    if (Data1.Descriptor == N_VAR)
    {
        if (Data2.Descriptor == N_ARR || Data2.Descriptor == N_VAR || Data2.Descriptor == N_NUM || Data2.Descriptor == N_FUN || Data2.Descriptor == N_SUM || Data2.Descriptor == N_SUB || Data2.Descriptor == N_MUL || Data2.Descriptor == N_DIV || Data2.Descriptor == N_MOD) return true;
        else                                                                                                                                                                                                                                                                   return false;
    }
    if (Data1.Descriptor == N_ARR)
    {
        if (Data2.Descriptor == N_ARR || Data2.Descriptor == N_NODE) return true;
        else                            return false;
    }
    if (Data1.Descriptor == N_LNK)
    {
        if (Data2.Descriptor == N_LNK) return true;
        else                           return false;
    }

    if (Params1.CanDownL ())
    {
        if (Params2.CanDownL ())
        {
            Params1.DownL ();
            Params2.DownL ();

            if (!FuncParamsComp (Params1, Params2, NodeInfo))
            {
                Params1.Up ();
                Params2.Up ();

                return false;
            }
            else
            {
                Params1.Up ();
                Params2.Up ();
            }
        }
        else
        {
            return false;
        }
    }
    if (Params1.CanDownR ())
    {
        if (Params2.CanDownR ())
        {
            Params1.DownR ();
            Params2.DownR ();

            if (!FuncParamsComp (Params1, Params2, NodeInfo))
            {
                Params1.Up ();
                Params2.Up ();

                return false;
            }
            else
            {
                Params1.Up ();
                Params2.Up ();
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

void OptimizeParamTree (newTree& Params)
{
    Params.PushP ();

    bool Change = true;

    while (Change)
    {
        Change = false;
        for (int i = 0; i < Params.Size (); i ++)
        {
            Params.CurrentNode (i);

            if (Params.Get ().Descriptor == N_NEW)
            {
                Params.DownL ();
                UpBranch (Params);

                Change = true;
            }
        }
    }

    Params.PopP ();
}

//------------------------------------------------------------------------------

void PostLinkMarkFunc (newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, int& State, newVarMID& VarMID, newArrMID& ArrMID, newFunMID& FunMID, newMarMID& MarMID)
{
    for (int i = 0; i < Tree.Size (); i ++)
    {
        Tree.CurrentNode (i);

        newNodeData Data = Tree.Get ();

        if (Data.Descriptor == N_GOTO)
        {
            if (MarMID.Counter.Find (Data.Name) == -1) throw TH_ERROR "Unknown mark name(%s)!", Data.GetName ());

            NodeInfo [i].ID = MarMID.Counter.Find (Data.Name);
        }

        if (Data.Descriptor == N_FUN)
        {
            if (FunMID.Headers [NodeInfo [i].ID] == -1) throw TH_ERROR "Only prototype for (%s)!", Data.GetName ());

            NodeInfo [i].Addr = FunMID.Headers [NodeInfo [i].ID];
        }
    }
}

#endif
