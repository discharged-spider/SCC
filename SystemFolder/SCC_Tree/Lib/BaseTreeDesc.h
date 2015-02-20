#ifndef BASETREEDESC__
#define BASETREEDESC__

//Nodes values for syntax tree
enum ExtraBaseTreeDesc
{
    N_NUM = 1,
    N_VAR = 2,
    N_ARR = 3,
    N_FUN = 4,
    N_LNK = 5,

    N_NEW = 10,
    N_DEL = 11,
    N_NATIVE = 12,

    N_EQ      = 20,
    N_INCR    = 21, // ++
    N_DECR    = 22, // --
    N_SUM_EQ  = 23, // +=
    N_SUB_EQ  = 24, // -=
    N_MUL_EQ  = 25, // *=
    N_DIV_EQ  = 26, // /=
    N_MOD_EQ  = 27, // %=

    N_SUM = 30,
    N_SUB = 31,
    N_MUL = 32,
    N_DIV = 33,
    N_MOD = 34,

    N_TRUE  = 40,
    N_FALSE = 41,

    N_AND   = 50,
    N_OR    = 51,

    N_NOT   = 52,// !
    N_OPEQ  = 53,// ==
    N_NONEQ = 54,// !=
    N_MORE  = 55,// >
    N_LESS  = 56,// <
    N_MREQ  = 57,// >=
    N_LSEQ  = 58,// <=

    N_IF   = 60,
    N_ELSE = 61,

    N_WHILE     = 70,
    N_FOR       = 71,
    N_BREAK     = 72,
    N_CONTINUE  = 73,
    N_RETURN    = 74,

    N_ECHO = 80,
    N_GET  = 81,

    N_MARK = 90,
    N_GOTO = 91
};

#endif
