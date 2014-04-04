#ifndef STACK_H_
    #define STACK_H_

#include "cstdio"
#include "iostream"
#include "cassert"

#include "cstring"

#include "..\Throw\Throw.h"

//==============================================================================

void DumpOut (int*       ToPrintf);
void DumpOut (char*      ToPrintf);
void DumpOut (char**     ToPrintf);
void DumpOut (double*    ToPrintf);
void DumpOut (float*     ToPrintf);
void DumpOut (bool*      ToPrintf);

template <typename Type>
void DumpOut (Type*   ToPrintf);

//==============================================================================

void CopyObj (int*    To, int*    From, int Size);
void CopyObj (char*   To, char*   From, int Size);
void CopyObj (char**  To, char**  From, int Size);
void CopyObj (double* To, double* From, int Size);
void CopyObj (float*  To, float*  From, int Size);

template <typename CopyType>
void StandartCopyObj (CopyType* To, CopyType* From, int Size);
template <typename CopyType>
void CopyObj         (CopyType* To, CopyType* From, int Size);

//==============================================================================

template <typename VectorType>
class newVector
{
    private:
    int Size_;
    VectorType* Data_;

    public:
     newVector ();
     newVector (const int Size);
     newVector (const char Str []);
     newVector (const int Size, const VectorType Arr []);

     newVector (const newVector& Vector);
     newVector& operator = (const newVector& Vector);

    ~newVector ();

    bool OK ();

    void Dump (const char* Title = NULL);

    int GetSize ();

    VectorType& operator [] (const int i);

    void SetSize (const int Size);
};

//------------------------------------------------------------------------------

template <typename VectorType>
newVector<VectorType>:: newVector () :
    Size_ (0),
    Data_ (NULL)
{}

template <typename VectorType>
newVector<VectorType>:: newVector (const int Size) :
    Size_ (0),
    Data_ (NULL)
{
    SetSize (Size);
}

template <typename VectorType>
newVector<VectorType>:: newVector (const char Str []) :
    Size_ (0),
    Data_ (NULL)
{
    int Size = strlen (Str) + 1;

    SetSize (Size + 1);

    for (int i = 0; i < Size; i ++)
    {
        Data_ [i] = Str [i];
    }

    Data_ [Size] = 0;
}

template <typename VectorType>
newVector<VectorType>:: newVector (const int Size, const VectorType Arr []) :
    Size_ (0),
    Data_ (NULL)
{
    SetSize (Size);

    CopyObj (Data_, Arr, Size_);
}

//------------------------------------------------------------------------------

template <typename VectorType>
newVector<VectorType>:: newVector (const newVector& Vector) :
    Size_ (0),
    Data_ (NULL)
{
    SetSize (Vector.Size_);

    CopyObj (Data_, Vector.Data_, Size_);
}

template <typename VectorType>
newVector<VectorType>& newVector<VectorType>::operator = (const newVector& Vector)
{
    SetSize (Vector.Size_);

    CopyObj (Data_, Vector.Data_, Size_);

    return *this;
}


//------------------------------------------------------------------------------

template <typename VectorType>
newVector<VectorType>::~newVector ()
{
    if (Data_) delete[] Data_;
    Data_ = NULL;
}

//------------------------------------------------------------------------------

template <typename VectorType>
bool newVector<VectorType>::OK ()
{
    if (Size_ < 0)           return false;
    if (Size_ > 0 && !Data_) return false;

    return true;
}

template <typename VectorType>
void newVector<VectorType>::Dump (const char* Title)
{
    printf ("=====Dump=====\n");
    if (Title != NULL) printf ("%s\n", Title);
    printf ("newVector//%s\n", (OK ())? "Good" : "BAD");
    printf ("{\n");
    printf ("    Size_ = %d;\n", Size_);
    printf ("    Data_ = //Adress = %d\n", (int)Data_);
    printf ("    {\n");
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

template <typename VectorType>
int newVector<VectorType>::GetSize ()
{
    return Size_;
}

//------------------------------------------------------------------------------

template <typename VectorType>
VectorType& newVector<VectorType>::operator [] (const int i)
{
    assert (OK ());

    if (i < 0)
    {
        Dump ("Because of error!");
        throw TH_ERROR "Mad border braker(%d)!", i);
    }
    assert (i > -1);

    //if (i >= Size_) SetSize (i + 1);
    if (i >= Size_) SetSize ((i < 10)? 10 : (i * 1.5));

    return *(Data_ + i);
}

//------------------------------------------------------------------------------

template <typename VectorType>
void newVector<VectorType>::SetSize (const int Size)
{
    assert (OK ());

    if (Size < 0)
    {
        Dump ("Because of error!");
        throw TH_ERROR "I don't know, how create negative size array!");
    }
    assert (Size >= 0);

    if (Size == 0)
    {
        Size_ = 0;

        if (Data_) delete[] Data_;
        Data_ = NULL;

        assert (OK ());
        return ;
    }

    int NewSize = (int)(Size * 1.25);

    VectorType* NewData = new VectorType [NewSize];

    if (Data_)
    {
        if (Size_ < NewSize) CopyObj (NewData, Data_, Size_  );
        else                 CopyObj (NewData, Data_, NewSize);
    }

    if (Data_) delete[] Data_;
    Data_ = NULL;

    Size_ = NewSize;
    Data_ = NewData;

    assert (OK ());
}

//==============================================================================

template <typename CellType>
class newStack
{
    private:
    int                  CellNum_;
    newVector <CellType> Cell_;

    public:
    newStack ();

    bool OK ();

    void Dump (const char* Title = NULL);

    void     Push (CellType Value);
    CellType Pop  ();

    int GetCellNum ();

    void Down (int Level);
    void Empty ();
    void Zero (int from = 0, int to = -1);
    void Fill (int n, int from = 0, int to = -1);
    void Dup ();
    void Swap ();

    void Update (CellType Value, CellType To);

    void operator << (CellType Value);
    void operator >> (CellType* To);
};

//------------------------------------------------------------------------------

template <typename CellType>
newStack<CellType>::newStack () :
    CellNum_  (0)
{
    assert (OK ());
}

//------------------------------------------------------------------------------

template <typename CellType>
bool newStack<CellType>::OK ()
{
    if (CellNum_ < 0)                 return false;
    if (CellNum_ > Cell_.GetSize ())  return false;

    return Cell_.OK ();
}

template <typename CellType>
void newStack<CellType>::Dump (const char* Title)
{
    printf ("=====Dump=====\n");
    if (Title != NULL) printf ("%s\n", Title);
    printf ("newStack //%s\n", (OK ())? "Good" : "BAD");
    printf ("{\n");
    printf ("    Cell_     = \n");
    printf ("    {\n");
    for (int i = 0; i < Cell_.GetSize (); i ++)
    {
        printf ("        %c [%d] ", ((i < CellNum_)? 'V' : 'X'), i);
        DumpOut (&Cell_ [i]);
        printf ("\n");
    }
    printf ("    };\n");
    printf ("    \n");
    printf ("    CellNum_ = %d;\n", CellNum_);
    printf ("}\n");
}

//------------------------------------------------------------------------------

template <typename CellType>
void newStack<CellType>::Push (CellType Value)
{
    assert (OK ());

    Cell_ [CellNum_] = Value;

    CellNum_ ++;

    assert (OK ());
}
template <typename CellType>
CellType newStack<CellType>::Pop  ()
{
    assert (OK ());

    if (CellNum_ <= 0)
    {
        Dump ("Because of error!");
        throw TH_ERROR "Nothing to Pop!");
    }
    assert (CellNum_ > 0);

    CellNum_ --;

    return Cell_ [CellNum_];
}

//------------------------------------------------------------------------------

template <typename CellType>
int newStack<CellType>::GetCellNum ()
{
    return CellNum_;
}

template <typename CellType>
void newStack<CellType>::Down (int Level)
{
    assert (OK ());

    CellType Temp = Cell_ [CellNum_ - 1];

    for (int i = CellNum_ - 1 - 1; i > CellNum_ - Level - 1 - 1; i --)
    {
        Cell_ [i + 1] = Cell_ [i];
    }

    Cell_ [CellNum_ - Level - 1] = Temp;

    assert (OK ());
}

template <typename CellType>
void newStack<CellType>::Empty ()
{
    assert (OK ());

    CellNum_ = 0;

    assert (OK ());
}

template <typename CellType>
void newStack<CellType>::Zero (int from, int to)
{
    Fill (0, from, to);
}

template <typename CellType>
void newStack<CellType>::Fill (int n, int from, int to)
{
    assert (OK ());

    if (to == -1) to = CellNum_;
    assert (from > -1 && to > -1);
    assert (to < CellNum_ + 1);
    assert (from < to);

    for (int i = from; i < to; i ++)
    {
        Cell_ [i] = n;
    }

    assert (OK ());
}

template <typename CellType>
void newStack<CellType>::Dup ()
{
    assert (OK ());

    Cell_ [CellNum_] = Cell_ [CellNum_ - 1];

    CellNum_ ++;

    assert (OK ());
}

template <typename CellType>
void newStack<CellType>::Swap ()
{
    assert (OK ());

    CellType Temp = Cell_ [CellNum_ - 1];

    Cell_ [CellNum_ - 1] = Cell_ [CellNum_ - 2];
    Cell_ [CellNum_ - 2] = Temp;

    assert (OK ());
}

template <typename CellType>
void newStack<CellType>::Update (CellType Value, CellType To)
{
    assert (OK ());

    for (int i = 0; i < CellNum_; i ++)
    {
        if (Cell_ [i] == Value) Cell_ [i] = To;
    }

    assert (OK ());
}

//------------------------------------------------------------------------------

template <typename CellType>
void newStack<CellType>::operator << (CellType Value)
{
    Push (Value);
}

template <typename CellType>
void newStack<CellType>::operator >> (CellType* To)
{
    *To = Pop ();
}

//==============================================================================

void CopyObj (int*    To, int*    From, int Size)
{
    StandartCopyObj (To, From, Size);
}
void CopyObj (char*   To, char*   From, int Size)
{
    StandartCopyObj (To, From, Size);
}
void CopyObj (char**  To, char**  From, int Size)
{
    StandartCopyObj (To, From, Size);
}
void CopyObj (double* To, double* From, int Size)
{
    StandartCopyObj (To, From, Size);
}
void CopyObj (float*  To, float*  From, int Size)
{
    StandartCopyObj (To, From, Size);
}

template <typename CopyType>
void StandartCopyObj (CopyType* To, CopyType* From, int Size)
{
    memcpy (To, From, Size * sizeof (CopyType));
}
template <typename CopyType>
void CopyObj (CopyType* To, CopyType* From, int Size)
{
    for (int i = 0; i < Size; i ++)
    {
        To [i] = From [i];
    }
}

//==============================================================================
void DumpOut (int*    ToPrintf)
{
    printf ("%d", *ToPrintf);
}
void DumpOut (char*   ToPrintf)
{
    printf ("%c", *ToPrintf);
}
void DumpOut (char**  ToPrintf)
{
    printf ("%s", *ToPrintf);
}
void DumpOut (double* ToPrintf)
{
    printf ("%lg", *ToPrintf);
}
void DumpOut (float*  ToPrintf)
{
    printf ("%lg", *ToPrintf);
}
void DumpOut (bool*      ToPrintf)
{
    printf ("%s", (*ToPrintf)? "true" : "false");
}


template <typename Type>
void DumpOut (Type*   ToPrintf)
{
    printf ("*%d", (int)ToPrintf);
}

#endif
