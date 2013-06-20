#ifndef COUNTER_H_
#define COUNTER_H_

#include "..\..\_SystemLibs\Stack\Stack.h"

class newIntCounter
{
    public:    
    int Size_;        
    newVector < int > Data_;
               
    newIntCounter ();       
    
    void Empty ();
    
    bool Find (int Value);
    
    void Rem (int Value);
    void Add (int Value);
};


newIntCounter::newIntCounter () :
    Size_ (0)
{}                   

void newIntCounter::Empty ()
{
    Size_ = 0;     
}

bool  newIntCounter::Find (int Value)
{
    for (int i = 0; i < Size_; i ++)
    {
        if (Data_[i] == Value) return true;    
    }
    
    return false;     
}

void newIntCounter::Rem (int Value)
{
    for (int i = 0; i < Size_; i ++)
    {
       if (Value == Data_[i])
        {
            Size_ --;
    
            Data_[i] = Data_ [Size_];           
            return;
        } 
    }      
}

void newIntCounter::Add (int Value)
{ 
    Data_ [Size_] = Value;
    
    Size_ ++;     
}
#endif
