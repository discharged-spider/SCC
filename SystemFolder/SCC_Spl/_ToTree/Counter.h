#ifndef COUNTER_H_
#define COUNTER_H_

#include "..\..\_SystemLibs\Stack\Stack.h"

class newCounter
{
    public:    
    int Size_;        
    newVector < newVector <char> > Data_;
               
    newCounter ();       
    
    void Empty ();
    
    bool Find (char Value []);
    
    void Rem (char Value []);
    void Add (char Value []);
};


newCounter::newCounter () :
    Size_ (0)
{}                   

void newCounter::Empty ()
{
    Size_ = 0;     
}

bool  newCounter::Find (char Value [])
{
    for (int i = 0; i < Size_; i ++)
    {
        if (strcmp (Value, &(Data_[i])[0]) == 0) return true;    
    }
    
    return false;     
}

void newCounter::Rem (char Value [])
{
    for (int i = 0; i < Size_; i ++)
    {
       if (strcmp (Value, &(Data_[i])[0]) == 0)
        {
            Size_ --;
    
            Data_[i] = Data_ [Size_];           
            return;
        } 
    }      
}

void newCounter::Add (char Value [])
{ 
    for (int c = 0; Value [c] != 0; c ++)
    {
        (Data_ [Size_])[c] = Value [c];
        (Data_ [Size_])[c + 1] = 0;
    }
    
    Size_ ++;     
}
#endif
