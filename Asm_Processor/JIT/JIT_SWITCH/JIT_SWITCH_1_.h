case (Syntax::SUM):
{
    //FCLEX //Not used to increase speed

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FADD_64_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
case (Syntax::SUB):
{
    //FCLEX //Not used to increase speed

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FSUB_64_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
case (Syntax::MUL):
{
    //FCLEX //Not used to increase speed

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FMUL_64_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
case (Syntax::DIV):
{
    //FCLEX //Not used to increase speed

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x8)

    FDIV_64_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
case (Syntax::MOD):
{
    //FCLEX //Not used to increase speed

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    POP_32_REG (EAX)

    FLD_64_MEMORY_ESP

    ADD_8_ESP
    PUSH_BYTE (0x4)

    FISTP_32_MEMORY_ESP

    MOV_32_REG (EDX)
    PUSH_BYTE (0)
    PUSH_BYTE (0)
    PUSH_BYTE (0)
    PUSH_BYTE (0)

    DIV_32_MEMORY_ESP

    PUSH_32_REG (EDX)

    FILD_32_MEMORY_ESP

    FSTP_64_MEMORY_ESP

    break;
}
