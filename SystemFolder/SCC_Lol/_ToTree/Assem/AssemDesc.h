#ifndef ASSEMDESC__
#define ASSEMDESC__

enum AssemDesc
{
    N_END = 1001,
    N_NAME,

    #define DESC(NAME, STR) NAME,

    #include "AssemDescDefine.h"
};

#endif
