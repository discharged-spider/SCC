case (20/*:*/):
{
    i ++;

    if ((unsigned int)Program [i] >= Marks.size ())
    {
        while (Marks.size () <= (unsigned int)Program [i]) Marks.push_back(-1);
    }

    Marks [(unsigned int)Program [i]] = (*OutputSize - 1) + 1;

    break;
}
case (21/*goto*/):
{
    i ++;

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
case (22/*call*/):
{
    i ++;

    if ((unsigned int)Program [i] >= Marks.size () || Marks [(unsigned int)Program [i]] == -1)
    {
        //To save 64-bit stack cell
        PUSH_32_CONST
        PUSH_BYTE (TRASH)
        PUSH_BYTE (TRASH)
        PUSH_BYTE (TRASH)
        PUSH_BYTE (TRASH)

        CALL_32_CONST

        ToReplace.push ((*OutputSize - 1) + 1);

        unsigned int TEMP = (unsigned int)Program [i];
        unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
        PUSH_BYTE (TEMP_TO_CHAR[0])
        PUSH_BYTE (TEMP_TO_CHAR[1])
        PUSH_BYTE (TEMP_TO_CHAR[2])
        PUSH_BYTE (TEMP_TO_CHAR[3])

        //To save 64-bit stack cell
        ADD_8_ESP
        PUSH_BYTE (0x4)

        break;
    }

    //To save 64-bit stack cell
    PUSH_32_CONST
    PUSH_BYTE (TRASH)
    PUSH_BYTE (TRASH)
    PUSH_BYTE (TRASH)
    PUSH_BYTE (TRASH)

    CALL_32_CONST
    int TEMP = Marks [(unsigned int)Program [i]] - ((*OutputSize - 1) + 5);
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    //To save 64-bit stack cell
    ADD_8_ESP
    PUSH_BYTE (0x4)

    break;
}
case (23/*end*/):
{
    RET_NEAR

    break;
}
