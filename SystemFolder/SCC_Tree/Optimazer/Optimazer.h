#ifndef OPTIMAZER_H_
#define OPTIMAZER_H_

#include "..\Lib\Tree.h"
#include "..\Lib\BaseTreeDesc.h"
#include "..\Lib\TreeOperations.h"
#include "..\..\_SystemLibs\Stack\Stack.h"

//------------------------------------------------------------------------------

void Optimaze (newTree& Tree);

void OptimazeNode (newTree& Tree);

bool CalculateResult (newTree& Tree, double* Result);

//------------------------------------------------------------------------------

void Optimaze (newTree& Tree)
{
    OptimazeNode (Tree);
}

//------------------------------------------------------------------------------

void OptimazeNode (newTree& Tree)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_SUM || Data.Descriptor == N_SUB || Data.Descriptor == N_MUL || Data.Descriptor == N_DIV || Data.Descriptor == N_MOD)
    {
        double Result;
        if (CalculateResult (Tree, &Result))
        {
            Tree.DownL ();
            Tree.Delete (true);
            Tree.DownR ();
            Tree.Delete (true);
            Tree.Set (newNodeData (N_NUM, Result));
        }

        return ;
    }

    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        OptimazeNode (Tree);
        Tree.Up ();
    }
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        OptimazeNode (Tree);
        Tree.Up ();
    }
}

bool CalculateResult (newTree& Tree, double* Result)
{
    newNodeData Data = Tree.Get ();

    if (Data.Descriptor == N_NUM)
    {
        *Result = Data.Data;

        return true;
    }

    if (Data.Descriptor == N_VAR)
    {
        return false;
    }

    if (Data.Descriptor == N_ARR)
    {
        if (Tree.CanDownR ())
        {
            Tree.DownR ();
            double ArrResult;
            if (CalculateResult (Tree, &ArrResult))
            {
                Tree.DownL ();
                Tree.Delete (true);
                Tree.DownR ();
                Tree.Delete (true);
                Tree.Set (newNodeData (N_NUM, ArrResult));
            }
            Tree.Up ();
        }

        return false;
    }

    bool Return = true;

    double LResult;
    if (Tree.CanDownL ())
    {
        Tree.DownL ();
        if (CalculateResult (Tree, &LResult))
        {
            Tree.Delete (true);
            Tree.DownL ();
            Tree.Set (newNodeData (N_NUM, LResult));
        }
        else Return = false;
        Tree.Up ();
    }
    double RResult;
    if (Tree.CanDownR ())
    {
        Tree.DownR ();
        if (CalculateResult (Tree, &RResult))
        {
            Tree.Delete (true);
            Tree.DownR ();
            Tree.Set (newNodeData (N_NUM, RResult));
        }
        else Return = false;
        Tree.Up ();
    }

    if (Data.Descriptor == N_SUM)
    {
        *Result = LResult + RResult;
        return Return;
    }
    if (Data.Descriptor == N_SUB)
    {
        *Result = LResult - RResult;
        return Return;
    }
    if (Data.Descriptor == N_MUL)
    {
        *Result = LResult * RResult;
        return Return;
    }
    if (Data.Descriptor == N_DIV)
    {
        *Result = LResult / RResult;
        return Return;
    }
    if (Data.Descriptor == N_MOD)
    {
        *Result = (int)LResult % (int)RResult;
        return Return;
    }

    return false;
}

#endif
