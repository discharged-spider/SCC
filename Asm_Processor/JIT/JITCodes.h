#ifndef JIT_CODES_H_
#define JIT_CODES_H_

#define EAX 0
#define EBX 3
#define ECX 1
#define EDX 2

#define ESP 4

#define PUSH_32_CONST    PUSH_BYTE (0x68)
#define PUSH_32_REG(REG) PUSH_BYTE (0x50 + REG)

#define POP_32_REG(REG) PUSH_BYTE (0x58 + REG)

#define ADD_8_ESP PUSH_BYTE (0x83) PUSH_BYTE (0xC4)
#define DIV_32_MEMORY_ESP PUSH_BYTE (0xF7) PUSH_BYTE (0x34) PUSH_BYTE (0x24)

#define MOV_32_REG(REG) PUSH_BYTE (0xB8 + REG)

#define JMP_8_CONST  PUSH_BYTE (0xEB)
#define JMP_32_CONST PUSH_BYTE (0xE9)

#define JNE_8_CONST PUSH_BYTE (0x75)
#define JZ_8_CONST PUSH_BYTE (0x74)
#define JA_8_CONST PUSH_BYTE (0x77)
#define JB_8_CONST PUSH_BYTE (0x72)

#define CALL_32_CONST PUSH_BYTE (0xE8)
#define CALL_32_ADDR_EAX PUSH_BYTE (0xFF) PUSH_BYTE (0xD0)

#define FINIT\
    PUSH_BYTE (0x9B)\
    PUSH_BYTE (0xDB)\
    PUSH_BYTE (0xE3)

#define FCLEX\
    PUSH_BYTE (0x9B)\
    PUSH_BYTE (0xDB)\
    PUSH_BYTE (0xE2)

#define FCOMI_ST(ST)\
    PUSH_BYTE (0xDB)\
    PUSH_BYTE (0xF0 + ST)

#define FCOMPP\
    PUSH_BYTE (0xDE)\
    PUSH_BYTE (0xD9)

#define FLD1\
    PUSH_BYTE (0xD9)\
    PUSH_BYTE (0xE8)

#define FLD_64_MEMORY_ESP\
    PUSH_BYTE (0xDD)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FILD_32_MEMORY_ESP\
    PUSH_BYTE (0xDB)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FADD_64_MEMORY_ESP\
    PUSH_BYTE (0xDC)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FSUB_64_MEMORY_ESP\
    PUSH_BYTE (0xDC)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/1 * 32 + 0 * 16 + 0 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FMUL_64_MEMORY_ESP\
    PUSH_BYTE (0xDC)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/0 * 32 + 0 * 16 + 1 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FDIV_64_MEMORY_ESP\
    PUSH_BYTE (0xDC)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/1 * 32 + 1 * 16 + 0 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FSTP_64_MEMORY_ESP\
    PUSH_BYTE (0xDD)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/0 * 32 + 1 * 16 + 1 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define FISTP_32_MEMORY_ESP\
    PUSH_BYTE (0xDB)\
    PUSH_BYTE (/*mod{*/0 * 128 + 0 * 64/*}*/ + /*OPB{*/0 * 32 + 1 * 16 + 1 * 8/*}*/ + /*r/m{*/1 * 4 + 0 * 2 + 0 * 1/*}*/)\
    PUSH_BYTE (0x24)//PUSH_BYTE (/*SS{*/0 * 128 + 0 * 64/*}*/ + /*INDEX{*/0 * 32 + 0 * 16 + 0 * 8/*}*/ + /*BASE{*/REG/*}*/)

#define RET_NEAR PUSH_BYTE (0xC3)
#define RET_FAR PUSH_BYTE (0xCB)

#endif
