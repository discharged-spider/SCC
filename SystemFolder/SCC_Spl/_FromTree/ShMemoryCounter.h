#ifndef SHMEMORYCOUNTER__
#define SHMEMORYCOUNTER__

#include "..\..\_SystemLibs\Stack\Stack.h"

class newShMemoryCounter
{
    private:
    int Size_;
    newStack  < int > D_;
    newVector < int > Data_;
    
    public:
    newShMemoryCounter ();       
    
    void Zero ();
         
    void Rem (int Addr);
    void Add (int Addr);
    
    void PushMemory ();
    void PopMemory  ();
    
    void AddFunc (int Function);
    
    bool UseInPop (int Addr, int Function);
    
    void PrintfPush (FILE* To, int Function, newTreeToShekspearData& ShData);
    void PrintfPop  (FILE* To, int Function, newTreeToShekspearData& ShData);                     
};

#endif
