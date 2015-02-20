#ifndef TREE_TO_SHEKSPEAR_H
#define TREE_TO_SHEKSPEAR_H

#include "stdio.h"
#include "..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "..\..\SCC_Tree\Lib\TreeOperations.h"
#include "..\..\SCC_Tree\InfoMaker\InfoMaker.h"
#include "float.h"
#include "math.h"
#include "Counter.h"
#include "Help.h"

#define MEMORY_OFFSET_ 6
#define T1 0 // Temp1
#define T2 1 // Temp2
#define IA 2 // Instance assist
#define RE 3 // Return
//#define AI 4 // Arr i
#define LA 5 // Links assist

#define PUSH_VAR(A) MEMORY_OFFSET_ + A

#define PUSH_ARRA(A)  MEMORY_OFFSET_ + Info.VarMemory + 3 * (A) + 0
#define PUSH_ARRS1(A) MEMORY_OFFSET_ + Info.VarMemory + 3 * (A) + 1
#define PUSH_ARRS2(A) MEMORY_OFFSET_ + Info.VarMemory + 3 * (A) + 2

#define PUSH_CHARACTER(A) AddListItem ("character", ShData.shuffler.shuffle (A), To, ShData.Way)

struct newTreeToShekspearData;
#include "ShMemoryCounter.h"

struct newTreeToShekspearData
{
    int CurrentSpeaker;
    newIntCounter OnScene;

    newShMemoryCounter MC;

    bool InArrIndex;
    newVector <bool> ArrFuncComplete;

    int CycleStart;
    int CycleEnd;

    int Act;
    int Scene;

    bool UseScenes;

    string Way;

    Shuffler shuffler;
};

void CreateShekspear    (FILE* To, newTree& Tree, newTreeInfo Info, newVector <newNodeInfo> NodeInfo);
void CreateShekspearRec (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

//------------------------------------------------------------------------------

void OutRoman (FILE* To, int Num);

void AddVars (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void CreateScene (FILE* To, int A, int B, newTreeToShekspearData& ShData);
void CreateSpeech (FILE* To, int A, newTreeToShekspearData& ShData);
void CreateListener (FILE* To, int A, newTreeToShekspearData& ShData);
void CreatePlace (FILE* To, newTreeToShekspearData& ShData);

void AddScene (FILE* To, newTreeToShekspearData& ShData);
void AddSceneCall (FILE* To, int A, newTreeToShekspearData& ShData);

void AddAct (FILE* To, newTreeToShekspearData& ShData);
void AddActCall (FILE* To, int A, newTreeToShekspearData& ShData);

void AddGotoScene (FILE* To, int A, newTreeToShekspearData& ShData);

void PushNumber (FILE* To, double Num, newTreeToShekspearData& ShData);

void SetA (FILE* To, newTreeToShekspearData& ShData, int A, int SelfModyfier = -1);
void SaveA (FILE* To, newTreeToShekspearData& ShData, int A);
void LoadA (FILE* To, newTreeToShekspearData& ShData, int A);

bool LinearInstance (newTree& Tree);
void SetVar (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int A = -1, int PIA = IA, int SelfModyfier = -1);

void PushArrIFunc (FILE* To, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int A);

void New (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void Equal (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void Echo (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);
void Get  (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

int GetMarkNumber (newTree& Tree, int State = 0);

void PushEquation (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int A = T2);

void If (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void While (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);
void For (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void NewFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void CallFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);

void PushParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData);
void PopParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int FunctionN);

//------------------------------------------------------------------------------

void CreateShekspear (FILE* To, newTree& Tree, newTreeInfo Info, newVector <newNodeInfo> NodeInfo, string Way)
{
    newTreeToShekspearData ShData;
    ShData.CurrentSpeaker = -1;

    ShData.InArrIndex = false;

    ShData.CycleStart = -1;
    ShData.CycleEnd = -1;

    ShData.Act = 0;

    ShData.UseScenes = false;

    ShData.Way = Way;

    ShData.shuffler.max = ListSize ("character", Way);

    fprintf (To, "/*Put your name here!!!!*/\n");
    fprintf (To, "/*{\n");
    fprintf (To, "This program was automatically generated from tree.\n");
    fprintf (To, "(c) Danila Baygushev, all rights reserved.\n");
    fprintf (To, "}*/\n\n");

    AddVars (To, Tree, Info, NodeInfo, ShData);

    AddAct (To, ShData);

    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);

    fprintf (To, "\n[Exeunt]");
}

void CreateShekspearRec (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("Shekspear Node %d\n", Tree.CurrentNode ());

    newNodeData Data = Tree.Get ();

    if (ShData.Scene > 1 && !ShData.UseScenes && Data.Descriptor != N_NODE)
    {
        AddAct (To, ShData);
    }

    if (Data.Descriptor == N_NEW)
    {
        New (To, Tree, Info, NodeInfo, ShData);

        return ;
    }
    if (Data.Descriptor == N_DEL)
    {
        Tree.DownL ();

        if (Tree.Get ().Descriptor == N_VAR) ShData.MC.Rem (PUSH_VAR (NodeInfo[Tree.CurrentNode ()].Addr));

        Tree.Up ();

        return ;
    }

    if (Data.Descriptor >= N_EQ && Data.Descriptor <= N_MOD_EQ)
    {
        Equal (To, Tree, Info, NodeInfo, ShData);

        return ;
    }

    if (Data.Descriptor == N_ECHO)
    {
        Echo (To, Tree, Info, NodeInfo, ShData);

        return ;
    }
    if (Data.Descriptor == N_GET)
    {
        Get (To, Tree, Info, NodeInfo, ShData);

        return ;
    }

    if (Data.Descriptor == N_IF)
    {
        If (To, Tree, Info, NodeInfo, ShData);

        return ;
    }

    if (Data.Descriptor == N_WHILE)
    {
        While (To, Tree, Info, NodeInfo, ShData);

        return ;
    }
    if (Data.Descriptor == N_FOR)
    {
        For (To, Tree, Info, NodeInfo, ShData);

        return ;
    }
    if (Data.Descriptor == N_BREAK && ShData.CycleEnd != -1)
    {
        CreatePlace (To, ShData);
        if (rand_state (2) == 1) fprintf (To, "    Let us ");
        if (rand_state (2) == 2) fprintf (To, "    We shall ");
        AddGotoScene (To, ShData.CycleEnd, ShData);
        fprintf (To, ".\n");

        return ;
    }
    if (Data.Descriptor == N_CONTINUE && ShData.CycleStart != -1)
    {
        CreatePlace (To, ShData);
        if (rand_state (2) == 1) fprintf (To, "    Let us ");
        if (rand_state (2) == 2) fprintf (To, "    We shall ");
        AddGotoScene (To, ShData.CycleStart, ShData);
        fprintf (To, ".\n");

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        CallFunc (To, Tree, Info, NodeInfo, ShData);

        return ;
    }

    if (Data.Descriptor == N_RETURN)
    {
        Tree.DownL ();

        if (LinearInstance (Tree))
        {
            CreateListener (To, RE, ShData);

            fprintf (To, "    You ");
            if (coin_toss ())
            {
                fprintf (To, "are as ");
                if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
                if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
                if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
                fprintf (To, " as ");
            }

            SetVar (To, Tree, Info, NodeInfo, ShData);

            fprintf (To, ".\n");
        }
        else
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, RE);
        }

        CreatePlace (To, ShData);

        if (rand_state (2) == 1) fprintf (To, "    Let us back.\n");
        if (rand_state (2) == 2) fprintf (To, "    We shall back.\n");

        Tree.Up ();

        return ;
    }

    if (Data.Descriptor == N_GOTO)
    {
        CreatePlace (To, ShData);

        fprintf (To, "    Let us proceed to the ");
        AddListItem ("places", Info.FuncNumber + NodeInfo [Tree.CurrentNode ()].ID, To, ShData.Way);
        fprintf (To, ".\n");

        return ;
    }
    if (Data.Descriptor == N_MARK)
    {
        AddSceneCall (To, Info.FuncNumber + NodeInfo [Tree.CurrentNode ()].ID, ShData);

        return ;
    }

    if (Tree.CanDownL())
    {
        Tree.DownL ();
        CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();
    }
    if (Tree.CanDownR())
    {
        Tree.DownR ();
        CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();
    }
}

//------------------------------------------------------------------------------

void OutRoman (FILE* To, int Num)
{
    const int  ArabArr  [13]    = { 1,   4,   5,   9,    10,  40,   50,  90,   100, 400,  500, 900,  1000};
    const char RomanArr [13][3] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};

    int Arri = 12;
    while (Num != 0)
    {
        if (Num >= ArabArr [Arri])
        {
            Num -= ArabArr [Arri];
            fprintf (To, "%s", RomanArr [Arri]);
        }
        else
        {
            Arri --;
        }

        if (Arri == -1) throw TH_ERROR "%d - is invalid number!", Num);
    }
}

void AddVars (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    int Size = MEMORY_OFFSET_ + Info.VarMemory + 3 * Info.ArrMemory;

    PUSH_CHARACTER(T1);
    fprintf (To, ", harum-scarum person./*Temp1*/\n");

    PUSH_CHARACTER(T2);
    fprintf (To, ", friend of harum-scarum person./*Temp2*/\n");

    PUSH_CHARACTER(IA);
    fprintf (To, ", right-hand person./*strange instance assist*/\n");

    PUSH_CHARACTER(RE);
    fprintf (To, ", devoted person./*Variable for return value*/\n");

    //PUSH_CHARACTER(AI);
    //fprintf (To, ", door keeper./*Arr i assist*/\n");

    PUSH_CHARACTER(LA);
    fprintf (To, ", interesting person./*links assist*/\n");

    for (int i = MEMORY_OFFSET_; i < Size; i ++)
    {
        PUSH_CHARACTER (i);

        if (i < MEMORY_OFFSET_ + Info.VarMemory)
        {
            fprintf (To, ", /* memory for variable #%d */.\n", i - MEMORY_OFFSET_);

            continue;
        }
        if (i < Size)
        {
            fprintf (To, ", /* memory for arr #%d'%d", (i - (MEMORY_OFFSET_ + Info.VarMemory)) / 3, (i - (MEMORY_OFFSET_ + Info.VarMemory)) % 3);
            fprintf (To, "*/.\n");

            continue;
        }
    }
}

void CreateScene (FILE* To, int A, int B, newTreeToShekspearData& ShData)
{
    if (ShData.OnScene.Size_ == 2 && ShData.OnScene.Find (A) && ShData.OnScene.Find (B)) return;

    ShData.CurrentSpeaker = -1;

    fprintf (To, "\n");

    if (ShData.OnScene.Size_ == 2 && (ShData.OnScene.Find (A) || ShData.OnScene.Find (B)))
    {
        fprintf (To, "[Exit ");
        if (ShData.OnScene.Data_ [0] == A || ShData.OnScene.Data_ [0] == B)
        {
            PUSH_CHARACTER (ShData.OnScene.Data_ [1]);
            ShData.OnScene.Rem (ShData.OnScene.Data_ [1]);
        }
        else
        {
            PUSH_CHARACTER (ShData.OnScene.Data_ [0]);
            ShData.OnScene.Rem (ShData.OnScene.Data_ [0]);
        }
        fprintf (To, "]\n");
    }
    else
    {
        if (ShData.OnScene.Size_ != 0)
        {
            fprintf (To, "[Exeunt]\n");
            ShData.OnScene.Empty ();
        }
    }

    fprintf (To, "[Enter ");
    if (!ShData.OnScene.Find (A))
    {
        PUSH_CHARACTER (A);
        ShData.OnScene.Add (A);

        if (!ShData.OnScene.Find (B)) fprintf (To, " and ");
    }
    if (!ShData.OnScene.Find (B))
    {
        PUSH_CHARACTER (B);
        ShData.OnScene.Add (B);
    }
    fprintf (To, "]\n");
}
void CreateSpeech (FILE* To, int A, newTreeToShekspearData& ShData)
{
    if (ShData.CurrentSpeaker == A) return;

    fprintf (To, "\n");
    PUSH_CHARACTER (A);
    fprintf (To, ":\n");
    ShData.CurrentSpeaker = A;
}
void CreateListener (FILE* To, int A, newTreeToShekspearData& ShData)
{
    if (ShData.CurrentSpeaker != -1 && ShData.CurrentSpeaker != A)
    {
        int B = ShData.CurrentSpeaker;
        CreateScene  (To, A, B, ShData);
        CreateSpeech (To, B, ShData);
    }
    else
    {
        if (ShData.OnScene.Size_ != 0)
        {
            int B = (ShData.OnScene.Data_ [0] != A)? ShData.OnScene.Data_ [0] : (ShData.OnScene.Size_ > 1)? ShData.OnScene.Data_ [1] : (A == T1)? T2 : T1;
            CreateScene  (To, A, B, ShData);
            CreateSpeech (To, B, ShData);
        }
        else
        {
            int B = (A == T1)? T2 : T1;
            CreateScene  (To, A, B, ShData);
            CreateSpeech (To, B, ShData);
        }
    }
}

void CreatePlace (FILE* To, newTreeToShekspearData& ShData)
{
    if (ShData.CurrentSpeaker != -1) return;

    if (ShData.OnScene.Size_ != 0)
    {
        CreateSpeech (To, ShData.OnScene.Data_[0], ShData);

        return;
    }

    CreateScene (To, T1, T2, ShData);

    CreateSpeech (To, T1, ShData);
}

void AddScene (FILE* To, newTreeToShekspearData& ShData)
{
    ShData.Scene ++;
    fprintf (To, "\nScene ");
    OutRoman (To, ShData.Scene);
    fprintf (To, ".\n");
    ShData.CurrentSpeaker = -1;
}
void AddSceneCall  (FILE* To, int A, newTreeToShekspearData& ShData)
{
    fprintf (To, "\nScene ");
    fprintf (To, "in the ");
    AddListItem ("places", A, To, ShData.Way);
    fprintf (To, ".\n");
    ShData.CurrentSpeaker = -1;
}

void AddAct (FILE* To, newTreeToShekspearData& ShData)
{
    ShData.Act ++;
    fprintf (To, "\nAct ");
    OutRoman (To, ShData.Act);
    fprintf (To, ".");
    ShData.Scene = 0;
    AddScene (To, ShData);
}
void AddActCall (FILE* To, int A, newTreeToShekspearData& ShData)
{
    fprintf (To, "\nAct ");
    fprintf (To, "in the ");
    AddListItem ("places", A, To, ShData.Way);
    fprintf (To, ".");
    ShData.Scene = 0;
    AddScene (To, ShData);
}


void AddGotoScene (FILE* To, int A, newTreeToShekspearData& ShData)
{
    if (ShData.Scene <= A) fprintf (To, "proceed");
    else                   fprintf (To, "return");
    fprintf (To, " to ");
    fprintf (To, "scene ");
    OutRoman (To, A);
}

void PushNumber (FILE* To, double Num, newTreeToShekspearData& ShData)
{
    #define BIG_INT unsigned long

    #define MAX_POW 5

    #define DOUBLE_EQL(A, B) (fabs ((A) - (B)) <= DBL_EPSILON * fmax (fabs (A), fabs (B)))
    #define IS_INT(A) (DOUBLE_EQL ((BIG_INT)(A), A))

    #define DOUBLE_COMP(A, B) (DOUBLE_EQL (A, B)? 0 : ((A > B)? 1 : -1))

    if (!IS_INT (Num))
    {
        if ((BIG_INT)Num != 0)
        {
            fprintf (To, "the sum of ");
            PushNumber (To, floor (Num), ShData);
            fprintf (To, " and ");
            Num -= floor (Num);
        }

        int powBase = 10;

        int pow = 0;
        while (!IS_INT (Num) && pow < MAX_POW)
        {
            pow ++;
            Num *= powBase;
        }

        fprintf (To, "the quotient between ");
        PushNumber (To, (BIG_INT)Num, ShData);
        fprintf (To, " and ");

        while (pow > 1)
        {
            if (pow % 3 == 0)
            {
                fprintf (To, "the cube of ");

                pow /= 3;

                continue;
            }
            if (pow % 2 == 0)
            {
                fprintf (To, "the square of ");

                pow /= 2;

                continue;
            }

            fprintf (To, "the product of ");
            PushNumber (To, powBase, ShData);
            fprintf (To, " and ");

            pow --;
        }

        PushNumber (To, powBase, ShData);
    }
    else
    {
        if (DOUBLE_EQL(Num, 0))
        {
            AddRandListItem ("nothing", To, ShData.Way);
            return;
        }

        double SquareNear = (Num >= 0)? sqrt (Num) : sqrt (-Num);
        SquareNear = (SquareNear - (int) SquareNear > 0.5)? pow ((int)SquareNear + 1, 2) : pow ((int)SquareNear, 2);
        SquareNear = (Num >= 0)? SquareNear : -SquareNear;

        double CubeNear = cbrt (Num);
        CubeNear = (CubeNear - (int) CubeNear > 0.5)? pow((int)CubeNear + 1, 3) : pow((int)CubeNear, 3);

        double PowTwoNear = (Num >= 0)? log2 (Num) : log2 (-Num);
        PowTwoNear = (PowTwoNear - (int) PowTwoNear > 0.5)? pow (2, ((int) PowTwoNear + 1)) : pow (2, ((int) PowTwoNear));
        PowTwoNear = (Num >= 0)? PowTwoNear : -PowTwoNear;

        if (Mod (SquareNear - Num) < Mod (CubeNear - Num) && Mod (SquareNear - Num) < Mod (PowTwoNear - Num))
        {
            if (SquareNear < 0)
            {
                fprintf (To, "the product of ");
                AddRandListItem ("negative_noun", To, ShData.Way);
                fprintf (To, " and ");
            }

            int compare_bit = DOUBLE_COMP (SquareNear, Num);

            if (compare_bit > 0)
            {
                fprintf (To, "the difference between ");
            }
            if (compare_bit < 0)
            {
                fprintf (To, "the sum of ");
            }

            fprintf (To, "the square of ");
            PushNumber (To, sqrt (Mod (SquareNear)), ShData);

            if (compare_bit != 0)
            {
                fprintf (To, " and ");
                PushNumber (To, Mod (SquareNear - Num), ShData);
            }

            return;
        }
        if (Mod (CubeNear - Num) <= Mod (SquareNear - Num) && Mod (CubeNear - Num) < Mod (PowTwoNear - Num))
        {
            int compare_bit = DOUBLE_COMP (CubeNear, Num);

            if (compare_bit > 0)
            {
                fprintf (To, "the difference between ");
            }
            if (compare_bit < 0)
            {
                fprintf (To, "the sum of ");
            }

            fprintf (To, "the cube of ");
            PushNumber (To, cbrt (CubeNear), ShData);

            if (compare_bit != 0)
            {
                fprintf (To, " and ");
                PushNumber (To, Mod (CubeNear - Num), ShData);
            }

            return;
        }
        if (Mod (PowTwoNear - Num) <= Mod (SquareNear - Num) && Mod (PowTwoNear - Num) <= Mod (CubeNear - Num))
        {
            int compare_bit = DOUBLE_COMP (PowTwoNear, Num);

            if (compare_bit > 0)
            {
                fprintf (To, "the difference between ");
            }
            if (compare_bit < 0)
            {
                fprintf (To, "the sum of ");
            }

            int Deg = (int)log2 (Mod (PowTwoNear));

            for (int i = 0; i < Deg; i ++)
            {
                if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
                if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
                if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);

                fprintf (To, " ");
            }

            if (Num > 0)
            {
                if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
                if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);
            }
            else
            {
                AddRandListItem ("negative_noun", To, ShData.Way);
            }

            if (compare_bit != 0)
            {
                fprintf (To, " and ");
                PushNumber (To, Mod (PowTwoNear - Num), ShData);
            }

            return;
        }
    }
}

void SetA (FILE* To, newTreeToShekspearData& ShData, int A, int SelfModyfier)
{
    CreateListener (To, A, ShData);

    fprintf (To, "    You ");
    if (coin_toss ())
    {
        fprintf (To, "are as ");
        if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
        if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
        if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }

    if (SelfModyfier >= N_SUM_EQ && SelfModyfier <= N_MOD_EQ)
    {
        if (SelfModyfier == N_SUM_EQ)
        {
            fprintf (To, "the sum of ");
        }
        if (SelfModyfier == N_SUB_EQ)
        {
            fprintf (To, "the difference between ");
        }
        if (SelfModyfier == N_MUL_EQ)
        {
            fprintf (To, "the product of ");
        }
        if (SelfModyfier == N_DIV_EQ)
        {
            fprintf (To, "the quotient between ");
        }
        if (SelfModyfier == N_MOD_EQ)
        {
            fprintf (To, "the remainder of quotient between ");
        }

        if (rand_state (2) == 1) AddRandListItem ("second_person", To, ShData.Way);
        if (rand_state (2) == 2) AddRandListItem ("second_person_reflexive", To, ShData.Way);

        fprintf (To, " and ");
    }
}

void SaveA (FILE* To, newTreeToShekspearData& ShData, int A)
{
    SetA (To, ShData, RE);
    PUSH_CHARACTER(A);
    fprintf (To, ".\n");

    CreateScene (To, A, RE, ShData);
    CreateSpeech (To, RE, ShData);
    CreateListener (To, A, ShData);

    fprintf (To, "    Remember me.\n");
}
void LoadA (FILE* To, newTreeToShekspearData& ShData, int A)
{
    CreateListener (To, A, ShData);

    fprintf (To, "    Recall your waste moment.\n");
}

bool LinearInstance (newTree& Tree)
{
    int Descriptor = Tree.Get ().Descriptor;

    if (Descriptor == N_FUN ||

        Descriptor == N_AND ||
        Descriptor == N_OR ||
        Descriptor == N_OPEQ ||
        Descriptor == N_NONEQ ||
        Descriptor == N_MORE ||
        Descriptor == N_LESS ||
        Descriptor == N_MREQ ||
        Descriptor == N_LSEQ)
    {
        return false;
    }

    bool Return = true;

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        Return = Return && LinearInstance (Tree);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        Return = Return && LinearInstance (Tree);
        Tree.Up ();
    }

    return Return;
}

void SetVar (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int A, int PIA, int SelfModyfier)
{
    //printf ("SetVar\n");

    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_TRUE)
    {
        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*true{*/");
        #endif

        PushNumber (To, 1, ShData);

        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*}*/");
        #endif

        return;
    }
    if (Data.Descriptor == N_FALSE)
    {
        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*false{*/");
        #endif

        PushNumber (To, 0, ShData);

        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*}*/");
        #endif

        return;
    }

    if (Data.Descriptor == N_NUM)
    {
        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*Number %lg {*/", Data.Data);
        #endif

        PushNumber (To, Data.Data, ShData);

        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*}*/");
        #endif

        return;
    }

    if (Data.Descriptor == N_AND)
    {
        Tree.DownL ();
        PushEquation (To, Tree, Info, NodeInfo, ShData, A);
        Tree.Up ();

        fprintf (To, "    If not, ");
        if (rand_state (2) == 1) fprintf (To, "we shall ");
        if (rand_state (2) == 2) fprintf (To, "let us ");
        AddGotoScene (To, ShData.Scene + 1, ShData);
        fprintf (To, ".\n");

        Tree.DownR ();
        PushEquation (To, Tree, Info, NodeInfo, ShData, PIA);
        Tree.Up ();

        CreateListener (To, A, ShData);

        fprintf (To, "    If so, you ");
        if (coin_toss ())
        {
            fprintf (To, "are as ");
            if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
            if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
            if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
            fprintf (To, " as ");
        }
        if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
        if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);
        fprintf (To, ".\n");

        AddScene (To, ShData);

        CreateListener (To, A, ShData);

        fprintf (To, "    If not, you ");
        if (coin_toss ())
        {
            fprintf (To, "are as ");
            if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
            if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
            if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
            fprintf (To, " as ");
        }
        AddRandListItem ("nothing", To, ShData.Way);
        fprintf (To, ".\n");

        return ;
    }
    if (Data.Descriptor == N_OR)
    {
        Tree.DownL ();
        PushEquation (To, Tree, Info, NodeInfo, ShData, A);
        Tree.Up ();

        fprintf (To, "    If so, ");
        if (rand_state (2) == 1) fprintf (To, "we shall ");
        if (rand_state (2) == 2) fprintf (To, "let us ");
        AddGotoScene (To, ShData.Scene + 1, ShData);
        fprintf (To, ".\n");

        Tree.DownR ();
        PushEquation (To, Tree, Info, NodeInfo, ShData, PIA);
        Tree.Up ();

        CreateListener (To, A, ShData);

        fprintf (To, "    If not, you ");
        if (coin_toss ())
        {
            fprintf (To, "are as ");
            if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
            if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
            if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
            fprintf (To, " as ");
        }
        AddRandListItem ("nothing", To, ShData.Way);
        fprintf (To, ".\n");

        AddScene (To, ShData);

        CreateListener (To, A, ShData);

        fprintf (To, "    If so, you ");
        if (coin_toss ())
        {
            fprintf (To, "are as ");
            if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
            if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
            if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
            fprintf (To, " as ");
        }
        if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
        if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);
        fprintf (To, ".\n");

        return ;
    }

    if (Data.Descriptor == N_NOT)
    {
        Tree.DownL ();

        if (!LinearInstance (Tree))
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, A);
        }

        if (!LinearInstance (Tree)) SetA (To, ShData, A);
        fprintf (To, "the remainder of quotient between the sum of ");
        if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
        if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);
        fprintf (To, " and ");
        if (LinearInstance (Tree)) SetVar (To, Tree, Info, NodeInfo, ShData);
        else                       PUSH_CHARACTER (A);
        fprintf (To, " and ");
        PushNumber (To, 2, ShData);
        if (!LinearInstance (Tree)) fprintf (To, ".\n");

        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_OPEQ || Data.Descriptor == N_NONEQ)
    {
        bool LL, LR;
        Tree.DownL ();
        LL = LinearInstance (Tree);
        if (!LL)
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, A);
        }
        Tree.Up ();
        Tree.DownR ();
        LR = LinearInstance (Tree);
        if (!LR)
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, PIA);
        }
        Tree.Up ();

        CreatePlace (To, ShData);

        fprintf (To, "    Are ");

        Tree.DownL ();
        if (LL) SetVar (To, Tree, Info, NodeInfo, ShData);
        else    PUSH_CHARACTER (A);
        Tree.Up ();

        if (Data.Descriptor == N_NONEQ) fprintf (To, " not");
        fprintf (To, " as ");
        if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
        if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
        if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");

        Tree.DownR ();
        if (LR) SetVar (To, Tree, Info, NodeInfo, ShData);
        else    PUSH_CHARACTER (PIA);
        Tree.Up ();

        fprintf (To, "?\n");

        SetA (To, ShData, A);
        AddRandListItem ("nothing", To, ShData.Way);
        fprintf (To, ".\n");

        fprintf (To, "    If so, you ");
        if (coin_toss ())
        {
            fprintf (To, "are as ");
            if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
            if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
            if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
            fprintf (To, " as ");
        }
        if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
        if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);
        fprintf (To, ".\n");

        return ;
    }
    if (Data.Descriptor == N_MORE || Data.Descriptor == N_LESS || Data.Descriptor == N_MREQ || Data.Descriptor == N_LSEQ)
    {
        bool LL, LR;
        Tree.DownL ();
        LL = LinearInstance (Tree);
        if (!LL)
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, A);
        }
        Tree.Up ();
        Tree.DownR ();
        LR = LinearInstance (Tree);
        if (!LR)
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, PIA);
        }
        Tree.Up ();

        CreatePlace (To, ShData);

        fprintf (To, "    Are ");

        Tree.DownL ();
        if (LL) SetVar (To, Tree, Info, NodeInfo, ShData);
        else    PUSH_CHARACTER (A);
        Tree.Up ();

        fprintf (To, " ");
        if (Data.Descriptor == N_MREQ || Data.Descriptor == N_LSEQ) fprintf (To, "not ");
        if (Data.Descriptor == N_MORE || Data.Descriptor == N_LSEQ) AddRandListItem ("positive_comparative", To, ShData.Way);
        else                                                        AddRandListItem ("negative_comparative", To, ShData.Way);
        fprintf (To, " than ");

        Tree.DownR ();
        if (LR) SetVar (To, Tree, Info, NodeInfo, ShData);
        else    PUSH_CHARACTER (PIA);
        Tree.Up ();

        fprintf (To, "?\n");

        SetA (To, ShData, A);
        AddRandListItem ("nothing", To, ShData.Way);
        fprintf (To, ".\n");

        fprintf (To, "    If so, you ");
        if (coin_toss ())
        {
            fprintf (To, "are as ");
            if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
            if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
            if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
            fprintf (To, " as ");
        }
        if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
        if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);
        fprintf (To, ".\n");

        return ;
    }

    if (Data.Descriptor == N_VAR)
    {
        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*Var %s{*/", Data.GetName ());
        #endif

        PUSH_CHARACTER( PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr) );

        #ifdef IN_PROGRAM_DEBUG
        fprintf (To, "/*}*/");
        #endif

        return;
    }

    if (Data.Descriptor == N_ARR)
    {
        /*Tree.DownL ();

        if (ShData.InArrIndex)
        {
            CreateListener (To, RE, ShData);

            fprintf (To, "    You ");
            if (Ver (50))
            {
                fprintf (To, "are as ");
                if (Ver (50))      AddRandListItem ("positive_adjective", To);
                else if (Ver (25)) AddRandListItem ("neutral_adjective", To);
                else               AddRandListItem ("negative_adjective", To);
                fprintf (To, " as ");
            }
            fprintf (To, "me.\n");

            fprintf (To, "\n");

            CreateListener (To, AI, ShData);

            fprintf (To, "    Remeber me.\n");
        }

        bool Backup = ShData.InArrIndex;
        ShData.InArrIndex = true;

        SetVar (To, Tree, Info, NodeInfo, ShData, AI);

        ShData.InArrIndex = Backup;
        Tree.Up  ();

        PushArrIFunc (To, Info, NodeInfo, ShData, PUSH_ARRA(NodeInfo[Tree.CurrentNode ()].Addr));

        if (LOnly)
        {
            CreateListener (To, A, ShData);

            fprintf (To, "You ");
            if (Ver (50))
            {
                fprintf (To, "are as ");
                if (Ver (50))      AddRandListItem ("positive_adjective", To);
                else if (Ver (25)) AddRandListItem ("neutral_adjective", To);
                else               AddRandListItem ("negative_adjective", To);
                fprintf (To, " as ");
            }
            PUSH_CHARACTER(PUSH_ARRS1(NodeInfo[Tree.CurrentNode ()].Addr));
            fprintf (To, ".\n");
        }
        else
        {
            CreateListener (To, IA, ShData);

            fprintf (To, "You ");
            if (Ver (50))
            {
                fprintf (To, "are as ");
                if (Ver (50))      AddRandListItem ("positive_adjective", To);
                else if (Ver (25)) AddRandListItem ("neutral_adjective", To);
                else               AddRandListItem ("negative_adjective", To);
                fprintf (To, " as ");
            }
            PUSH_CHARACTER(PUSH_ARRS1(NodeInfo[Tree.CurrentNode ()].Addr));
            fprintf (To, ".\n");
        }

        if (ShData.InArrIndex)
        {
            CreateListener (To, AI, ShData);

            fprintf (To, "    Recall your waste moment.\n");
        }*/

        return;
    }

    if (Data.Descriptor == N_FUN)
    {
        CallFunc (To, Tree, Info, NodeInfo, ShData);

        SetA (To, ShData, A);

        PUSH_CHARACTER (RE);

        fprintf (To, ".\n");

        return;
    }

    bool LL, RL;
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        LL = LinearInstance (Tree);
        Tree.Up ();
    }
    else LL = true;
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        RL = LinearInstance (Tree);
        Tree.Up ();
    }
    else RL = true;

    if (!LL)
    {
        Tree.DownL ();
        SetVar (To, Tree, Info, NodeInfo, ShData, A, PIA, SelfModyfier);
        Tree.Up ();
    }
    if (!RL)
    {
        SaveA (To, ShData, A);

        Tree.DownR ();
        SetVar (To, Tree, Info, NodeInfo, ShData, PIA, A, SelfModyfier);
        Tree.Up ();

        LoadA (To, ShData, A);
    }

    if (!LL || !RL)
    {
        SetA (To, ShData, A, SelfModyfier);
    }

    if (Data.Descriptor == N_SUM)
    {
        fprintf (To, "the sum of ");
    }
    if (Data.Descriptor == N_SUB)
    {
        fprintf (To, "the difference between ");
    }
    if (Data.Descriptor == N_MUL)
    {
        fprintf (To, "the product of ");
    }
    if (Data.Descriptor == N_DIV)
    {
        fprintf (To, "the quotient between ");
    }
    if (Data.Descriptor == N_MOD)
    {
        fprintf (To, "the remainder of quotient between ");
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        if (LL) SetVar (To, Tree, Info, NodeInfo, ShData, -1);
        else PUSH_CHARACTER(A);
        Tree.Up ();
    }

    fprintf (To, " and ");

    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        if (RL) SetVar (To, Tree, Info, NodeInfo, ShData, -1);
        else PUSH_CHARACTER(PIA);
        Tree.Up ();
    }

    if (!LL || !RL)
    {
        fprintf (To, ".\n");
    }
}

void PushArrIFunc (FILE* To, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int A)
;
/*{
    int ArrID = (A - MEMORY_OFFSET_ - Info.VarMemory) / 3;
    if (ShData.ArrFuncComplete [ArrID])
    {
        CreatePlace (To, ShData);

        fprintf (To, "    ");
        if (Ver (50)) fprintf (To, "Let us ");
        else             fprintf (To, "We shall ");
        fprintf (To, "proceed to ");
        AddListItem ("places", ArrID + Info.FuncNumber, To, ShData.Way);
        fprintf (To, ".\n");

        return;
    }

    ShData.ArrFuncComplete [ArrID] = true;
    int Mrk = ShData.Scene;

    AddSceneCall (To, ArrID + Info.FuncNumber, ShData);

    CreateListener (To, AI, ShData);

    fprintf (To, "    Are you as ");
    AddRandListItem ("positive_adjective", To, ShData.Way);
    fprintf (To, " as ");
    PUSH_CHARACTER (A + 1);
    fprintf (To, "?\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    If so, ");
    if (Ver (50)) fprintf (To, "let us ");
    else             fprintf (To, "we shall ");
    fprintf (To, "back.\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    Are you ");
    AddRandListItem ("positive_comparative", To, ShData.Way);
    fprintf (To, " than ");
    PUSH_CHARACTER (A + 1);
    fprintf (To, "?\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    If so, ");
    if (Ver (50)) fprintf (To, "let us ");
    else             fprintf (To, "we shall ");
    AddGotoScene (To, Mrk + 1, ShData);
    fprintf (To, ".\n");

    //####

    CreateListener (To, AI, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    fprintf (To, "the difference between thyself and ");
    if (Ver (50)) AddRandListItem ("positive_noun", To, ShData.Way);
    else          AddRandListItem ("neutral_noun", To, ShData.Way);
    fprintf (To, ".\n");

    AddScene (To, ShData);

    CreateListener (To, A + 2, ShData);

    fprintf (To, "    Remember me.\n");

    CreateListener (To, A + 2, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    fprintf (To, "the sum of thyself and ");
    if (Ver (50)) AddRandListItem ("positive_noun", To, ShData.Way);
    else          AddRandListItem ("neutral_noun", To, ShData.Way);
    fprintf (To, ".\n");

    CreateListener (To, A + 1, ShData);

    fprintf (To, "    Recall your triumf.\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    fprintf (To, "the difference between thyself and ");
    if (Ver (50)) AddRandListItem ("positive_noun", To, ShData.Way);
    else          AddRandListItem ("neutral_noun", To, ShData.Way);
    fprintf (To, ".\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    Are you ");
    AddRandListItem ("positive_comparative", To, ShData.Way);
    fprintf (To, " than nothing?\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    If so, ");
    if (Ver (50)) fprintf (To, "let us ");
    else             fprintf (To, "we shall ");
    AddGotoScene (To, Mrk + 0, ShData);
    fprintf (To, ".\n");

    CreateListener (To, AI, ShData);

    if (Ver (50)) fprintf (To, "    Let us ");
    else             fprintf (To, "    We shall ");
    fprintf (To, "back.\n");

    //####

    AddScene (To, ShData);

    CreateListener (To, AI, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    fprintf (To, "the difference between ");
    if (Ver (50)) AddRandListItem ("positive_noun", To, ShData.Way);
    else          AddRandListItem ("neutral_noun", To, ShData.Way);
    fprintf (To, " and thyself.\n");

    AddScene (To, ShData);

    CreateListener (To, RE, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    PUSH_CHARACTER (A + 1);
    fprintf (To, ".\n");

    CreateListener (To, A + 1, ShData);

    fprintf (To, "    Remember me.\n");

    CreateListener (To, RE, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    PUSH_CHARACTER (A + 2);
    fprintf (To, ".\n");

    CreateListener (To, A + 2, ShData);

    fprintf (To, "Recall your triumf.\n");

    CreateListener (To, A + 1, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    PUSH_CHARACTER (A + 2);
    fprintf (To, ".\n");

    CreateListener (To, A + 2, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    fprintf (To, "the difference between ");
    PUSH_CHARACTER (RE);
    fprintf (To, " and ");
    if (Ver (50)) AddRandListItem ("positive_noun", To, ShData.Way);
    else          AddRandListItem ("neutral_noun", To, ShData.Way);
    fprintf (To, ".\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    You ");
    if (Ver (50))
    {
        fprintf (To, "are as ");
        if (Ver (50))      AddRandListItem ("positive_adjective", To, ShData.Way);
        else if (Ver (25)) AddRandListItem ("neutral_adjective", To, ShData.Way);
        else               AddRandListItem ("negative_adjective", To, ShData.Way);
        fprintf (To, " as ");
    }
    fprintf (To, "the difference between thyself and ");
    if (Ver (50)) AddRandListItem ("positive_noun", To, ShData.Way);
    else          AddRandListItem ("neutral_noun", To, ShData.Way);
    fprintf (To, ".\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    Are you ");
    AddRandListItem ("positive_comparative", To, ShData.Way);
    fprintf (To, " than nothing?\n");

    CreateListener (To, AI, ShData);

    fprintf (To, "    If so, ");
    if (Ver (50)) fprintf (To, "let us ");
    else             fprintf (To, "we shall ");
    AddGotoScene (To, Mrk + 2, ShData);
    fprintf (To, ".\n");

    CreateListener (To, AI, ShData);

    if (Ver (50)) fprintf (To, "    Let us ");
    else             fprintf (To, "    We shall ");
    fprintf (To, "back.\n");
}*/

void New (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("New\n");
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR)
    {
        if (Tree.CanDownR ())
        {
            int Addr = PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr);
            ShData.MC.Add (Addr);
            Tree.DownR ();

            if (LinearInstance (Tree))
            {
                SetA (To, ShData, Addr);

                SetVar (To, Tree, Info, NodeInfo, ShData);

                fprintf (To, ".\n");
            }
            else
            {
                SetVar (To, Tree, Info, NodeInfo, ShData, Addr);
            }

            Tree.Up ();
        }

        return ;
    }
    if (Data.Descriptor == N_ARR)
    {

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        NewFunc (To, Tree, Info, NodeInfo, ShData);

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        New (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        New (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();
    }
}

void Equal (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("Equal\n");

    int Desc = Tree.Get ().Descriptor;

    Tree.DownL ();

    if (Tree.Get ().Descriptor == N_VAR)
    {
        int Addr = PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr);
        Tree.Up ();

        if (Desc != N_INCR && Desc != N_DECR)
        {
            Tree.DownR ();
            if (LinearInstance (Tree))
            {
                SetA (To, ShData, Addr, Desc);

                SetVar (To, Tree, Info, NodeInfo, ShData);

                fprintf (To, ".\n");
            }
            else
            {
                SetVar (To, Tree, Info, NodeInfo, ShData, Addr, IA, Desc);
            }
            Tree.Up ();
        }
        else
        {
            if (Desc == N_INCR) Desc = N_SUM_EQ;
            if (Desc == N_DECR) Desc = N_SUB_EQ;

            SetA (To, ShData, Addr, Desc);

            PushNumber (To, 1, ShData);

            fprintf (To, ".\n");
        }

        return ;
    }
    if (Tree.Get ().Descriptor == N_ARR)
    {
        Tree.Up ();

        Tree.DownL ();
        //PushInstance (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();

    }
}

void Echo (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("Echo\n");

    Tree.DownL ();

    if (LinearInstance (Tree))
    {
        SetA (To, ShData, T1);

        SetVar (To, Tree, Info, NodeInfo, ShData);

        fprintf (To, ".\n");
    }
    else
    {
        SetVar (To, Tree, Info, NodeInfo, ShData, T1);
    }

    CreateListener (To, T1, ShData);

    fprintf (To, "    Speak your mind.\n");

    Tree.Up ();
}
void Get  (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("Get\n");

    Tree.DownL ();

    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_VAR)
    {
        CreateListener (To, PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr), ShData);

        fprintf (To, "    Open your mind.\n");

        Tree.Up ();

        return ;
    }
    if (Data.Descriptor == N_ARR)
    {
        Tree.Up ();

        return ;
    }

    if (Data.Descriptor == N_FUN)
    {
        Tree.Up ();

        return ;
    }
}

int GetMarkNumber (newTree& Tree, int State)
{
    newNodeData Data = Tree.Get ();

    int Return = 0;

    if (Data.Descriptor == N_MARK || Data.Descriptor == N_AND || Data.Descriptor == N_OR || Data.Descriptor == N_IF || Data.Descriptor == N_ELSE) Return +=  1;

    if (Data.Descriptor == N_WHILE) Return += 2;
    if (Data.Descriptor == N_FOR) Return += 3;

    if (Data.Descriptor == N_NEW)
    {
        State = 1;

        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            Return += GetMarkNumber (Tree, State);
            Tree.Up ();
        }
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            Return += GetMarkNumber (Tree, State);
            Tree.Up ();
        }

        State = 0;

        return Return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        Return += GetMarkNumber (Tree, State);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        Return += GetMarkNumber (Tree, State);
        Tree.Up ();
    }

    return Return;
}

void PushEquation (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int A)
{
    //printf ("Push equation\n");

    //newNodeData Data = Tree.Get ();
    //GCC say it's not used!

    if (!LinearInstance (Tree))
    {
        SetVar (To, Tree, Info, NodeInfo, ShData, A);
    }

    CreatePlace (To, ShData);

    fprintf (To, "    Are ");

    if (!LinearInstance (Tree)) PUSH_CHARACTER (A);
    else                        SetVar (To, Tree, Info, NodeInfo, ShData);

    fprintf (To, " as ");
    if (rand_state (3) == 1) AddRandListItem ("positive_adjective", To, ShData.Way);
    if (rand_state (3) == 2) AddRandListItem ("neutral_adjective", To, ShData.Way);
    if (rand_state (3) == 3) AddRandListItem ("negative_adjective", To, ShData.Way);
    fprintf (To, " as ");

    if (rand_state (2) == 1) AddRandListItem ("positive_noun", To, ShData.Way);
    if (rand_state (2) == 2) AddRandListItem ("neutral_noun", To, ShData.Way);

    fprintf (To, "?\n");
}

void If (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("If\n");

    bool Copy = ShData.UseScenes;
    ShData.UseScenes = true;

    Tree.DownL ();
    PushEquation (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();

    Tree.DownR ();

    if (Tree.Get ().Descriptor == N_ELSE)
    {
        ShData.MC.PushMemory ();

        Tree.DownL ();

        CreatePlace (To, ShData);

        fprintf (To, "    If not, ");
        if (rand_state (2) == 1) fprintf (To, "let us ");
        if (rand_state (2) == 2) fprintf (To, "we shall ");
        AddGotoScene (To, ShData.Scene + GetMarkNumber (Tree) + 1, ShData);
        fprintf (To, ".\n");
        CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();

        ShData.MC.PopMemory ();

        ShData.MC.PushMemory ();
        Tree.DownR ();

        CreatePlace (To, ShData);

        if (rand_state (2) == 1) fprintf (To, "    Let us ");
        if (rand_state (2) == 2) fprintf (To, "    We shall ");
        AddGotoScene (To, ShData.Scene + GetMarkNumber (Tree) + 2, ShData);
        fprintf (To, ".\n");

        AddScene (To, ShData);

        CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
        Tree.Up ();

        ShData.MC.PopMemory ();
    }
    else
    {
        CreatePlace (To, ShData);

        fprintf (To, "    If not, ");
        if (rand_state (2) == 1) fprintf (To, "let us ");
        if (rand_state (2) == 2) fprintf (To, "we shall ");
        AddGotoScene (To, ShData.Scene + GetMarkNumber (Tree) + 1, ShData);
        fprintf (To, ".\n");

        ShData.MC.PushMemory ();
        CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
        ShData.MC.PopMemory ();
    }

    AddScene (To, ShData);

    Tree.Up ();

    ShData.UseScenes = Copy;
}

void While (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("While\n");

    bool Copy = ShData.UseScenes;
    ShData.UseScenes = true;

    AddScene (To, ShData);
    int SC = ShData.CycleStart, EC = ShData.CycleEnd;
    ShData.CycleStart = ShData.Scene;
    ShData.CycleEnd = ShData.Scene + GetMarkNumber (Tree) - 1;

    CreatePlace (To, ShData);

    Tree.DownL ();
    PushEquation (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();

    CreatePlace (To, ShData);

    fprintf (To, "    If not, ");
    if (rand_state (2) == 1) fprintf (To, "let us ");
    if (rand_state (2) == 2) fprintf (To, "we shall ");
    AddGotoScene (To, ShData.CycleEnd, ShData);
    fprintf (To, ".\n");

    ShData.MC.PushMemory ();
    Tree.DownR ();
    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    ShData.MC.PopMemory ();

    CreatePlace (To, ShData);

    if (rand_state (2) == 1) fprintf (To, "    Let us ");
    if (rand_state (2) == 2) fprintf (To, "    We shall ");
    AddGotoScene (To, ShData.CycleStart, ShData);
    fprintf (To, ".\n");

    AddScene (To, ShData);

    ShData.CycleStart = SC;
    ShData.CycleEnd = EC;

    ShData.UseScenes = Copy;
}
void For (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("For\n");

    bool Copy = ShData.UseScenes;
    ShData.UseScenes = true;

    ShData.MC.PushMemory ();
    Tree.DownL ();
    Tree.DownL ();
    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    Tree.Up ();

    CreatePlace (To, ShData);

    Tree.DownR ();
    Tree.DownL ();
    if (rand_state (2) == 1) fprintf (To, "    Let us ");
    if (rand_state (2) == 2) fprintf (To, "    We shall ");
    AddGotoScene (To, ShData.Scene + GetMarkNumber (Tree) + 2, ShData);
    fprintf (To, ".\n");
    Tree.Up ();
    AddScene (To, ShData);
    int SC = ShData.CycleStart, EC = ShData.CycleEnd;
    ShData.CycleStart = ShData.Scene;
    ShData.CycleEnd = ShData.Scene + GetMarkNumber (Tree) + 2;
    Tree.Up ();

    Tree.DownL ();
    Tree.DownR ();
    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    Tree.Up ();

    AddScene (To, ShData);
    Tree.DownR ();
    Tree.DownL ();
    PushEquation (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    Tree.Up ();

    CreatePlace (To, ShData);

    fprintf (To, "    If not, ");
    if (rand_state (2) == 1) fprintf (To, "let us ");
    if (rand_state (2) == 2) fprintf (To, "we shall ");
    AddGotoScene (To, ShData.CycleEnd, ShData);
    fprintf (To, ".\n");

    ShData.MC.PushMemory ();
    Tree.DownR ();
    Tree.DownR ();
    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    Tree.Up ();
    ShData.MC.PopMemory ();

    CreatePlace (To, ShData);

    if (rand_state (2) == 1) fprintf (To, "    Let us ");
    if (rand_state (2) == 2) fprintf (To, "    We shall ");
    AddGotoScene (To, ShData.CycleStart, ShData);
    fprintf (To, ".\n");

    AddScene (To, ShData);

    ShData.MC.PopMemory ();

    ShData.CycleStart = SC;
    ShData.CycleEnd = EC;

    ShData.UseScenes = Copy;
}

void NewFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    Tree.PushP ();

    if (NodeInfo [Tree.CurrentNode ()].Addr == -2) throw TH_ERROR "Use natives?");

    if (!Tree.CanDownR()) Tree.CurrentNode(NodeInfo [Tree.CurrentNode ()].Addr);

    if (NodeInfo [Tree.CurrentNode ()].Addr == -1)
    {
        Tree.PopP();

        return;
    }

    Tree.CurrentNode (NodeInfo [Tree.CurrentNode ()].Addr);

    if (strcmp (Tree.Get ().GetName (), "main") != 0)
    {
        CreatePlace (To, ShData);

        fprintf (To, "    Let us proceed to act ");
        OutRoman (To, ShData.Act + 1);
        fprintf (To, ".\n");
    }

    AddActCall (To, NodeInfo [Tree.CurrentNode ()].ID, ShData);

    ShData.MC.AddFunc (NodeInfo [Tree.CurrentNode ()].ID);

    ShData.UseScenes = true;

    ShData.MC.PushMemory ();
    Tree.DownL ();
    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    Tree.DownR ();
    CreateShekspearRec (To, Tree, Info, NodeInfo, ShData);
    Tree.Up ();
    ShData.MC.PopMemory ();

    ShData.UseScenes = false;

    AddAct (To, ShData);

    NodeInfo [Tree.CurrentNode ()].Addr = -1;

    Tree.PopP ();
}

void CallFunc (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    //printf ("CallFunc\n");

    int FuncNumber = NodeInfo [Tree.CurrentNode ()].ID;

    ShData.MC.PrintfPush (To, FuncNumber, ShData);

    Tree.DownL ();
    Tree.PushP ();
    Tree.Up ();
    Tree.CurrentNode (NodeInfo [Tree.CurrentNode ()].Addr);
    Tree.DownL ();
    PushParams (To, Tree, Info, NodeInfo, ShData);
    Tree.PopP ();
    Tree.Up ();

    CreatePlace (To, ShData);

    fprintf (To, "    Let us proceed to ");
    AddListItem ("places", FuncNumber, To, ShData.Way);
    fprintf (To, ".\n");

    Tree.DownL ();
    Tree.PushP ();
    Tree.Up ();
    Tree.CurrentNode (NodeInfo [Tree.CurrentNode ()].Addr);
    Tree.DownL ();
    PopParams (To, Tree, Info, NodeInfo, ShData, FuncNumber);
    Tree.PopP ();
    Tree.Up ();

    ShData.MC.PrintfPop (To, FuncNumber, ShData);
}

void PushParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NEW)
    {
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            PushParams (To, Tree, Info, NodeInfo, ShData);
            Tree.Up ();
        }

        return;
    }

    if (Data.Descriptor == N_VAR)
    {
        int Addr = PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr);

        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.PushP ();

        if (LinearInstance (Tree))
        {
            SetA (To, ShData, Addr);

            SetVar (To, Tree, Info, NodeInfo, ShData);

            fprintf (To, ".\n");
        }
        else
        {
            SetVar (To, Tree, Info, NodeInfo, ShData, Addr);
        }

        Tree.PosSave_.Swap ();
        Tree.PopP ();

        return;
    }

    if (Data.Descriptor == N_LNK)
    {
        int Addr = PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr);

        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.PushP ();

        SetA (To, ShData, Addr);
        PUSH_CHARACTER (PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr));
        fprintf (To, ".\n");

        Tree.PosSave_.Swap ();
        Tree.PopP ();

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.DownL ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();

        PushParams (To, Tree, Info, NodeInfo, ShData);

        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.DownR ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();

        PushParams (To, Tree, Info, NodeInfo, ShData);

        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
    }
}

void PopParams (FILE* To, newTree& Tree, newTreeInfo& Info, newVector <newNodeInfo>& NodeInfo, newTreeToShekspearData& ShData, int FunctionN)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NEW)
    {
        if (Tree.CanDownL ())
        {
            Tree.DownL ();
            PopParams (To, Tree, Info, NodeInfo, ShData, FunctionN);
            Tree.Up ();
        }

        return;
    }

    if (Data.Descriptor == N_LNK)
    {
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.PushP ();

        Tree.DownL ();
        int Addr = PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr);
        Tree.Up ();

        Tree.PosSave_.Swap ();
        Tree.PopP ();

        if (ShData.MC.UseInPop (Addr, FunctionN))
        {
            SetA (To, ShData, LA);
            PUSH_CHARACTER (Addr);
            fprintf (To, ".\n");

            LoadA (To, ShData, Addr);

            SetA (To, ShData, Addr);
            PUSH_CHARACTER (LA);
            fprintf (To, ".\n");
        }

        SetA (To, ShData, Addr);
        PUSH_CHARACTER (PUSH_VAR(NodeInfo[Tree.CurrentNode ()].Addr));
        fprintf (To, ".\n");

        if (ShData.MC.UseInPop (Addr, FunctionN))
        {
            SaveA (To, ShData, Addr);

            SetA (To, ShData, Addr);
            PUSH_CHARACTER (LA);
            fprintf (To, ".\n");
        }

        return;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.DownL ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();

        PopParams (To, Tree, Info, NodeInfo, ShData, FunctionN);

        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.DownR ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();

        PopParams (To, Tree, Info, NodeInfo, ShData, FunctionN);

        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.Up ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
    }
}

#include "ShMemoryCounter.cpp"

#endif
