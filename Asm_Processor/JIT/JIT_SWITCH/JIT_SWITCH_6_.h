case (Syntax::LOG_GET_VAR):
{
    i ++;

    if (!UseInspector) break;

    MOV_32_REG (EBX)
    unsigned int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    TEMP = LINK_LOG_GET_VAR;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
case (Syntax::LOG_GET_ARR):
{
    i ++;

    if (!UseInspector) break;

    MOV_32_REG (EBX)
    unsigned int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    TEMP = LINK_LOG_GET_ARR;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
case (Syntax::LOG_SET_VAR):
{
    i ++;

    if (!UseInspector) break;

    MOV_32_REG (EBX)
    unsigned int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    TEMP = LINK_LOG_SET_VAR;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
case (Syntax::LOG_SET_ARR):
{
    i ++;

    if (!UseInspector) break;

    MOV_32_REG (EBX)
    unsigned int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    MOV_32_REG (EAX)
    ToLink.push ((*OutputSize - 1) + 1);

    TEMP = LINK_LOG_GET_ARR;
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_ADDR_EAX

    break;
}
