#ifndef ASSEM__
#define ASSEM__

#include "AssemBase.h"

#include "AssemDesc.h"

using std::string;

//==============================================================================

bool TryToKey (string& Word, newNodeData* Data);

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size);

//==============================================================================

bool TryToKey (string& Word, newNodeData* Data)
{
    if (Word == "," || Word == "\n")
    {
        (*Data).Descriptor = N_END;

        return true;
    }

    #define DESC(NAME, STR) \
    if (Word == STR) \
    { \
        (*Data).Descriptor = NAME; \
        \
        return true;\
    }

    #define BASE_DESC(NAME, STR) DESC (NAME, STR)

    #include "AssemDescDefine.h"

    return false;
}

//==============================================================================

void LexemProgram (FILE* file, newVector <newNodeData>& Tokens, int* Size)
{
    LexemProgram (file, Tokens, Size, &TryToKey);
}

#endif
