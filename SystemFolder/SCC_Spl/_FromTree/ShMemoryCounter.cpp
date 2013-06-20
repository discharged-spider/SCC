#ifndef ASMMEMORYCOUNTER_CPP_
#define ASMMEMORYCOUNTER_CPP_

#include "ShMemoryCounter.h"

newShMemoryCounter::newShMemoryCounter () :
    Size_ (0)
{}  

void newShMemoryCounter::Zero ()
{
    Size_ = 0;    
}

void newShMemoryCounter::Rem (int Addr)
{
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

void newShMemoryCounter::Add (int Addr)
{
    Data_ [Size_] = Addr;
    
    D_.Push (Size_); 
    
    Size_ ++; 
} 

void newShMemoryCounter::PushMemory ()
{
    D_.Push (-1);     
}
void newShMemoryCounter::PopMemory ()
{
    int i = 0; 
    while (true)
    {
        i = D_.Pop ();  
        
        if (i == -1) break;
        if (i < 0) continue;
          
        Rem (Data_ [i]);
    }     
}

void newShMemoryCounter::AddFunc (int Function)
{
    D_.Push (Function - 3);     
}

bool newShMemoryCounter::UseInPop (int Addr, int Function)
{
    newStack<int> Dl = D_;
    
    int i = 0;
    while (Dl.GetCellNum () > 0)
    {
        i = Dl.Pop ();
       
        if (i == Function - 3) break;
        if (i < 0) continue;
        
        if (Data_ [i] == Addr) return true;
    }
    
    return false; 
}

void newShMemoryCounter::PrintfPush (FILE* To, int Function, newTreeToShekspearData& ShData)
{
    newStack<int> Dl = D_;
    
    int i = 0;
    while (Dl.GetCellNum () > 0)
    {
        i = Dl.Pop ();
       
        if (i == Function - 3) break;
        if (i < 0) continue;
       
        SaveA (To, ShData, Data_ [i]);          
    }
}

void newShMemoryCounter::PrintfPop  (FILE* To, int Function, newTreeToShekspearData& ShData)
{
    newStack<int> Dl = D_;
    
    int i = 0;
    while (Dl.GetCellNum () > 0)
    {
        i = Dl.Pop ();
       
        if (i == Function - 3) break;
        if (i < 0) continue;
       
        LoadA (To, ShData, Data_ [i]);          
    }
}

#endif
