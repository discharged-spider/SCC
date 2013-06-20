#ifndef ASMMEMORYCOUNTER__
#define ASMMEMORYCOUNTER__

#include "..\..\_SystemLibs\Stack\Stack.h"

class newAsmMemoryCounter
{
    private:
    int Size_;
    newStack  < int > D_;
    newVector < int > Data_;

    public:
    newAsmMemoryCounter ();

    void Zero ();

    void Rem (int Addr, FILE* To, int ArrOffset);
    void Add (int Addr);

    void PrintfRem (int Addr, FILE* To, int ArrOffset);

    void PushMemory ();
    void PopMemory  (FILE* To, int ArrOffset);

    void PrintfPopFunctionMemory (FILE* To, int ArrOffset, int Function);

    void AddFunc (int Function);

    void PrintfPush (FILE* To, int Function);
    void PrintfPop  (FILE* To, int Function);
};

newAsmMemoryCounter::newAsmMemoryCounter () :
    Size_ (0)
{}

void newAsmMemoryCounter::Zero ()
{
    Size_ = 0;
}

void newAsmMemoryCounter::Rem (int Addr, FILE* To, int ArrOffset)
{
    if (Addr < ArrOffset)
    {
        fprintf (To, "//Delete var\n");
        fprintf (To, "//{\n");
        fprintf (To, "push 1 //Size\n");
        fprintf (To, "pushm %d //Address\n", Addr);
        fprintf (To, "rem\n");
        fprintf (To, "//}\n");
    }
    else
    {
        fprintf (To, "//Delete arr\n");
        fprintf (To, "//{\n");
        fprintf (To, "pushm %d //Size\n", Addr + 1);
        fprintf (To, "pushm %d //Address\n", Addr);
        fprintf (To, "rem\n");
        fprintf (To, "//}\n");
    }

    for (int i = 0; i < Size_; i ++)
    {
        if (Addr == Data_ [i])
        {
            Size_ --;

            Data_ [i] = Data_ [Size_];

            D_.Update (Addr, -2);

            return ;
        }
    }
}

void newAsmMemoryCounter::Add (int Addr)
{
    Data_ [Size_] = Addr;

    D_.Push (Size_);

    Size_ ++;
}

void newAsmMemoryCounter::PrintfRem (int Addr, FILE* To, int ArrOffset)
{
    if (Addr < ArrOffset)
    {
        fprintf (To, "//Delete var\n");
        fprintf (To, "//{\n");
        fprintf (To, "push 1 //Size\n");
        fprintf (To, "pushm %d //Address\n", Addr);
        fprintf (To, "rem\n");
        fprintf (To, "//}\n");
    }
    else
    {
        fprintf (To, "//Delete arr\n");
        fprintf (To, "//{\n");
        fprintf (To, "pushm %d //Size\n", Addr + 1);
        fprintf (To, "pushm %d //Address\n", Addr);
        fprintf (To, "rem\n");
        fprintf (To, "//}\n");
    }
}

void newAsmMemoryCounter::PushMemory ()
{
    D_.Push (-1);
}
void newAsmMemoryCounter::PopMemory (FILE* To, int ArrOffset)
{
    int i = 0;
    while (true)
    {
        i = D_.Pop ();

        if (i == -1) break;
        if (i < 0) continue;

        Rem (Data_ [i], To, ArrOffset);
    }
}

void newAsmMemoryCounter::PrintfPopFunctionMemory (FILE* To, int ArrOffset, int Function)
{
    newStack<int> Dl = D_;

    int i = 0;
    while (true)
    {
        i = Dl.Pop ();

        if (i == -(Function + 3)) break;
        if (i < 0) continue;

        PrintfRem (Data_ [i], To, ArrOffset);
    }
}

void newAsmMemoryCounter::AddFunc (int Function)
{
    D_.Push (-(Function + 3));
}

void newAsmMemoryCounter::PrintfPush (FILE* To, int Function)
{
    fprintf (To, "//Push memory\n");
    fprintf (To, "//{\n");

    newStack<int> Dl = D_;

    int i = 0;
    while (Dl.GetCellNum () > 0)
    {
        i = Dl.Pop ();

        if (i == -(Function + 3)) break;
        if (i < 0) continue;

        fprintf (To, "pushm %d\n", Data_ [i]);
    }

    fprintf (To, "//}\n");
}

void newAsmMemoryCounter::PrintfPop  (FILE* To, int Function)
{
    fprintf (To, "//Pop memory\n");
    fprintf (To, "//{\n");

    newStack<int> Dl = D_;
    newStack<int> Dll;

    int i = 0;
    while (Dl.GetCellNum () > 0)
    {
       i = Dl.Pop ();

       if (i == -(Function + 3)) break;
       if (i < 0) continue;
       Dll.Push (i);
    }

    while (Dll.GetCellNum () > 0)
    {
        i = Dll.Pop ();
        fprintf (To, "popm %d\n", Data_ [i]);
    }

    fprintf (To, "//}\n");
}

#endif
