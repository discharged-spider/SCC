case (30/*equal*/):
{
    FCLEX

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FCOMI_ST(1)

    FCOMPP

    double TEMP = 0;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    JNE_8_CONST
    PUSH_BYTE (13)

    ADD_8_ESP
    PUSH_BYTE (0x8)

    TEMP = 1;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    break;
}
case (31/*more*/):
{
    FCLEX

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FCOMI_ST(1)

    FCOMPP

    double TEMP = 1;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    JB_8_CONST
    PUSH_BYTE (13)

    ADD_8_ESP
    PUSH_BYTE (0x8)

    TEMP = 0;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    break;
}
case (32/*less*/):
{
    FCLEX

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FCOMI_ST(1)

    FCOMPP

    double TEMP = 1;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    JA_8_CONST
    PUSH_BYTE (13)

    ADD_8_ESP
    PUSH_BYTE (0x8)

    TEMP = 0;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    break;
}
case (33/*not*/):
{
    FCLEX

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FLD1

    FCOMI_ST(1)

    FCOMPP

    double TEMP = 0;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    JZ_8_CONST
    PUSH_BYTE (13)

    ADD_8_ESP
    PUSH_BYTE (0x8)

    TEMP = 1;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[4])
    PUSH_BYTE (TEMP_TO_CHAR[5])
    PUSH_BYTE (TEMP_TO_CHAR[6])
    PUSH_BYTE (TEMP_TO_CHAR[7])

    PUSH_32_CONST
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    break;
}
case (34/*ifnot*/):
{
    FCLEX

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FLD1

    FCOMI_ST(1)

    FCOMPP

    i ++;

    JZ_8_CONST
    PUSH_BYTE (5)

    JMP_32_CONST

    if ((unsigned int)Program [i] >= Marks.size () || Marks [(unsigned int)Program [i]] == -1)
    {
        ToReplace.push ((*OutputSize - 1) + 1);

        unsigned int TEMP = (unsigned int)Program [i];
        unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
        PUSH_BYTE (TEMP_TO_CHAR[0])
        PUSH_BYTE (TEMP_TO_CHAR[1])
        PUSH_BYTE (TEMP_TO_CHAR[2])
        PUSH_BYTE (TEMP_TO_CHAR[3])
    }
    else
    {
        int TEMP = Marks [(unsigned int)Program [i]] - ((*OutputSize - 1) + 5);
        unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
        PUSH_BYTE (TEMP_TO_CHAR[0])
        PUSH_BYTE (TEMP_TO_CHAR[1])
        PUSH_BYTE (TEMP_TO_CHAR[2])
        PUSH_BYTE (TEMP_TO_CHAR[3])
    }

    break;
}
