#ifndef TREE__
#define TREE__

#include "cstdio"
#include "cmath"
#include "string"
#include "memory"
#include "..\..\_SystemLibs\Stack\Stack.h"
#include "..\..\_SystemLibs\Throw\Throw.h"

#ifndef TREE_VERSION
#error No tree version defined!
#endif

using std::string;

//==============================================================================

enum BaseNodeDescriptors
{
    N_NODE = 0
};

//==============================================================================

//Data for node
struct newNodeData
{
    int Descriptor;

    double Data;
    char Name [30];

    newNodeData ();
    newNodeData (int NDescriptor);
    newNodeData (int NDescriptor, double NData);

    void         SetName (const char NName []);
    const char* GetName ();
};

newNodeData::newNodeData () :
    Descriptor (N_NODE),
    Data       (-1),
    Name       ()
{}
newNodeData::newNodeData (int NDescriptor) :
    Descriptor (NDescriptor),
    Data       (0),
    Name       ()
{}
newNodeData::newNodeData (int NDescriptor, double NData) :
    Descriptor (NDescriptor),
    Data       (NData),
    Name       ()
{}

void newNodeData::SetName (const char NName [])
{
    strcpy (Name, NName);
}

const char* newNodeData::GetName ()
{
    return Name;
}

//==============================================================================

void DumpOut (newNodeData* Node);

void CopyObj (newNodeData*  To, newNodeData*  From, int Size);

//==============================================================================

//Dump function for vector optimization
void DumpOut (newNodeData* Node)
{
    printf ("Descriptor = %d, Data = %g, Name = %s", (*Node).Descriptor, (*Node).Data, (*Node).GetName ());
}

//Copy function for fast vector update
void CopyObj (newNodeData*  To, newNodeData*  From, int Size)
{
    StandartCopyObj (To, From, Size);
}

//------------------------------------------------------------------------------

struct newNode
{
    //For fast tree
    int Parent;

    int Left;
    int Right;

    int Level;

    newNodeData Data;

    newNode ();
};

newNode::newNode () :
    Parent (-1),

    Left   (-1),
    Right  (-1),

    Level (0),

    Data ()
{}

//------------------------------------------------------------------------------

bool WriteNodeToFile (FILE* File, newNode& Node);
bool ReadNodeFromFile (FILE* File, newNode& Node);
void DumpOut (newNode* Node);

void CopyObj (newNode*  To, newNode*  From, int Size);

//------------------------------------------------------------------------------

//Function for write format customization
bool WriteNodeToFile (FILE* File, newNode& Node)
{
    if (!File)  return false;
    if (fwrite (&Node, sizeof (Node), 1, File) != 1) return false;

    return true;
}

//Function for write format customization
bool ReadNodeFromFile (FILE* File, newNode& Node)
{
    if (!File)  return false;
    if (fread (&Node, sizeof (Node), 1, File) != 1) return false;

    return true;
}

//Dump function for vector optimization
void DumpOut (newNode* Node)
{
    printf ("%d/ |%d| \\%d ", (*Node).Left, (*Node).Parent, (*Node).Right);
    DumpOut (&(*Node).Data);
}

//Copy function for fast vector update
void CopyObj (newNode*  To, newNode*  From, int Size)
{
    StandartCopyObj (To, From, Size);
}

//==============================================================================

class newTree
{
    private:
    newVector <newNode> Data_;
    int Size_;

    int CurrentNode_;

    public:
    newStack <int> PosSave_;

    newTree ();
    ~newTree ();

    bool OK ();
    void Dump (const char* Title = NULL);

    //Push current position
    void PushP ();
    //Pop current position
    void PopP  ();
    //Go to root
    void Root ();

    bool CanUp ();
    bool CanDownL ();
    bool CanDownR ();

    //Show level of current node
    int GetLevel ();

    void Up ();
    // !!! If can't, creates new node
    void DownL ();
    // !!! If can't, creates new node
    void DownR ();

    //Add left node
    void AddL   ();
    //Add right node
    void AddR   ();
    //Replace current node with end node
    void Insert ();
    //Delete current node. If param == true, delete whole branch
    void Delete (bool AllUnderTree = false);

    //Get current node data
    newNodeData& Get ();
    //Get current node data
    void         Set (newNodeData NodeData);

    //Shows, if this tree empty
    bool Empty ();

    //Return size of tree
    int Size ();

    //Return current node number
    int CurrentNode ();
    //Set current node. !!Use carefully!! While using tree number of node can randomly change
    void CurrentNode (int Value);

    //Return node number i!!! Only carefull use!!!
    newNode GetNode (int i);

    void SaveToFile   (FILE* File);
    void LoadFromFile (FILE* File);
};

//------------------------------------------------------------------------------

newTree::newTree () :
    Data_ (),

    Size_ (1),
    CurrentNode_ (0),

    PosSave_ ()
{
    Data_ [0].Level = 0;

    assert (OK ());
}

newTree::~newTree ()
{}

//------------------------------------------------------------------------------

bool newTree::OK ()
{
    if (Size_ < 0)             return false;
    if (CurrentNode_ < 0)      return false;
    if (CurrentNode_ >= Size_) return false;

    return Data_.OK () + PosSave_.OK ();
}

void newTree::Dump (const char* Title)
{
    printf ("=====Dump=====\n");
    if (Title != NULL) printf ("%s\n", Title);
    printf ("newTree//%s\n", (OK ())? "Good" : "BAD");
    printf ("{\n");
    printf ("    Size_        = %d;\n", Size_);
    printf ("    CurrentNode_ = %d;\n", CurrentNode_);
    printf ("    Data_        = \n");
    for (int i = 0; i < Size_; i ++)
    {
        printf ("    [%d] ", i);
        DumpOut (&Data_ [i]);
        printf ("\n");
    }
    printf ("    };\n");
    printf ("}\n");
}

//------------------------------------------------------------------------------

void newTree::PushP ()
{
    assert (OK ());

    PosSave_.Push (CurrentNode_);

    assert (OK ());
}

void newTree::PopP  ()
{
    assert (OK ());

    CurrentNode_ = PosSave_.Pop ();

    assert (OK ());
}

void newTree::Root ()
{
    assert (OK ());

    CurrentNode_ = 0;

    assert (OK ());
}

//------------------------------------------------------------------------------

bool newTree::CanUp ()
{
    assert (OK ());

    return Data_ [CurrentNode_].Parent != -1;
}
bool newTree::CanDownL ()
{
    assert (OK ());

    return Data_ [CurrentNode_].Left != -1;
}
bool newTree::CanDownR ()
{
    assert (OK ());

    return Data_ [CurrentNode_].Right != -1;
}

//------------------------------------------------------------------------------

int newTree::GetLevel ()
{
    assert (OK ());

    return Data_[CurrentNode_].Level;
}
//------------------------------------------------------------------------------

void newTree::Up ()
{
    assert (OK ());

    if (CurrentNode_ == 0)
    {
        throw TH_ERROR "Root, can't up.");
    }

    CurrentNode_ = Data_ [CurrentNode_].Parent;

    assert (OK ());
}

void newTree::DownL ()
{
    assert (OK ());

    int Left = Data_ [CurrentNode_].Left;

    if (Left != -1)
    {
        CurrentNode_ = Left;
    }
    else
    {
        AddL ();
    }

    assert (OK ());
}

void newTree::DownR ()
{
    assert (OK ());

    int Right = Data_ [CurrentNode_].Right;

    if (Right != -1)
    {
        CurrentNode_ = Right;
    }
    else
    {
        AddR ();
    }

    assert (OK ());
}

//------------------------------------------------------------------------------

void newTree::AddL   ()
{
    assert (OK ());

    if (Data_ [CurrentNode_].Left != -1)
    {
        Dump ("Because of error.");

        throw TH_ERROR "Left already exist");
    }

    Data_ [Size_] = newNode ();

    Data_ [Size_].Level = Data_ [CurrentNode_].Level + 1;

    Data_ [Size_].Parent = CurrentNode_;

    Data_ [CurrentNode_].Left = Size_;

    CurrentNode_ = Size_;

    Size_ ++;

    assert (OK ());
}

void newTree::AddR ()
{
    assert (OK ());

    if (Data_ [CurrentNode_].Right != -1)
    {
        Dump ("Because of error.");

        throw TH_ERROR "Right already exist");
    }

    Data_ [Size_] = newNode ();

    Data_ [Size_].Level = Data_ [CurrentNode_].Level + 1;

    Data_ [Size_].Parent = CurrentNode_;

    Data_ [CurrentNode_].Right = Size_;

    CurrentNode_ = Size_;

    Size_ ++;

    assert (OK ());
}

void newTree::Insert ()
{
    assert (OK ());

    if (CurrentNode_ == 0)
    {
        Dump ("Because of error.");

        throw TH_ERROR "Can't insert to root");
    }

    Size_ --;

    if      (Data_[Data_ [Size_].Parent].Left  == Size_) Data_[Data_ [Size_].Parent].Left  = CurrentNode_;
    else if (Data_[Data_ [Size_].Parent].Right == Size_) Data_[Data_ [Size_].Parent].Right = CurrentNode_;

    if (Data_ [Size_].Left  != -1) Data_[Data_ [Size_].Left ].Parent = CurrentNode_;
    if (Data_ [Size_].Right != -1) Data_[Data_ [Size_].Right].Parent = CurrentNode_;

    Data_ [CurrentNode_] = Data_ [Size_];

    PosSave_.Update (Size_, CurrentNode_);

    assert (OK ());
}

void newTree::Delete (bool AllUnderTree)
{
    assert (OK ());

    if (CurrentNode_ == 0)
    {
        Dump ("Because of error.");

        throw TH_ERROR "Can't delete root");
    }

    if (Data_ [CurrentNode_].Left != -1)
    {
        if (AllUnderTree)
        {
            DownL ();
            Delete (true);
        }
        else
        {
            Dump ("Because of error.");

            throw TH_ERROR "Can't delete node with childrens");
        }
    }
    if (Data_ [CurrentNode_].Right != -1)
    {
        if (AllUnderTree)
        {
            DownR ();
            Delete (true);
        }
        else
        {
            Dump ("Because of error.");

            throw TH_ERROR "Can't delete node with childrens");
        }
    }

    PosSave_.Update (CurrentNode_, -1);

    if      (Data_ [Data_ [CurrentNode_].Parent].Left  == CurrentNode_) Data_ [Data_ [CurrentNode_].Parent].Left  = -1;
    else if (Data_ [Data_ [CurrentNode_].Parent].Right == CurrentNode_) Data_ [Data_ [CurrentNode_].Parent].Right = -1;

    delete &Data_[CurrentNode_].Data;

    PosSave_.Push (Data_ [CurrentNode_].Parent);

    if (CurrentNode_ < Size_ - 1) Insert ();
    else                          Size_ --;

    CurrentNode_ = PosSave_.Pop ();

    assert (OK ());
}

//------------------------------------------------------------------------------

newNodeData& newTree::Get ()
{
    assert (OK ());

    return Data_ [CurrentNode_].Data;
}

void newTree::Set (newNodeData NodeData)
{
    assert (OK ());

    Data_ [CurrentNode_].Data = NodeData;

    assert (OK ());
}

//------------------------------------------------------------------------------

bool newTree::Empty ()
{
    assert (OK ());

    return (Size () == 1) && Get().Descriptor == N_NODE && !CanDownL() && !CanDownR();
}

//------------------------------------------------------------------------------

int newTree::Size ()
{
    return Size_;
}

//------------------------------------------------------------------------------

int newTree::CurrentNode ()
{
    return CurrentNode_;
}

void newTree::CurrentNode (int Value)
{
    CurrentNode_ = Value;

    assert (OK ());
}

//------------------------------------------------------------------------------

newNode newTree::GetNode (int i)
{
    assert (OK ());

    if (i < 0 || i >= Size_)
    {
        throw TH_ERROR "Invalid pos");
    }

    return Data_ [i];
}

//------------------------------------------------------------------------------

void newTree::SaveToFile (FILE* File)
{
    assert (OK ());

    int Version = TREE_VERSION;

    if (fwrite (&Version, sizeof (int), 1, File) != 1) throw TH_ERROR "Can't write");

    if (fwrite (&Size_,        sizeof (int), 1, File) != 1) throw TH_ERROR "Can't write");
    if (fwrite (&CurrentNode_, sizeof (int), 1, File) != 1) throw TH_ERROR "Can't write");

    for (int i = 0; i < Size_; i ++)
    {
        if (!WriteNodeToFile (File, Data_ [i])) throw TH_ERROR "Can't write");
    }

    assert (OK ());
}

void newTree::LoadFromFile (FILE* File)
{
    assert (OK ());

    int Version = 0;
    if (fread (&Version, sizeof (int), 1, File) != 1) throw TH_ERROR "Can't read");
    if (Version != TREE_VERSION) throw TH_ERROR "Uncompatible versions!");

    if (fread (&Size_,        sizeof (int), 1, File) != 1) throw TH_ERROR "Can't read");
    if (fread (&CurrentNode_, sizeof (int), 1, File) != 1) throw TH_ERROR "Can't read");


    for (int i = 0; i < Size_; i ++)
    {
        if (!ReadNodeFromFile (File, Data_ [i])) throw TH_ERROR "Can't read");
    }

    assert (OK ());
}

#endif
