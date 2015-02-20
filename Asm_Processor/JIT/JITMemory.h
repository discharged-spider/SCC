#ifndef JIT_MEMORY_H_
#define JIT_MEMORY_H_

#include "cstdio"

#include "iostream"

#include "vector"
#include "stack"

#include "windows.h"

using namespace std;

namespace JITMemory
{
    vector <bool>   FreeMemory_;
    vector <double> Memory_;

    void PushMS ();
    void PopMS ();

    void Add ();
    void Rem ();

    void Clear ();

    void Dump ();
}

//-------------------------------------------------------------------------------------------------

void JITMemory::PushMS ()
{
    unsigned int i = 0;
    __asm __volatile__
    (
        "mov %%ebx, %0"
        : "=m"(i)
        :
        : "ebx"
    );

    double Value = JITMemory::Memory_.at (i);
    unsigned int* IValue = (unsigned int*)&Value;
    __asm __volatile__
    (
        "mov %0, %%ecx \n"
        "mov %1, %%edx \n"
        :
        : "m"(IValue[0]), "m"(IValue[1])
    );
}

void JITMemory::PopMS ()
{
    unsigned int i = 0;

    double Value = 0;
    unsigned int* IValue = (unsigned int*)&Value;
    __asm __volatile__
    (
        "mov %%ebx, %0\n"
        "mov %%ecx, %1\n"
        "mov %%edx, %2\n"
        : "=m"(i), "=m"(IValue[0]), "=m"(IValue[1])
        :
        : "ebx", "ecx", "edx"
    );

    JITMemory::Memory_.at (i) = Value;
}

void JITMemory::Add ()
{
    unsigned int Size = 0;
    __asm __volatile__
    (
        "mov %%ebx, %0"
        : "=m"(Size)
        :
        : "ebx"
    );

    for (unsigned int i = 0, c = 0; i < JITMemory::Memory_.size (); i ++)
    {
        if (JITMemory::FreeMemory_ [i])
        {
            c ++;
        }
        else c = 0;

        if (c == Size)
        {
            for (unsigned int j = i - c + 1; j < i + 1; j ++)
            {
                JITMemory::FreeMemory_ [j] = false;
            }

            int TEMP = i - Size + 1;
            __asm __volatile__
            (
                "mov %0, %%eax"
                :
                : "m"(TEMP)
            );

            return ;
        }
    }

    int TEMP = JITMemory::Memory_.size ();

    JITMemory::Memory_.resize (JITMemory::Memory_.size () + Size);
    JITMemory::FreeMemory_.resize (JITMemory::Memory_.size (), false);

    __asm __volatile__
    (
        "mov %0, %%eax"
        :
        : "m"(TEMP)
    );
}

void JITMemory::Rem ()
{
    unsigned int Addr = 0;
    unsigned int Size = 0;

    __asm __volatile__
    (
        "mov %%ebx, %0 \n"
        "mov %%ecx, %1 \n"
        : "=m"(Addr), "=m"(Size)
        :
        : "ebx", "ecx"
    );

    for (unsigned int i = 0; i < Size; i ++)
    {
        JITMemory::FreeMemory_ [Addr + i] = true;
    }
}

void JITMemory::Clear ()
{
    JITMemory::Memory_.clear ();
    JITMemory::FreeMemory_.clear ();
}

void JITMemory::Dump ()
{
    printf ("<<<<==MEMORY_DUMP==>>>>>\n");
    for (unsigned int i = 0; i < JITMemory::Memory_.size (); i ++)
    {
       printf ("%g[%s] ", JITMemory::Memory_ [i], (JITMemory::FreeMemory_ [i])? "V" : "X");
    }
    printf ("\n");
}

#endif
