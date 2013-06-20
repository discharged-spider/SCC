case (0/*push*/):
{
    i ++;

    unsigned char* Double = (unsigned char*)(&Program [i]);

    PUSH_32_CONST
    PUSH_BYTE (Double[4])
    PUSH_BYTE (Double[5])
    PUSH_BYTE (Double[6])
    PUSH_BYTE (Double[7])

    PUSH_32_CONST
    PUSH_BYTE (Double[0])
    PUSH_BYTE (Double[1])
    PUSH_BYTE (Double[2])
    PUSH_BYTE (Double[3])

    break;
}
case (1/*pop*/):
{
    ADD_8_ESP
    PUSH_BYTE (0x8)

    break;
}
case (2/*dup*/):
{
    POP_32_REG (EAX)
    POP_32_REG (EBX)

    PUSH_32_REG(EBX)
    PUSH_32_REG(EAX)

    PUSH_32_REG(EBX)
    PUSH_32_REG(EAX)

    break;
}
case (3/*swap*/):
{
    POP_32_REG (EAX)
    POP_32_REG (EBX)

    POP_32_REG (ECX)
    POP_32_REG (EDX)

    PUSH_32_REG(EBX)
    PUSH_32_REG(EAX)

    PUSH_32_REG(EDX)
    PUSH_32_REG(ECX)

    break;
}
case (4/*down*/):
{
    i ++;

    if (DownPos == -1)
    {
        JMP_8_CONST
        PUSH_BYTE (0x34) //Calculated before

        DownPos = ((*OutputSize - 1) + 1);

        PUSH_BYTE (0x8B) PUSH_BYTE (0xC4) //mov eax, esp
        PUSH_BYTE (0x83) PUSH_BYTE (0xC0) PUSH_BYTE (0x08) //add eax, 04h + add eax, 04h

        PUSH_BYTE (0x8B) PUSH_BYTE (0x48) PUSH_BYTE (0xFC) //mov ecx, [eax - 4]
        PUSH_BYTE (0x8B) PUSH_BYTE (0x10) //mov edx, [eax]

        PUSH_BYTE (0xD9) PUSH_BYTE (0xE8) //fld1

        PUSH_BYTE (0x89) PUSH_BYTE (0x18) //mov [eax], ebx
        PUSH_BYTE (0xDB) PUSH_BYTE (0x00) //fild dword ptr [eax]

        PUSH_BYTE (0xD8) PUSH_BYTE (0xC1) //fadd ST, ST(1)

        //Start :

        PUSH_BYTE (0xDB) PUSH_BYTE (0xF1) //fcomi ST(0), ST(1)

        PUSH_BYTE (0x74) PUSH_BYTE (0x16) //jz End

        PUSH_BYTE (0xD8) PUSH_BYTE (0xE1) //fsub ST, ST (1)

        PUSH_BYTE (0x8B) PUSH_BYTE (0x58) PUSH_BYTE (0x04) //mov ebx, dword ptr [eax + 4]
        PUSH_BYTE (0x89) PUSH_BYTE (0x58) PUSH_BYTE (0xFC) //mov dword ptr [eax - 4], ebx

        PUSH_BYTE (0x83) PUSH_BYTE (0xC0) PUSH_BYTE (0x04) //add eax, 04h

        PUSH_BYTE (0x8B) PUSH_BYTE (0x58) PUSH_BYTE (0x04) //mov ebx, dword ptr [eax + 4]
        PUSH_BYTE (0x89) PUSH_BYTE (0x58) PUSH_BYTE (0xFC) //mov dword ptr [eax - 4], ebx

        PUSH_BYTE (0x83) PUSH_BYTE (0xC0) PUSH_BYTE (0x04) //add eax, 04h

        PUSH_BYTE (0xEB) PUSH_BYTE (0xE6) //jmp Start

        //End :

        PUSH_BYTE (0xDE) PUSH_BYTE (0xD9) //fcompp

        PUSH_BYTE (0x89) PUSH_BYTE (0x48) PUSH_BYTE (0xFC) //mov dword ptr [eax - 4], ecx
        PUSH_BYTE (0x89) PUSH_BYTE (0x10) //mov dword ptr [eax], edx

        RET_NEAR
    }

    MOV_32_REG (EBX)
    int TEMP = (unsigned int)Program [i];
    unsigned char* TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    CALL_32_CONST
    TEMP = DownPos - ((*OutputSize - 1) + 5);
    TEMP_TO_CHAR = (unsigned char*)&TEMP;
    PUSH_BYTE (TEMP_TO_CHAR[0])
    PUSH_BYTE (TEMP_TO_CHAR[1])
    PUSH_BYTE (TEMP_TO_CHAR[2])
    PUSH_BYTE (TEMP_TO_CHAR[3])

    break;
}
