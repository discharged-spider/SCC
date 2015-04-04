#ifndef ALLDESC
#define ALLDESC

#include "..\..\SCC_TREE\Lib\BaseTreeDesc.h"
#include "Assem\AssemDesc.h"

//==============================================================================

void GetDesc (FILE* file, newNodeData Data);

//==============================================================================

void GetDesc (FILE* file, newNodeData Data)
{
    if (Data.Descriptor == N_NUM)
    {
        fprintf (file, "%g", Data.Data);
    }
    if (Data.Descriptor == N_VAR)
    {
        fprintf (file, "%s", Data.GetName ());
    }
    if (Data.Descriptor == N_ARR)
    {
        fprintf (file, "%s", Data.GetName ());
    }
    if (Data.Descriptor == N_FUN)
    {
        fprintf (file, "%s", Data.GetName ());
    }

    if (Data.Descriptor == N_END)
    {
        fprintf (file, "\n");
    }

    #define DESC(NAME, STR) \
    if (Data.Descriptor == NAME) \
    { \
        fprintf (file, STR); \
    }

    #define BASE_DESC(NAME, STR) DESC (NAME, STR)

    #include "Assem/AssemDescDefine.h"

    if (Data.Descriptor == N_NAME)
    {
        fprintf (file, "%s(N_NAME)", Data.GetName ());
    }
}

#endif
