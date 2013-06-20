#ifndef SHEKSPEARTOTREE__
#define SHEKSPEARTOTREE__

#include "..\..\SCC_Tree\Lib\Tree.h"
#include "..\..\SCC_Tree\Lib\TreeOperations.h"
#include "..\..\SCC_Tree\Lib\BaseTreeDesc.h"
#include "..\..\_SystemLibs\Throw\Throw.h"
#include "cmath"
#include "Help.h"
#include "Counter.h"

struct newShekspearData
{
    char FirstPersonName [60];
    char SecondPersonName [60];
    bool SecondPersonExist;

    newCounter OnScene;

    newCounter Vars;
    newCounter Stacks;

    newCounter Marks;
    newCounter Funcs;

    bool InFunc;
    char FunctionName [30];

    int Act;
    int Scene;

    bool UseNumOut;
    bool UseNumIn;

    newTree NumIn;
    newTree NumOut;

    bool UseStatements;

    int StartPos;

    bool Return;

    string Way;
};

bool CreateAll (newTree& Tree, FILE* Program, FILE* ErrorOutput, newTree NumIn, newTree NumOut, string Way);

//------------------------------------------------------------------------------

void CreateInfo (FILE* Program, newShekspearData& ShData);

void GetInfoAll     (FILE* Program, newShekspearData& ShData, char Str []);
void GetInfoAllOnce (FILE* Program, newShekspearData& ShData, char Str []);

void GetInfoAct (FILE* Program, newShekspearData& ShData, char Str []);
void GetInfoScene (FILE* Program, newShekspearData& ShData, char Str []);

void GetInfoSpeech (FILE* Program, newShekspearData& ShData, char Str []);
bool GetInfoPhrase (FILE* Program, newShekspearData& ShData, char Str []);

void GetInfoStackOp (FILE* Program, newShekspearData& ShData, char Str []);

void GetInfoInOut (FILE* Program, newShekspearData& ShData, char Str []);

void GetInfoCallGoto (FILE* Program, newShekspearData& ShData, char Str []);

void GetInfoIf (FILE* Program, newShekspearData& ShData, char Str []);

//------------------------------------------------------------------------------

void CreateTree (newTree& Tree, FILE* Program, newShekspearData& ShData);

void AddVarsStacks (newTree& Tree, newShekspearData& ShData);

void AddNumInOut (newTree& Tree, newShekspearData& ShData);

void Add (newTree& Tree);
void Free (newTree& Tree);

int GetRoman (char Num []);

void GetAll     (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);
void GetAllOnce (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetAct (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);
void GetScene (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetOnScene (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetSpeech (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);
bool GetPhrase (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetStackOp (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);
void GetFirstPerson (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);
void GetSecondPerson (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetInOut (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetCallGoto (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetIf (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetInstance (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

void GetEquation (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str []);

//******************************************************************************

bool CreateAll (newTree& Tree, FILE* Program, FILE* ErrorOutput, newTree NumIn, newTree NumOut, string Way)
{
    newShekspearData ShData;

    ShData.InFunc    = false;

    ShData.UseNumOut = false;
    ShData.UseNumIn  = false;

    ShData.NumIn = NumIn;
    ShData.NumOut = NumOut;

    ShData.UseStatements = false;

    ShData.Return = false;

    ShData.Way = Way;

    #ifdef INFO_DEBUG
        printf ("%s\n", __PRETTY_FUNCTION__);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    try
    {
        CreateInfo (Program, ShData);

        ShData.OnScene.Size_ = 0;

        CreateTree (Tree, Program, ShData);
    }
    catch (newThrowError& Error)
    {
        fprintf (ErrorOutput, "\n%s", Error.ErrorText_);

        int ErrorPos = ftell (Program);
        fseek (Program, 0, 0);
        char c = 0;
        while (true)
        {
            if (ftell (Program) == ErrorPos) fprintf (ErrorOutput, "<-ERROR OR ERROR->");

            if (!fread (&c, 1, 1, Program)) break;
            fprintf (ErrorOutput, "%c", c);
        }
        fseek (Program, ErrorPos, 0);

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

void CreateInfo (FILE* Program, newShekspearData& ShData)
{
    char Str [60] = "";

    ReadTo (Program, "\n\n");

    ShData.StartPos = ftell (Program);

    while (true)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) return;

        if (strcmp (Str, "act") == 0) break;

        if (ShData.Vars.Find (Str)) throw TH_ERROR "Name has been already used (%s)", Str);
        ShData.Vars.Add (Str);
        if (!InList ("character", Str, ShData.Way)) throw TH_ERROR "You need use Shekspear's names! (%s - invalid)", Str);

        ReadTo (Program, "\n");

        ShData.StartPos = ftell (Program);
    }

    GetInfoAct (Program, ShData, Str);

    if (!ReadShekspearWord (Program, Str, ShData.Way)) return;

    GetInfoAll (Program, ShData, Str);
}


void GetInfoAll (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    GetInfoAllOnce (Program, ShData, Str);

    while (true)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) break;
        GetInfoAllOnce (Program, ShData, Str);
    }
}

void GetInfoAllOnce (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "act") == 0)
    {
        GetInfoAct (Program, ShData, Str);

        return;
    }
    if (strcmp (Str, "scene") == 0)
    {
        GetInfoScene (Program, ShData, Str);

        return;
    }
    if (strcmp (Str, "[") == 0)
    {
        newTree Tree;
        GetOnScene (Tree, Program, ShData, Str);

        return;
    }
    if (InList ("character", Str, ShData.Way))
    {
        GetInfoSpeech (Program, ShData, Str);

        return;
    }

    throw TH_ERROR "How interpritate %s?", Str);
}

void GetInfoAct (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "act") == 0)
    {
        if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot act number");

        if (strcmp (Str, "in") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot place name after in");
            if (!InList ("places", Str, ShData.Way))           throw TH_ERROR "After in you need place name");

            ShData.Funcs.Add (Str);
        }
        else
        {
            ShData.Act = GetRoman (Str);
        }

        ReadTo (Program, "\n");

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot scene after act");
        GetInfoScene (Program, ShData, Str);

        return;
    }

    throw TH_ERROR "Need \"act\" to init act");
}

void GetInfoScene (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "scene") == 0)
    {
        if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot scene number or place.");

        if (strcmp (Str, "in") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot place name after in");
            if (!InList ("places", Str, ShData.Way))           throw TH_ERROR "After in you need place name");
        }
        else
        {
            ShData.Scene = GetRoman (Str);
        }

        ReadTo (Program, "\n");

        return;
    }

    throw TH_ERROR "Need \"scene\" to init scene");
}

void GetInfoSpeech (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "This actor(%s) wasn't planned", Str);
    if (!ShData.OnScene.Find (Str)) throw TH_ERROR "%s isn't on scene!", Str);

    strcpy (ShData.FirstPersonName, Str);
    if (ShData.OnScene.Size_ == 2)
    {
        ShData.SecondPersonExist = true;
        if (strcmp (ShData.FirstPersonName, &(ShData.OnScene.Data_ [0])[0]) == 0)
        {
            strcpy (ShData.SecondPersonName, &(ShData.OnScene.Data_ [1])[0]);
        }
        else
        {
            strcpy (ShData.SecondPersonName, &(ShData.OnScene.Data_ [0])[0]);
        }
    }
    else
    {
        ShData.SecondPersonExist = false;
    }

    if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Actor don't know, what to do!");
    if (strcmp (Str, ":") != 0) throw TH_ERROR "Actor can only speech.");

    if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Actor must say something.");
    if (!GetInfoPhrase (Program, ShData, Str)) throw TH_ERROR "Actor must say something.");
    ReadSentense (Program, ShData.Way);

    while (true)
    {
        int CopyP = ftell (Program);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) break;
        if (!GetInfoPhrase (Program, ShData, Str))
        {
            fseek (Program, CopyP, 0);
            break;
        }
        ReadSentense (Program, ShData.Way);
    }
}

bool GetInfoPhrase (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (InList ("character", Str, ShData.Way) || strcmp (Str, "[") == 0 || strcmp (Str, "act") == 0 || strcmp (Str, "scene") == 0)
    {
        return false;
    }

    if (strcmp (Str, "remember") == 0 || strcmp (Str, "recall") == 0)
    {
        GetInfoStackOp (Program, ShData, Str);

        return true;
    }
    if (strcmp (Str, "open") == 0 || strcmp (Str, "speak") == 0 || strcmp (Str, "listen") == 0)
    {
        GetInfoInOut (Program, ShData, Str);

        return true;
    }

    if (strcmp (Str, "let") == 0 || strcmp (Str, "we") == 0)
    {
        GetInfoCallGoto (Program, ShData, Str);

        return true;
    }

    if (strcmp (Str, "if") == 0 || InList ("be", Str, ShData.Way))
    {
        GetInfoIf (Program, ShData, Str);

        return true;
    }

    if (InList ("first_person", Str, ShData.Way) || InList ("second_person", Str, ShData.Way))
    {
        return true;
    }

    return false;
}

void GetInfoStackOp (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "remember") == 0 || strcmp (Str, "recall") == 0)
    {
        if (!ShData.SecondPersonExist) throw TH_ERROR "Talk to whoom?");

        if (!ShData.Stacks.Find (ShData.SecondPersonName))
        {
            ShData.Vars.Rem (ShData.SecondPersonName);
            ShData.Stacks.Add (ShData.SecondPersonName);
        }

        return;
    }
}

void GetInfoInOut (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "open") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Open what?");
        if (!InList ("second_person_possessive", Str, ShData.Way)) throw TH_ERROR "You need possessive word after open", Str);
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Open what? Heart? Mind? Eyes?");

        if (strcmp (Str, "heart") == 0)
        {
            ShData.UseNumOut = true;
        }

        return;
    }
    if (strcmp (Str, "listen") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Listen what?");
        if (strcmp (Str, "to") != 0) throw TH_ERROR "Actor can only listen to.");
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Listen to what?");
        if (!InList ("second_person_possessive", Str, ShData.Way)) throw TH_ERROR "You need possessive word after speak", Str);
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Listen to what? ");
        if (strcmp (Str, "heart") != 0) throw TH_ERROR "Actor can only listen to him/her mind");

        ShData.UseNumIn = true;

        return;
    }
}

void GetInfoCallGoto (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "let") == 0 || strcmp (Str, "we") == 0)
    {
        if (strcmp (Str, "let") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Let whoom?");
            if (strcmp (Str, "us") != 0) throw TH_ERROR "Maybe you mean \"Let us\"?");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Let us do what?");
        }
        if (strcmp (Str, "we") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "We... what to do?");
            if (strcmp (Str, "shall") != 0 && strcmp (Str, "must") != 0) throw TH_ERROR "We must or shell?");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "We Must/Shell what?");
        }

        if (strcmp (Str, "back") == 0)
        {
            if (!ShData.Funcs.Find (ShData.FunctionName)) ShData.Funcs.Add (ShData.FunctionName);

            return;
        }
        if (strcmp (Str, "return") != 0 && strcmp (Str, "proceed") != 0) throw TH_ERROR "Let us... I think we can oly return or proceed");
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And after?");
        if (strcmp (Str, "to") != 0) throw TH_ERROR "I think we can only proceed/return TO.");
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "To what?");
        if (InList ("places", Str, ShData.Way))
        {
             if (!ShData.Funcs.Find (Str) && !ShData.Marks.Find (Str)) ShData.Marks.Add (Str);
        }
        else
        {
            if (strcmp (Str, "scene") == 0)
            {
                if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "Scene number?");

                sprintf (Str, "%d|%d", ShData.Act, GetRoman (Str));
                if (!ShData.Marks.Find (Str)) ShData.Marks.Add (Str);
            }
            else if (strcmp (Str, "act") == 0)
            {
                if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "Act number?");

                sprintf (Str, "%d", GetRoman (Str));
                if (!ShData.Marks.Find (Str)) ShData.Marks.Add (Str);
            }
            else throw TH_ERROR "I think, we can only to proceed/return to scene/act");
        }

        return;
    }
}

void GetInfoIf (FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef INFO_DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    ShData.UseStatements = true;

    if (strcmp (Str, "if") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "If - what?");
        if (strcmp (Str, "so") != 0 && strcmp (Str, "not") != 0) throw TH_ERROR "If %s - I think it's incorrect.", Str);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What to do if true/false?");

        if (!GetInfoPhrase (Program, ShData, Str)) throw TH_ERROR "How I can do it?");

        return;
    }
}

//------------------------------------------------------------------------------

void CreateTree (newTree& Tree, FILE* Program, newShekspearData& ShData)
{
    Tree.PushP ();

    fseek (Program, ShData.StartPos, 0);

    Tree.Set (newNodeData (N_NODE));
    Tree.DownL ();
    AddVarsStacks (Tree, ShData);

    Add (Tree);
    AddNumInOut (Tree, ShData);

    char Str [60] = "";

    if (!ReadShekspearWord (Program, Str, ShData.Way)) return;

    Add (Tree);
    GetAct (Tree, Program, ShData, Str);

    if (!ReadShekspearWord (Program, Str, ShData.Way)) return;

    Add (Tree);
    GetAll (Tree, Program, ShData, Str);

    Free (Tree);

    Tree.PopP ();
}

void AddVarsStacks (newTree& Tree, newShekspearData& ShData)
{
    newNodeData Data;

    if (ShData.Vars.Size_ > 0)
    {
        if (ShData.Stacks.Size_ > 0)
        {
            Tree.Set (newNodeData (N_NODE));
            Tree.DownL ();
        }

        Tree.PushP ();

        Tree.DownL ();

        if (ShData.UseStatements)
        {
            Data = newNodeData (N_VAR);
            strcpy (Data.Name, "StatVar");
            Tree.Set (Data);
        }
        else
        {
            Data = newNodeData (N_VAR);
            strcpy (Data.Name, &(ShData.Vars.Data_ [0])[0]);
            Tree.Set (Data);
        }

        for (int i = (ShData.UseStatements)? 0 : 1; i < ShData.Vars.Size_; i ++)
        {
            Tree.Up ();

            Tree.Set (newNodeData (N_NODE));
            Tree.DownR ();
            Tree.DownL ();

            Data = newNodeData (N_VAR);
            strcpy (Data.Name, &(ShData.Vars.Data_ [i])[0]);
            Tree.Set (Data);
        }

        if (ShData.Vars.Size_ > 1) UpBranch (Tree);

        Tree.PopP ();

        Tree.Set (newNodeData (N_NEW));

        if (ShData.Stacks.Size_ > 0)
        {
            Tree.Up ();
        }
    }
    if (ShData.Stacks.Size_ > 0)
    {
        if (ShData.Vars.Size_ > 0)
        {
            Tree.Set (newNodeData (N_NODE));
            Tree.DownR ();
        }

        Tree.PushP ();

        Tree.DownL ();

        Data = newNodeData (N_ARR);
        strcpy (Data.Name, &(ShData.Stacks.Data_ [0])[0]);
        Tree.Set (Data);
        Tree.DownL ();
        Tree.Set (newNodeData (N_NUM, 100));
        Tree.Up ();
        Tree.DownR ();
        Tree.DownR ();
        Tree.DownL ();
        Tree.Set (newNodeData (N_NUM, 0));
        Tree.Up ();
        Tree.DownR ();
        Tree.Set (newNodeData (N_NUM, 2));
        Tree.Up ();
        Tree.Up ();
        Tree.Up ();

        for (int i = 1; i < ShData.Stacks.Size_; i ++)
        {
            Tree.Up ();

            Tree.Set (newNodeData (N_NODE));
            Tree.DownR ();
            Tree.DownL ();

            Data = newNodeData (N_ARR);
            strcpy (Data.Name, &(ShData.Stacks.Data_ [i])[0]);
            Tree.Set (Data);
            Tree.DownL ();
            Tree.Set (newNodeData (N_NUM, 100));
            Tree.Up ();
            Tree.DownR ();
            Tree.DownR ();
            Tree.DownL ();
            Tree.Set (newNodeData (N_NUM, 0));
            Tree.Up ();
            Tree.DownR ();
            Tree.Set (newNodeData (N_NUM, 2));
            Tree.Up ();
            Tree.Up ();
            Tree.Up ();
        }

        if (ShData.Stacks.Size_ > 1) UpBranch (Tree);

        Tree.PopP ();

        Tree.Set (newNodeData (N_NEW));

        if (ShData.Vars.Size_ > 0)
        {
            Tree.Up ();
        }
    }
}

void AddNumInOut (newTree& Tree, newShekspearData& ShData)
{
    if (ShData.UseNumIn)
    {
        if (ShData.UseNumOut)
        {
            Tree.Set (newNodeData (N_NODE));
            Tree.DownL ();
        }

        CopyTreeToTree (ShData.NumIn, Tree);

        if (ShData.UseNumOut)
        {
            Tree.Up ();
        }
    }
    if (ShData.UseNumOut)
    {
        if (ShData.UseNumIn)
        {
            Tree.Set (newNodeData (N_NODE));
            Tree.DownR ();
        }

        CopyTreeToTree (ShData.NumOut, Tree);

        if (ShData.UseNumIn)
        {
            Tree.Up ();
        }
    }
}

void Add (newTree& Tree)
{
    if (Tree.Get ().Descriptor != N_NODE || Tree.CanDownL () || Tree.CanDownR ())
    {
        Tree.Up ();
        Tree.DownR ();
        Tree.DownL ();
    }
}

void Free (newTree& Tree)
{
    if (Tree.Get ().Descriptor == N_NODE && !Tree.CanDownL () && !Tree.CanDownR ())
    {
        Tree.Delete ();

        Tree.PushP ();
        Tree.PosSave_.Swap ();
        Tree.PopP ();
        Tree.PushP ();
        Tree.PosSave_.Swap ();
        if (!Tree.CanDownL () && !Tree.CanDownR ())
        {
            Tree.PopP ();
        }
        else
        {
            Tree.PopP ();
            Tree.Up ();
            Tree.DownL ();
            UpBranch (Tree);
        }
    }
    else
    {
        UpBranch (Tree);
    }
}

int GetRoman (char Num [])
{
    const int  ArabArr  [13]    = { 1,   4,   5,   9,    10,  40,   50,  90,   100, 400,  500, 900,  1000};
    const char RomanArr [13][3] = {"i", "iv", "v", "ix", "x", "xl", "l", "xc", "c", "cd", "d", "cm", "m"};

    int Result = 0;

    int Numi = 0;
    int Arri = 12;
    while (Num [Numi] != 0)
    {
        if (Num [Numi] == RomanArr [Arri][0] && (RomanArr [Arri][1] == 0 || Num [Numi + 1] == RomanArr [Arri][1]))
        {
            Result += ArabArr [Arri];

            if (RomanArr [Arri][1] == 0) Numi += 1;
            else                         Numi += 2;
        }
        else
        {
            Arri --;
        }

        if (Arri == -1) throw TH_ERROR "%s - is not roman number!", Num);
    }

    return Result;
}

void GetAll (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    Tree.PushP ();

    Tree.DownL ();
    GetAllOnce (Tree, Program, ShData, Str);

    while (true)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) break;
        Add (Tree);
        GetAllOnce (Tree, Program, ShData, Str);

        if (ShData.Return)
        {
            ShData.Return = false;
            break;
        }
    }

    Free (Tree);
    Tree.PopP ();
}

void GetAllOnce (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "act") == 0)
    {
        GetAct (Tree, Program, ShData, Str);

        return;
    }
    if (strcmp (Str, "scene") == 0)
    {
        GetScene (Tree, Program, ShData, Str);

        return;
    }
    if (strcmp (Str, "[") == 0)
    {
        GetOnScene (Tree, Program, ShData, Str);

        return;
    }
    if (InList ("character", Str, ShData.Way))
    {
        GetSpeech (Tree, Program, ShData, Str);

        return;
    }

    throw TH_ERROR "How interpritate %s?", Str);
}

void GetAct (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "act") == 0)
    {
        if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot act number or name");

        if (strcmp (Str, "in") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot place name after in");
            if (!InList ("places", Str, ShData.Way))           throw TH_ERROR "After in you need place name");

            ReadTo (Program, "\n");

            Tree.DownL ();

            if (ShData.InFunc) throw TH_ERROR "You can't create function in function!");

            Tree.Set (newNodeData (N_NODE));

            ShData.InFunc = true;

            Tree.DownL ();
             Tree.Set (newNodeData (N_NEW));
             Tree.DownL ();
              newNodeData Data = newNodeData (N_FUN);
              strcpy (Data.Name, Str);
              Tree.Set (Data);
             Tree.Up ();
            Tree.Up ();

            Tree.DownR ();
             Tree.Set (Data);
            Tree.Up ();

            Tree.DownL ();
             Tree.DownL ();
              Tree.DownR ();
              if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What do in fuction?");
              GetAll (Tree, Program, ShData, Str);
              Tree.Up ();
             Tree.Up ();
            Tree.Up ();

            ShData.InFunc = false;

            Tree.Up ();

            sprintf (Str, "%d", ShData.Act);
            if (ShData.Marks.Find (Str))
            {
                Tree.DownR ();

                Data = newNodeData (N_MARK);
                strcpy (Data.Name, Str);
                Tree.Set (Data);

                Tree.Up ();
            }
            else
            {
                Tree.DownL ();
                UpBranch (Tree);
            }

            return;
        }

        ShData.Act = GetRoman (Str);

        ReadTo (Program, "\n");

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot scene after act");
        GetScene (Tree, Program, ShData, Str);

        if (ShData.InFunc)
        {
            ShData.Return = true;

            return;
        }

        sprintf (Str, "%d", ShData.Act);
        if (ShData.Marks.Find (Str))
        {
            newNodeData Data = newNodeData (N_MARK);
            strcpy (Data.Name, Str);
            Tree.Set (Data);
        }

        return;
    }

    throw TH_ERROR "Need \"act\" to init act");
}

void GetScene (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "scene") == 0)
    {
        if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot scene number or place.");

        if (strcmp (Str, "in") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot place name after in");
            if (!InList ("places", Str, ShData.Way))           throw TH_ERROR "After in you need place name");

            ReadTo (Program, "\n");

            if (ShData.Marks.Find (Str))
            {
                newNodeData Data = newNodeData (N_MARK);
                strcpy (Data.Name, Str);
                Tree.Set (Data);
            }

            return;
        }

        ShData.Scene = GetRoman (Str);

        sprintf (Str, "%d|%d", ShData.Act, ShData.Scene);
        if (ShData.Marks.Find (Str))
        {
            newNodeData Data = newNodeData (N_MARK);
            strcpy (Data.Name, Str);
            Tree.Set (Data);
        }

        ReadTo (Program, "\n");

        return;
    }

    throw TH_ERROR "Need \"scene\" to init scene");
}

void GetOnScene (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot what to do actors after [!");

    if (strcmp (Str, "enter") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot, who enter!");

        if (strcmp (Str, "]") == 0) throw TH_ERROR "You forgot, who enter(not [enter])!");

        if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "Enter unknown actor %s!", Str);
        if (ShData.OnScene.Find (Str)) throw TH_ERROR "%s is already on scene!", Str);
        ShData.OnScene.Add (Str);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot at least ] after enter actor's name!");

        while (strcmp (Str, "]") != 0)
        {
            if (strcmp (Str, "and") != 0) throw TH_ERROR "Need and betwen actors!");

            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And what after and?");

            if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "Enter unknown actor %s!", Str);
            if (ShData.OnScene.Find (Str)) throw TH_ERROR "%s is already on scene!", Str);
            ShData.OnScene.Add (Str);

            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot at least ] after enter actor's name!");
        }

        return ;
    }

    if (strcmp (Str, "exit") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot, who exit!");

        if (strcmp (Str, "]") == 0) throw TH_ERROR "You forgot, who exit(not [exit])!");

        if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "Exit unknown actor %s!", Str);
        if (!ShData.OnScene.Find (Str)) throw TH_ERROR "%s isn't on scene!", Str);
        ShData.OnScene.Rem (Str);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot at least ] after exit actor's name!");

        while (strcmp (Str, "]") != 0)
        {
            if (strcmp (Str, "and") != 0) throw TH_ERROR "Need and betwen actors!");

            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And what after and?");

            if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "Exit unknown actor %s!", Str);
            if (!ShData.OnScene.Find (Str)) throw TH_ERROR "%s isn't on scene!", Str);
            ShData.OnScene.Rem (Str);

            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot at least ] after enter actor's name!");
        }

        return ;
    }

    if (strcmp (Str, "exeunt") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot, who exeunt!");

        if (strcmp (Str, "]") == 0)
        {
            ShData.OnScene.Empty ();

            return ;
        }

        if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "Exeunt unknown actor %s!", Str);
        if (!ShData.OnScene.Find (Str)) throw TH_ERROR "%s isn't on scene!", Str);
        ShData.OnScene.Rem (Str);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot at least ] after exeunt actor's name!");

        while (strcmp (Str, "]") != 0)
        {
            if (strcmp (Str, "and") != 0) throw TH_ERROR "Need and betwen actors!");

            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And what after and?");

            if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "Exeunt unknown actor %s!", Str);
            if (!ShData.OnScene.Find (Str)) throw TH_ERROR "%s isn't on scene!", Str);
            ShData.OnScene.Rem (Str);

            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "You forgot at least ] after enter actor's name!");
        }

        return ;
    }

    if (strcmp (Str, "]") == 0) return ;

    throw TH_ERROR "Actors don't know, what to do");
}

void GetSpeech (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (!ShData.Vars.Find (Str) && !ShData.Stacks.Find (Str)) throw TH_ERROR "This actor(%s) wasn't planned", Str);
    if (!ShData.OnScene.Find (Str)) throw TH_ERROR "%s isn't on scene!", Str);

    strcpy (ShData.FirstPersonName, Str);
    if (ShData.OnScene.Size_ == 2)
    {
        ShData.SecondPersonExist = true;
        if (strcmp (ShData.FirstPersonName, &(ShData.OnScene.Data_ [0])[0]) == 0)
        {
            strcpy (ShData.SecondPersonName, &(ShData.OnScene.Data_ [1])[0]);
        }
        else
        {
            strcpy (ShData.SecondPersonName, &(ShData.OnScene.Data_ [0])[0]);
        }
    }
    else
    {
        ShData.SecondPersonExist = false;
    }

    if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Actor don't know, what to do!");
    if (strcmp (Str, ":") != 0) throw TH_ERROR "Actor can only speech.");

    Tree.PushP ();

    Tree.DownL ();
    if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Actor must say something.");
    if (!GetPhrase (Tree, Program, ShData, Str)) throw TH_ERROR "Actor must say something.");
    ReadSentense (Program, ShData.Way);

    while (true)
    {
        int CopyP = ftell (Program);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) break;
        Add (Tree);
        if (!GetPhrase (Tree, Program, ShData, Str))
        {
            fseek (Program, CopyP, 0);
            break;
        }

        ReadSentense (Program, ShData.Way);

        if (ShData.Return) break;
    }

    Free (Tree);
    Tree.PopP ();
}

bool GetPhrase (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (InList ("character", Str, ShData.Way) || strcmp (Str, "[") == 0 || strcmp (Str, "act") == 0 || strcmp (Str, "scene") == 0)
    {
        return false;
    }

    if (strcmp (Str, "remember") == 0 || strcmp (Str, "recall") == 0)
    {
        GetStackOp (Tree, Program, ShData, Str);

        return true;
    }
    if (strcmp (Str, "open") == 0 || strcmp (Str, "speak") == 0 || strcmp (Str, "listen") == 0)
    {
        GetInOut (Tree, Program, ShData, Str);

        return true;
    }

    if (strcmp (Str, "let") == 0 || strcmp (Str, "we") == 0)
    {
        GetCallGoto (Tree, Program, ShData, Str);

        return true;
    }

    if (strcmp (Str, "if") == 0 || InList ("be", Str, ShData.Way))
    {
        GetIf (Tree, Program, ShData, Str);

        return true;
    }

    if (InList ("first_person", Str, ShData.Way) || InList ("second_person", Str, ShData.Way))
    {
        GetEquation (Tree, Program, ShData, Str);

        return true;
    }

    throw TH_ERROR "Program exeption unknown word %s", Str);
    return false;
}

void GetStackOp (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "remember") == 0 || strcmp (Str, "recall") == 0)
    {
        if (!ShData.SecondPersonExist) throw TH_ERROR "Talk to whoom?");

        Tree.Set (newNodeData (N_NODE));

        newNodeData Data;

        if (strcmp (Str, "remember") == 0) Tree.DownL ();
        else                               Tree.DownR ();

        Tree.Set (newNodeData (N_EQ));
        if (strcmp (Str, "remember") == 0) Tree.DownL ();
        else                               Tree.DownR ();
         Data.Descriptor = N_ARR;
         strcpy (Data.Name, ShData.SecondPersonName);
         Tree.Set (Data);
         Tree.DownL ();
          Tree.Set (Data);
          Tree.DownL ();
           Tree.Set (newNodeData (N_NUM, 1));
          Tree.Up ();
         Tree.Up ();
        Tree.Up ();
        if (strcmp (Str, "remember") == 0) Tree.DownR ();
        else                               Tree.DownL ();
         if (strcmp (Str, "remember") == 0)
         {
             GetFirstPerson (Tree, Program, ShData, Str);
         }
         else
         {
             GetSecondPerson (Tree, Program, ShData, Str);
         }
        Tree.Up ();

        Tree.Up ();

        if (strcmp (Str, "remember") == 0) Tree.DownR ();
        else                               Tree.DownL ();

        Tree.Set (newNodeData (N_EQ));
        Tree.DownL ();
         Tree.Set (Data);
         Tree.DownL ();
          Tree.Set (newNodeData (N_NUM, 1));
         Tree.Up ();
        Tree.Up ();
        Tree.DownR ();
         if (strcmp (Str, "remember") == 0) Tree.Set (newNodeData (N_SUM));
         else                               Tree.Set (newNodeData (N_SUB));
         Tree.DownL ();
          Tree.Set (Data);
          Tree.DownL ();
           Tree.Set (newNodeData (N_NUM, 1));
          Tree.Up ();
         Tree.Up ();
         Tree.DownR ();
          Tree.Set (newNodeData (N_NUM, 1));
         Tree.Up ();
        Tree.Up ();

        Tree.Up ();

        return;
    }
}

void GetFirstPerson (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (ShData.Vars.Find (ShData.FirstPersonName))
    {
        newNodeData Data = newNodeData (N_VAR);
        strcpy (Data.Name, ShData.FirstPersonName);
        Tree.Set (Data);
    }
    else if (ShData.Stacks.Find (ShData.FirstPersonName))
    {
        newNodeData Data = newNodeData (N_ARR);
        strcpy (Data.Name, ShData.FirstPersonName);
        Tree.Set (Data);

        Tree.DownL();
        Tree.Set (newNodeData (N_NUM, 0));
        Tree.Up ();
    }
    else throw TH_ERROR "Who are %s?", ShData.FirstPersonName);
}
void GetSecondPerson (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (!ShData.SecondPersonExist) throw TH_ERROR "Talk to whoom?");

    if (ShData.Vars.Find (ShData.SecondPersonName))
    {
        newNodeData Data = newNodeData (N_VAR);
        strcpy (Data.Name, ShData.SecondPersonName);
        Tree.Set (Data);
    }
    else if (ShData.Stacks.Find (ShData.SecondPersonName))
    {
        newNodeData Data = newNodeData (N_ARR);
        strcpy (Data.Name, ShData.SecondPersonName);
        Tree.Set (Data);

        Tree.DownL();
        Tree.Set (newNodeData (N_NUM, 0));
        Tree.Up ();
    }
    else throw TH_ERROR "Who are %s?", ShData.SecondPersonName);
}

void GetInOut (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "open") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Open what?");
        if (!InList ("second_person_possessive", Str, ShData.Way)) throw TH_ERROR "You need possessive word after open", Str);
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Open what? Heart? Mind? Eyes?");

        if (strcmp (Str, "heart") == 0)
        {
            newNodeData Data = newNodeData (N_FUN);
            strcpy (Data.Name, "NumOut");
            Tree.Set (Data);
            Tree.DownL ();
            GetSecondPerson (Tree, Program, ShData, Str);
            Tree.Up ();
        }
        else if (strcmp (Str, "mind") == 0)
        {
            Tree.Set (newNodeData (N_GET));
            Tree.DownL ();
            GetSecondPerson (Tree, Program, ShData, Str);
            Tree.Up ();
        }
        else throw TH_ERROR "Actor can only open heart or mind");

        return;
    }
    if (strcmp (Str, "speak") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Speak what?");
        if (!InList ("second_person_possessive", Str, ShData.Way)) throw TH_ERROR "You need possessive word after speak", Str);
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Speak what? Speech?");
        if (strcmp (Str, "mind") != 0) throw TH_ERROR "Actor can only speak him/her mind");

        Tree.Set (newNodeData (N_ECHO));
        Tree.DownL ();
        GetSecondPerson (Tree, Program, ShData, Str);
        Tree.Up ();

        return;
    }
    if (strcmp (Str, "listen") == 0)
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Listen what?");
        if (strcmp (Str, "to") != 0) throw TH_ERROR "Actor can only listen to.");
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Listen to what?");
        if (!InList ("second_person_possessive", Str, ShData.Way)) throw TH_ERROR "You need possessive word after speak", Str);
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Listen to what? ");
        if (strcmp (Str, "heart") != 0) throw TH_ERROR "Actor can only listen to him/her mind");

        newNodeData Data = newNodeData (N_FUN);
        strcpy (Data.Name, "NumIn");
        Tree.Set (Data);
        Tree.DownL ();
        Tree.Set (newNodeData (N_LNK));
        Tree.DownL ();
        GetSecondPerson (Tree, Program, ShData, Str);
        Tree.Up ();
        Tree.Up ();

        return;
    }
}

void GetCallGoto (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "let") == 0 || strcmp (Str, "we") == 0)
    {
        if (strcmp (Str, "let") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Let whoom?");
            if (strcmp (Str, "us") != 0) throw TH_ERROR "Maybe you mean \"Let us\"?");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Let us do what?");
        }
        if (strcmp (Str, "we") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "We... what to do?");
            if (strcmp (Str, "shall") != 0 && strcmp (Str, "must") != 0) throw TH_ERROR "We must or shell?");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "We Must/Shell what?");
        }

        if (strcmp (Str, "back") == 0)
        {
            if (!ShData.InFunc) throw TH_ERROR "You can return only in function");

            Tree.Set (newNodeData (N_RETURN));
            Tree.DownL ();
            Tree.Set (newNodeData (N_NUM, 0));
            Tree.Up ();

            return;
        }
        if (strcmp (Str, "return") != 0 && strcmp (Str, "proceed") != 0) throw TH_ERROR "Let us... I think we can only return or proceed");
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And after?");
        if (strcmp (Str, "to") != 0) throw TH_ERROR "I think we can only proceed/return TO.");
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "To what?");
        if (InList ("places", Str, ShData.Way))
        {
             if (ShData.Funcs.Find (Str))
             {
                 newNodeData Data = newNodeData (N_FUN);
                 strcpy (Data.Name, Str);
                 Tree.Set (Data);
             }
             else if (ShData.Marks.Find (Str))
             {
                 newNodeData Data = newNodeData (N_GOTO);
                 strcpy (Data.Name, Str);
                 Tree.Set (Data);
             }
             else throw TH_ERROR "Act/scene %s wasn't declared", Str);
        }
        else
        {
            if (strcmp (Str, "scene") == 0)
            {
                if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "Scene number?");

                newNodeData Data = newNodeData (N_GOTO);
                sprintf (Data.Name, "%d|%d", ShData.Act, GetRoman (Str));
                Tree.Set (Data);
            }
            else if (strcmp (Str, "act") == 0)
            {
                if (!ReadWord (Program, Str, ShData.Way)) throw TH_ERROR "Act number?");

                newNodeData Data = newNodeData (N_GOTO);
                sprintf (Data.Name, "%d", GetRoman (Str));
                Tree.Set (Data);
            }
            else throw TH_ERROR "I think, we can only to proceed/return to scene/act");
        }

        return;
    }
}

void GetIf (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (InList ("be", Str, ShData.Way))
    {
        newNodeData Data;

        Tree.Set (newNodeData (N_EQ));
        Tree.DownL ();
        Data.Descriptor = N_VAR;
        strcpy (Data.Name, "StatVar");
        Tree.Set (Data);
        Tree.Up ();

        Tree.DownR ();

        Tree.DownL ();
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What?");
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And what?");
        bool Not = strcmp (Str, "not") == 0;
        if (Not && !ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Not what?");

        if (strcmp (Str, "as") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "As... what?");
            if (!InList ("positive_adjective", Str, ShData.Way) && !InList ("neutral_adjective", Str, ShData.Way) && !InList ("negative_adjective", Str, ShData.Way)) throw TH_ERROR "I think as %s as - is incorrect.", Str);
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "As adj... and after?");
            if (strcmp (Str, "as") != 0) throw TH_ERROR "Need as after adjective");

            if (!Not) Tree.Set (newNodeData (N_OPEQ));
            else      Tree.Set (newNodeData (N_NONEQ));
        }
        else if (InList ("positive_comparative", Str, ShData.Way))
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Than who/what?");
            if (strcmp (Str, "than") != 0) throw TH_ERROR "Only than is correct.");

            if (!Not) Tree.Set (newNodeData (N_MORE));
            else      Tree.Set (newNodeData (N_LESS));
        }
        else if (InList ("negative_comparative", Str, ShData.Way))
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Than who/what?");
            if (strcmp (Str, "than") != 0) throw TH_ERROR "Only than is correct.");

            if (!Not) Tree.Set (newNodeData (N_LESS));
            else      Tree.Set (newNodeData (N_MORE));
        }
        else throw TH_ERROR "Unknown compare %s", Str);

        Tree.DownR ();
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "and what?");
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        Tree.Up ();

        return;
    }

    if (strcmp (Str, "if") == 0)
    {
        newNodeData Data;

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "If - what?");
        if (strcmp (Str, "so") != 0 && strcmp (Str, "not") != 0) throw TH_ERROR "If %s - I think it's incorrect.", Str);

        Tree.Set (newNodeData (N_IF));
        Tree.DownL ();

        Data.Descriptor = N_VAR;
        strcpy (Data.Name, "StatVar");

        if (strcmp (Str, "not") == 0)
        {
            Tree.Set (newNodeData (N_NOT));
            Tree.DownL ();
            Tree.Set (Data);
            Tree.Up ();
        }
        else Tree.Set (Data);
        Tree.Up ();

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What to do if true/false?");

        Tree.DownR ();
        if (!GetPhrase (Tree, Program, ShData, Str)) throw TH_ERROR "How I can do it?");
        Tree.Up ();

        return;
    }
}

void GetInstance (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (strcmp (Str, "sum") == 0 || strcmp (Str, "difference") == 0 || strcmp (Str, "product") == 0 || strcmp (Str, "quotient") == 0 || strcmp (Str, "remainder") == 0)
    {
        if (strcmp (Str, "sum") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s what?", Str);
            if (strcmp (Str, "of") != 0) throw TH_ERROR "After sum you need \"of\"");
            Tree.Set (newNodeData (N_SUM));
        }
        if (strcmp (Str, "difference") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s what?", Str);
            if (strcmp (Str, "between") != 0) throw TH_ERROR "After difference you need \"between\"");
            Tree.Set (newNodeData (N_SUB));
        }
        if (strcmp (Str, "product") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s what?", Str);
            if (strcmp (Str, "of") != 0) throw TH_ERROR "After product you need \"of\"");
            Tree.Set (newNodeData (N_MUL));
        }
        if (strcmp (Str, "quotient") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s what?", Str);
            if (strcmp (Str, "between") != 0) throw TH_ERROR "After quotient you need \"between\"");
            Tree.Set (newNodeData (N_DIV));
        }
        if (strcmp (Str, "remainder") == 0)
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s what?", Str);
            if (strcmp (Str, "of") != 0) throw TH_ERROR "After remainder you need \"of\"");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s of what?", Str);
            if (strcmp (Str, "quotient") != 0) throw TH_ERROR "After remainder of you need \"quotient\"");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s what?", Str);
            if (strcmp (Str, "between") != 0) throw TH_ERROR "After quotient you need \"between\"");
            Tree.Set (newNodeData (N_MOD));
        }

        Tree.DownL ();
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What?");
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "And what?");
        if (strcmp (Str, "and") != 0) throw TH_ERROR "You need \"and\"");

        Tree.DownR ();
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What?");
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        return;
    }

    if (strcmp (Str, "twice") == 0)
    {
        Tree.Set (newNodeData (N_MUL));
        Tree.DownL ();
        Tree.Set (newNodeData (N_NUM, 2));
        Tree.Up ();
        Tree.DownR ();
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "twice what?");
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        return;
    }

    if (strcmp (Str, "square") == 0 || strcmp (Str, "cube") == 0)
    {
        bool Square = strcmp (Str, "square") == 0;
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "square/cube - and what? Potato!");
        if (strcmp (Str, "of") != 0) throw TH_ERROR "You need \"of\" after square/cube");

        newTree Branch;
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Of what?");
        GetInstance (Branch, Program, ShData, Str);

        Tree.Set (newNodeData (N_MUL));
        Tree.DownL ();
        CopyTreeToTree (Branch, Tree);
        Tree.Up ();
        Tree.DownR ();
        if (Square)
        {
            CopyTreeToTree (Branch, Tree);
        }
        else
        {
            Tree.Set (newNodeData (N_MUL));
            Tree.DownL ();
            CopyTreeToTree (Branch, Tree);
            Tree.Up ();
            Tree.DownR ();
            CopyTreeToTree (Branch, Tree);
            Tree.Up ();
        }
        Tree.Up ();

        return;
    }

    if (InList ("positive_adjective", Str, ShData.Way) || InList ("neutral_adjective", Str, ShData.Way) || InList ("negative_adjective", Str, ShData.Way))
    {
        int CopyP = ftell (Program);

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s - who/what?", Str);

        int i = 1;
        while (InList ("positive_adjective", Str, ShData.Way) || InList ("neutral_adjective", Str, ShData.Way) || InList ("negative_adjective", Str, ShData.Way))
        {
            i ++;

            CopyP = ftell (Program);
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "%s - who/what?", Str);
        }

        fseek (Program, CopyP, 0);

        Tree.DownL ();
        Tree.Set (newNodeData (N_NUM, pow (2.0, i)));
        Tree.Up ();

        Tree.Set (newNodeData (N_MUL));

        Tree.DownR ();
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What?");
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        return;
    }

    if (InList ("first_person", Str, ShData.Way) || InList ("first_person_reflexive", Str, ShData.Way))
    {
        GetFirstPerson (Tree, Program, ShData, Str);

        return;
    }
    if (InList ("second_person", Str, ShData.Way) || InList ("second_person_reflexive", Str, ShData.Way))
    {
        GetSecondPerson (Tree, Program, ShData, Str);

        return;
    }

    if (InList ("first_person_possessive", Str, ShData.Way) || InList ("second_person_possessive", Str, ShData.Way) || InList ("third_person_possessive", Str, ShData.Way))
    {
        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "What?");
        GetInstance (Tree, Program, ShData, Str);

        return;
    }

    if (InList ("nothing", Str, ShData.Way))
    {
        Tree.Set (newNodeData (N_NUM, 0));

        return;
    }

    if (InList ("positive_noun", Str, ShData.Way) || InList ("neutral_noun", Str, ShData.Way))
    {
        Tree.Set (newNodeData (N_NUM, 1));

        return;
    }
    if (InList ("negative_noun", Str, ShData.Way))
    {
        Tree.Set (newNodeData (N_NUM, -1));

        return;
    }

    if (ShData.Vars.Find (Str))
    {
        newNodeData Data = newNodeData (N_VAR);
        strcpy (Data.Name, Str);
        Tree.Set (Data);

        return;
    }
    if (ShData.Stacks.Find (Str))
    {
        newNodeData Data = newNodeData (N_ARR);
        strcpy (Data.Name, Str);
        Tree.Set (Data);

        Tree.DownL();
        Tree.Set (newNodeData (N_NUM, 0));
        Tree.Up ();

        return;
    }

    throw TH_ERROR "%s - I don't think it's correct.", Str);
}

void GetEquation (newTree& Tree, FILE* Program, newShekspearData& ShData, char Str [])
{
    #ifdef DEBUG
        printf ("%s: Str = \"%s\"\n", __PRETTY_FUNCTION__, Str);
        #ifdef TREESAVE
            FastDebugSave (Tree, "..\\..\\Save.tree");
        #endif
        #ifdef PAUSE
            system ("pause");
        #endif
    #endif

    if (InList ("first_person", Str, ShData.Way) || InList ("second_person", Str, ShData.Way))
    {
        Tree.Set (newNodeData (N_EQ));
        Tree.DownL ();
        if (InList ("first_person", Str, ShData.Way)) GetFirstPerson (Tree, Program, ShData, Str);
        else                              GetSecondPerson (Tree, Program, ShData, Str);
        Tree.Up ();

        if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Equal what/whoom?");
        if (InList ("be", Str, ShData.Way))
        {
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "As?");
            if (strcmp (Str, "as") != 0) throw TH_ERROR "Only ~be?");
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "As... And what?");
            if (!InList ("positive_adjective", Str, ShData.Way) && !InList ("neutral_adjective", Str, ShData.Way) && !InList ("negative_adjective", Str, ShData.Way)) throw TH_ERROR "As %s as - I think, it's incorrect.", Str);
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "Sudden end after as adjective");
            if (strcmp (Str, "as") != 0) throw TH_ERROR "as adj %s - I think, it's incorrect.", Str);
            if (!ReadShekspearWord (Program, Str, ShData.Way)) throw TH_ERROR "As adj as... and what?");
        }

        Tree.DownR ();
        GetInstance (Tree, Program, ShData, Str);
        Tree.Up ();

        return;
    }
}

#endif
