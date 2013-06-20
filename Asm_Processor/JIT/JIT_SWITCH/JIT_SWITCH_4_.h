case (40/*pushm*/):
{
    i ++;

    MOV_32_REG (EBX)
    unsigned int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    TEMP = LINK_PUSHMS;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    PUSH_32_REG (EDX)
    PUSH_32_REG (ECX)

    break;
}
case (41/*popm*/):
{
    i ++;

    MOV_32_REG (EBX)
    unsigned int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    POP_32_REG (ECX)
    POP_32_REG (EDX)

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    TEMP = LINK_POPMS;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
case (42/*pushms*/):
{
    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (EBX)

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    unsigned int TEMP = LINK_PUSHMS;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    PUSH_32_REG (EDX)
    PUSH_32_REG (ECX)

    break;
}
case (43/*popms*/):
{
    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (EBX)

    POP_32_REG (ECX)
    POP_32_REG (EDX)

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    unsigned int TEMP = LINK_POPMS;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
case (44/*add*/):
{
    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (EBX)

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    unsigned int TEMP = LINK_ADD;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    PUSH_32_REG (EAX) //To keep memory for FSTP_64_MEMORY_ESP
    PUSH_32_REG (EAX)

    FILD_32_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
case (45/*rem*/):
{
    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (EBX)

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (ECX)

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    unsigned int TEMP = LINK_REM;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
