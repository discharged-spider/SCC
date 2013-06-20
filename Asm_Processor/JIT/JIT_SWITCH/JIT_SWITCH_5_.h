case (50/*@*/):
{
    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (EBX)

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    unsigned int TEMP = LINK_ECHO;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
case (51/*&*/):
{
    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    unsigned int TEMP = LINK_GET;
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    PUSH_32_REG (EBX) //To keep memory for FSTP_64_MEMORY_ESP
    PUSH_32_REG (EBX)

    FILD_32_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
