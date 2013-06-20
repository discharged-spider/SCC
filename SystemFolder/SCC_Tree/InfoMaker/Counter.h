#ifndef COUNTER__
#define COUNTER__

#include "..\..\_SystemLibs\Stack\Stack.h"

class newCounter
{
    private:
    int Size_;
    newStack  < int >              D_;
    newVector < bool >             Mask_;
    newVector < newVector <char> > Data_;

    public:
    newCounter ();

    int Size ();

    int UsedSize ();

    int Find (char Value []);

    void Rem (int i);
    int  Add (char Value []);

    int LevelRem (char Value []);

    void PushMemory ();
    void PopMemory ();
};

newCounter::newCounter () :
    Size_ (0)
{}

int newCounter::Size ()
{
    return Size_;
}

int newCounter::UsedSize ()
{
    int Result = 0;
    for (int i = 0; i < Size_; i ++)
    {
        if (Mask_ [i] == true) Result ++;
    }

    return Result;
}

int  newCounter::Find (char Value [])
{
    for (int i = 0; i < Size_; i ++)
    {
       if (Mask_ [i] == true && strcmp (Value, &(Data_[i])[0]) == 0) return i;
    }

    return -1;
}

void newCounter::Rem (int i)
{
    Mask_ [i] = false;

    D_.Update (i, -2);

    if (i == Size_ - 1) Size_ --;
}

int newCounter::LevelRem (char Value [])
{
    newStack <int> D = D_;

    int Level = 0;
    int i = 0;
    while (D.GetCellNum () > 0)
    {
        i = D.Pop ();

        if (i == -1)
        {
            Level ++;
            continue;
        }
        if (i == -2) continue;

        if (strcmp (&(Data_ [i])[0], Value) == 0)
        {
            return Level;
        }
    }

    return -1;
}

int newCounter::Add (char Value [])
{
    for (int i = 0; i < Size_; i ++)
    {
        if (Mask_ [i] == false)
        {
            Mask_ [i] = true;

            for (int c = 0; Value [c] != 0; c ++)
            {
                (Data_ [i])[c] = Value [c];
                (Data_ [i])[c + 1] = 0;
            }

            D_.Push (i);

            return i;
        }
    }

    Mask_ [Size_] = true;

    for (int c = 0; Value [c] != 0; c ++)
    {
        (Data_ [Size_])[c] = Value [c];
        (Data_ [Size_])[c + 1] = 0;
    }

    Size_ ++;

    D_.Push (Size_ - 1);

    return Size_ - 1;
}

void newCounter::PushMemory ()
{
    D_.Push (-1);
}
void newCounter::PopMemory ()
{
    int i = 0;
    while (true)
    {
        i = D_.Pop ();

        if (i == -1) break;
        if (i == -2) continue;

        Rem (i);
    }
}

#endif
