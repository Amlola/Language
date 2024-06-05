#include "../inc/BinaryTransl.h"    


IrError ElfCtor(BinaryTranslate* elf, Ir* ir) 
    {
    elf->buf        = (unsigned char*)calloc(BUFFER_CAP, sizeof(unsigned char));

    elf->buf_size   = 0;

    elf->call_func  = (JMP*)calloc(ir->cur_num_func, sizeof(JMP));

    elf->label      = (JMP*)calloc(ir->num_labels, sizeof(JMP));

    elf->status     = FISRT_TIME;

    return NO_IR_ERROR;
    }


IrError ElfDtor(BinaryTranslate* elf)
    {
    assert(elf);

    free(elf->buf);

    free(elf->call_func);

    free(elf->label);

    elf->buf_size = IR_POIZON_VALUE;

    return NO_IR_ERROR;
    }



IrError GetBinaryFile(FILE* file, BinaryTranslate* elf, Ir* ir, LangNameTableArray* table_array) 
    {
    assert(ir);
    assert(elf);

    BinaryTranslation(elf, ir);

    elf->buf_size = 0;

    elf->status   = SECOND_TIME;

    BinaryTranslation(elf, ir);

    PrintHeaders(file, elf->buf_size);

    //printf("%zu\n", elf->buf_size);

    //GetBinaryLib(file);

    //memcpy((elf->buf + 2), &(elf->buf_size), sizeof(int));

    fwrite(elf->buf, sizeof(unsigned char), elf->buf_size, file);

    //GetData(file, table_array);

    fclose(file);

    return NO_IR_ERROR;
    }


IrError BinaryTranslation(BinaryTranslate* elf, Ir* ir)
    {
    assert(elf);
    assert(ir);

    FillBinaryStart(elf);

    for (size_t cur_func = 0; cur_func < ir->cur_num_func; cur_func++)
        {
        for (size_t cur_command = 0; cur_command <= ir->funcs[cur_func].cur_command; cur_command++)
            {
            switch (GET_COMMAND_VALUE.instr)
                {
                case IR_RET:
                    {
                    FillBuffer(elf, ELF_RET);

                    break;
                    }

                case IR_CALL:
                    {
                    PrintBinaryCall(elf, GET_COMMAND_VALUE, ir);

                    break;
                    }
                
                case IR_OUT:
                    {
                    PrintBinaryOut(elf, ir);

                    break;
                    }

                /*case IR_IN:
                    {
                    PRINT_TO_NASM("call Input");

                    break;
                    }*/

                case IR_ADD:
                case IR_SUB:
                case IR_MULT:
                case IR_DIV:
                    {
                    PrintBinaryMathOperation(elf, GET_COMMAND_VALUE);

                    break;
                    }

                case IR_PUSH:
                    {
                    PrintBinaryPush(elf, &GET_COMMAND_VALUE);

                    break;
                    }

                case IR_POP:
                    {
                    PrintBinaryPop(elf, &GET_COMMAND_VALUE);

                    break;
                    }

                case IR_FUNC: 
                    { 
                    if (elf->status == FISRT_TIME)
                        GetAdressFunc(elf, &ir->funcs[cur_func]);

                    break;
                    }

                /*case IR_MOV:
                    {
                    PrintBinaryMov(elf, GET_COMMAND_VALUE);

                    break;
                    }*/

                /*case IR_LABEL:
                    {
                    if (GET_COMMAND_VALUE.arg1_type == IR_NUM)
                        {
                        fprintf(file, "\n%s_%zu:\n",   
                                                GET_COMMAND_VALUE.name_label, 
                                                GET_COMMAND_VALUE.offset_var);
                        }
                    
                    else
                        fprintf(file, "\n%s:\n",   
                                                GET_COMMAND_VALUE.name_label);
                                                 
                    break;
                    }*/

                default:                    
                    break;
                }
            }
        }

    return NO_IR_ERROR;
    }


IrError PrintBinaryMathOperation(BinaryTranslate* elf, List_type command) 
    {
     if (command.arg1_type == IR_REG && command.instr == IR_ADD)
        {
        FillBuffer(elf, ELF_ADD_R14_NUM);
        GetHexInt(elf, (int)(command.offset_var));
        }

    else if (command.arg1_type == IR_REG && command.instr == IR_SUB)
        {
        FillBuffer(elf, ELF_SUB_R14_NUM);
        GetHexInt(elf, (int)(command.offset_var));
        }

    else    
        {
        FillBuffer(elf, ELF_MOV_XMM0_RSP_PL_8);
        FillBuffer(elf, ELF_MOV_XMM1_RSP);
        
        FillBuffer(elf, ELF_POP_REG);
        elf->buf[elf->buf_size - 1] += IR_RCX;

        switch (command.instr)
            {
            case IR_ADD:  FillBuffer(elf, ELF_ADD_XMM0_XMM1); break;
            case IR_SUB:  FillBuffer(elf, ELF_SUB_XMM0_XMM1); break;
            case IR_MULT: FillBuffer(elf, ELF_MUL_XMM0_XMM1); break;
            case IR_DIV:  FillBuffer(elf, ELF_DIV_XMM0_XMM1); break;
            
            default: break;
            }

        FillBuffer(elf, ELF_MOV_RSP_XMM0);
        }

    return NO_IR_ERROR;
    }


IrError PrintBinaryPush(BinaryTranslate* elf, Ir_command* instr) 
    {
    assert(instr);

    switch (instr->arg1_type)
        {
        case IR_REG:
            {
            FillBuffer(elf, ELF_PUSH_R15);

            break;
            }

        case IR_NUM:
            {
            long long num = 0;
            memcpy(&num, &instr->num, sizeof(double));

            FillBuffer(elf, ELF_MOV_RCX_NUM);
            GetHexLong(elf, num);

            FillBuffer(elf, ELF_PUSH_REG);
            elf->buf[elf->buf_size - 1] += IR_RCX;
            }

        default:
            break;
        }

    return NO_IR_ERROR;
    }


IrError PrintBinaryPop(BinaryTranslate* elf, Ir_command* instr)
    {
    assert(instr);

    switch (instr->arg1_type)
        {
        case IR_REG:
            {
            if (instr->reg1 != IR_R15)
                {
                FillBuffer(elf, ELF_POP_REG);

                elf->buf[elf->buf_size - 1] += instr->reg1;
                }

            else
                FillBuffer(elf, ELF_POP_R15);

            break;
            }

        case IR_MEM_REG_NUM:
            {
            FillBuffer(elf, ELF_POP_R14_NUM_MEM);

            GetHexInt(elf, (int)instr->offset_var);

            break;
            }

        default:
            break;
        }

    return NO_IR_ERROR;
    }


IrError PrintBinaryOut(BinaryTranslate* elf, Ir* ir)
    {
    assert(elf);
    assert(ir);

    FillBuffer(elf, ELF_CALL);

    GetHexInt(elf, ELF_PRINT_ADDRESS - ELF_MAIN_ADDRESS - elf->buf_size);

    return NO_IR_ERROR;
    }


IrError GetAdressFunc(BinaryTranslate* elf, Ir_funcs* func)
    {
    assert(elf);
    assert(func);

    elf->call_func[func->num_func].name    = func->name_func;

    elf->call_func[func->num_func].address = CUR_ADDRESS;

    return NO_IR_ERROR;
    }


IrError PrintBinaryCall(BinaryTranslate* elf, List_type command, Ir* ir)
    {   
    assert(elf);
    assert(ir);

    FillBuffer(elf, ELF_CALL);

    if (elf->status == SECOND_TIME)
        {
        unsigned char* call_address = nullptr;

        for (size_t i = 0; i < ir->cur_num_func; i++)
            {
            if (command.call_func == elf->call_func[i].name)
                {
                call_address = elf->call_func[i].address;
                break;
                }
            }

        GetHexInt(elf, call_address - CUR_ADDRESS - 5);
        }

    return NO_IR_ERROR;  
    }


void FillBinaryStart(BinaryTranslate* elf)
    {
    assert(elf);

    unsigned char start_opcode[] = {0xE8, 0x10, 0x00, 0x00, 0x00,                   // call main
                                   0x48, 0xC7, 0xC0, 0x3C, 0x00, 0x00, 0x00, 0x48, 0xC7, 0xC7, 0x16, 0x00, 0x00, 0x00, 0x0F, 0x05};                         // int 80h

    memcpy(CUR_ADDRESS, &start_opcode, sizeof(start_opcode));

    elf->buf_size += sizeof(start_opcode);
    }


void GetBinaryLib(FILE* file)
    {
    assert(file);

    unsigned char* buffer = (unsigned char*)calloc(LIB_FILE_SIZE, sizeof(unsigned char));

    FILE* lib = fopen("stdlib.o", "rb");

    fseek(lib, 0x180, SEEK_SET);

    fread(buffer, LIB_FILE_SIZE, sizeof(unsigned char), lib);

    fclose(lib);

    fwrite(buffer, LIB_FILE_SIZE, sizeof(unsigned char), file);

    free(buffer);
    }


void GetData(FILE* file, LangNameTableArray* table_array)
    {
    size_t var_buf_size = VAR_BUF_SIZE;

    char* VarBuf = (char*)calloc(var_buf_size, sizeof(char));

    fwrite(VarBuf, var_buf_size, sizeof(char), file);

    free(VarBuf);
    }


void FillBuffer(BinaryTranslate* elf, ALL_CMDS cmd)
    {
    assert(elf);

    size_t len = opcodes_array[cmd].len;

    memcpy(CUR_ADDRESS, opcodes_array[cmd].opcode, len);

    elf->buf_size += len;
    }


void GetHexInt(BinaryTranslate* elf, int num)
    {
    unsigned char hex_num[4] = "";

    for (int i = 0; i < 4; i++)
        {   
        if (num == 0)
            break;
        
        unsigned char x = num & 0xff;
        num = num >> 8;
        hex_num[i] = x;
        }

    memcpy(CUR_ADDRESS, hex_num, 4);

    elf->buf_size += 4;
    }


void GetHexLong(BinaryTranslate* elf, long long num)
    {
    unsigned char hex_num[8] = "";

    for (int i = 0; i < 8; i++)
        {   
        if (num == 0)
            break;
        
        unsigned char x = num & 0xff;
        num = num >> 8;
        hex_num[i] = x;
        }

    memcpy(CUR_ADDRESS, hex_num, 8);

    elf->buf_size += 8;
    }