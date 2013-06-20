#ifndef TREEOPERATIONS__
#define TREEOPERATIONS__

#include "Tree.h"
#include "..\..\_SystemLibs\Throw\Throw.h"

//==============================================================================

void CopyTreeToTree    (newTree& FromTree, newTree& ToTree);

//------------------------------------------------------------------------------

void DownBranchL (newTree& Tree);
void DownBranchR (newTree& Tree);

void UpBranch   (newTree& Tree);

//------------------------------------------------------------------------------

void FastDebugSave (newTree& Tree, char Name[]);

//------------------------------------------------------------------------------

int GetListSize (newTree& Tree);

//------------------------------------------------------------------------------

void RemoveFreeNodes (newTree& Tree);

//==============================================================================

void CopyTreeToTree (newTree& FromTree, newTree& ToTree)
{
    ToTree.Set (FromTree.Get ()); 
    
    if (ToTree.CanDownL ())
    {
        ToTree.DownL ();                
        ToTree.Delete (true);                    
    }
    if (ToTree.CanDownR ())
    {
        ToTree.DownR ();                
        ToTree.Delete (true);                    
    }
      
    if (FromTree.CanDownL ())
    {
        FromTree.DownL ();
        ToTree.  DownL ();
        
        CopyTreeToTree (FromTree, ToTree);
        
        FromTree. Up ();
        ToTree.   Up ();                     
    }
    if (FromTree.CanDownR ())
    {
        FromTree.DownR ();
        ToTree.  DownR ();
        
        CopyTreeToTree (FromTree, ToTree);
        
        FromTree. Up ();
        ToTree.   Up ();                     
    }     
}

//------------------------------------------------------------------------------

void DownBranchL (newTree& Tree)
{
    newTree TreeCopy;
    CopyTreeToTree (Tree, TreeCopy);
    
    Tree.DownL ();
    CopyTreeToTree (TreeCopy, Tree);   
    Tree.Up (); 
}
void DownBranchR (newTree& Tree)
{
    newTree TreeCopy;
    CopyTreeToTree (Tree, TreeCopy);
    
    Tree.DownR ();
    CopyTreeToTree (TreeCopy, Tree);   
    Tree.Up (); 
}

void UpBranch (newTree& Tree)
{
    newTree TreeCopy;
    CopyTreeToTree (Tree, TreeCopy);
    
    Tree.Delete (true);
    
    CopyTreeToTree (TreeCopy, Tree);    
}

//------------------------------------------------------------------------------

void FastDebugSave (newTree& Tree, char Name[])
{
    FILE* Save = fopen (Name, "wb");
    
    Tree.SaveToFile (Save);
    
    fclose (Save);
}

//------------------------------------------------------------------------------

int GetListSize (newTree& Tree)
{
    int Result = 0;
    if (Tree.Get ().Descriptor != N_NODE || !Tree.CanDownL ()) Result ++;
    else
    {
        if (Tree.CanDownL ())
        {
            Tree.DownL ();                
            Result += GetListSize (Tree);
            Tree.Up ();                   
        }
        if (Tree.CanDownR ())
        {
            Tree.DownR ();                
            Result += GetListSize (Tree);
            Tree.Up ();                   
        }
    }        
    
    return Result;
}

//------------------------------------------------------------------------------

void RemoveFreeNodes (newTree& Tree)
{
    Tree.PushP (); 
    
    bool Change = true;
    
    while (Change)
    { 
        Change = false;
          
        for (int i = 1; i < Tree.Size (); i ++)
        {
            Tree.CurrentNode (i);
        
            if (Tree.Get ().Descriptor == N_NODE && !Tree.CanDownL () && !Tree.CanDownR ())
            {
                Tree.Delete ();
                
                Change = true;            
            }
        }     
    }     
    
    Tree.PopP ();     
}

#endif
