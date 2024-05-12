#include "../inc/TranslateToNasm.h"


IrError NasmTranslation(Ir* ir, FILE* file) 
    {
    FillNasmStart(file, ir);

    for (size_t cur_func = 0; cur_func < ir->cur_num_func; cur_func++)
        {
        for (size_t cur_command = 0; cur_command <= ir->funcs[cur_func].cur_command; cur_command++)
            {
            switch (GET_COMMAND_VALUE.instr)
                {
                case IR_RET:
                    {
                    PRINT_TO_NASM("ret");
                    PRINT_NASM_NEW_LINE;

                    break;
                    }

                case IR_CALL:
                    {
                    fprintf(file, "\t\tcall %s \n", GET_COMMAND_VALUE.call_func);

                    break;
                    }
                
                case IR_OUT:
                    {
                    PRINT_TO_NASM("call Print");

                    break;
                    }

                case IR_IN:
                    {
                    PRINT_TO_NASM("call Input");

                    break;
                    }

                case IR_PUSH:
                    {
                    PrintPushToNasm(file, &GET_COMMAND_VALUE);

                    break;
                    }

                case IR_POP:
                    {
                    PrintPopToNasm(file, &GET_COMMAND_VALUE);

                    break;
                    }

                case IR_FUNC: 
                    { 
                    fprintf(file, "\n%s:\n", ir->funcs[cur_func].name_func);

                    break;
                    }

                case IR_LABEL:
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
                    }

                 case IR_CMP:
                    {   
                    if (GET_COMMAND_VALUE.arg2_type == IR_NUM)
                        fprintf(file, "\t\tcmp %s, %zu\n", 
                                            register_array[GET_COMMAND_VALUE.reg1],
                                            (size_t)GET_COMMAND_VALUE.num);

                    else 
                        {
                        fprintf(file, "\t\tcmp %s, %s\n",
                                            register_array[GET_COMMAND_VALUE.reg1],
                                            register_array[GET_COMMAND_VALUE.reg2]);  
                        }

                    break;
                    }

                case IR_ADD:
                case IR_SUB:
                case IR_MULT:
                case IR_DIV:
                    {
                    PrintMathOperationToNasm(file, GET_COMMAND_VALUE);

                    break;
                    }

                case IR_JA:  
                case IR_JAE: 
                case IR_JB:  
                case IR_JBE: 
                case IR_JE:  
                case IR_JNE: 
                case IR_JMP:
                    {
                    PrintConditionalToNasm(file, GET_COMMAND_VALUE, ir, cur_func, cur_command);

                    break;
                    }

                case IR_SQRT: 
                    {
                    PrintSqrtToNasm(file);

                    break;
                    }

                case IR_MOV:
                    {
                    fprintf(file, "\t\tmov %s, %zu\t\t\t\t\t; conditional reg\n",  
                                            register_array[GET_COMMAND_VALUE.reg1], 
                                            (size_t)GET_COMMAND_VALUE.num);

                    break;
                    }
                
                default:
                    {
                    printf("unexpected type\n");
                    return IR_UNEXPECTED_TYPE;

                    break;
                    }
                }
            }
        }

    PRINT_NASM_NEW_LINE;

    return NO_IR_ERROR;
    }


void PrintMathOperationToNasm(FILE* file, List_type command) 
    {
    if (command.arg1_type == IR_REG && command.instr == IR_ADD)
        fprintf(file, "\t\tadd %s, %zu\n", register_array[command.reg1], command.offset_var);

    else if (command.arg1_type == IR_REG && command.instr == IR_SUB)
        fprintf(file, "\t\tsub %s, %zu\n", register_array[command.reg1], command.offset_var);
    
    else 
        {
        PRINT_TO_NASM("movsd xmm0, qword [rsp + 8]");
        PRINT_TO_NASM("movsd xmm1, qword [rsp]");
        PRINT_TO_NASM("pop rcx");

        switch (command.instr)
            {
            case IR_ADD:  PRINT_TO_NASM("addsd xmm0, xmm1"); break;
            case IR_SUB:  PRINT_TO_NASM("subsd xmm0, xmm1"); break;
            case IR_MULT: PRINT_TO_NASM("mulsd xmm0, xmm1"); break;
            case IR_DIV:  PRINT_TO_NASM("divsd xmm0, xmm1"); break;
            
            default: break;
            }

        PRINT_TO_NASM("movsd qword [rsp], xmm0\n");
        PRINT_NASM_NEW_LINE;        
        }
    }


void PrintConditionalToNasm(FILE* file, List_type com_instr, Ir* ir, size_t cur_func, size_t cur_command) 
    {
    const char* name_label = GET_COMMAND_VALUE.name_label;

    size_t offset          = GET_COMMAND_VALUE.offset_var;

    switch (com_instr.instr)
        {
        case IR_JA:  fprintf(file, "\t\tjg %s_%zu\n",  name_label, offset); break;
        case IR_JAE: fprintf(file, "\t\tjge %s_%zu\n", name_label, offset); break;
        case IR_JB:  fprintf(file, "\t\tjl %s_%zu\n",  name_label, offset); break;
        case IR_JBE: fprintf(file, "\t\tjle %s_%zu\n", name_label, offset); break;
        case IR_JE:  fprintf(file, "\t\tje %s_%zu\n",  name_label, offset); break;
        case IR_JNE: fprintf(file, "\t\tjne %s_%zu\n", name_label, offset); break;
        case IR_JMP: fprintf(file, "\t\tjmp %s_%zu\n", name_label, offset); break;
    
        default:    
            break;
        }
    }


void PrintPushToNasm(FILE* file, Ir_command* instr) 
    {
    assert(file);
    assert(instr);

    switch (instr->arg1_type)
        {
        case IR_REG:
            {
            fprintf(file, "\t\tpush %s%s", register_array[instr->reg1], instr->comment);
            break;
            }

        case IR_MEM_REG_NUM:
            {
            fprintf(file, "\t\tpush qword [%s + %zu]\n", register_array[instr->reg1], instr->offset_var);
            break;
            }

        case IR_NUM:
            {
            long long num = 0;
            memcpy(&num, &instr->num, sizeof(double));
            fprintf(file, "\t\tmov rcx, 0x%llx \n", num);
            fprintf(file, "\t\tpush rcx\n");
            }
        
        default:
            break;
        }
    }


void PrintPopToNasm(FILE* file, Ir_command* instr) 
    {
    assert(file);
    assert(instr);

    switch (instr->arg1_type)
        {
        case IR_REG:
            {
            fprintf(file, "\t\tpop %s%s", register_array[instr->reg1], instr->comment);
            break;
            }

        case IR_MEM_REG_NUM:
            {
            fprintf(file, "\t\tpop qword [%s + %zu]\n", register_array[instr->reg1], instr->offset_var);
            break;
            }

        default:
            break;
        }
    }


void PrintSqrtToNasm(FILE* file)
    {
    PRINT_NASM_NEW_LINE;

    PRINT_TO_NASM("movsd xmm1, qword [rsp]");
    PRINT_TO_NASM("sqrtsd xmm0, xmm1");
    PRINT_TO_NASM("movsd qword [rsp], xmm0");

    PRINT_NASM_NEW_LINE;
    }


void FillNasmStart(FILE* file, Ir* ir)
    {
    assert (file);
    assert(ir);

    fprintf(file,
            "%%include \"stdlib.s\"\n"
            "global _start\n\n"
            "section .text\n"
            "_start:\n\n"
            "\t\tmov r14, var_buf\n"
            "\t\tcall %s\n"
            "\t\tmov eax, 1\n"
            "\t\tmov ebx, 0\n"
            "\t\tint 80h\n\n",
            ir->name_main_func
            );
    }


void FillNasmData(FILE* file, LangNameTableArray* table_array) 
    {
    assert(file);
    assert(table_array);

    fprintf(file, "section .data\n");
    fprintf(file, "var_buf: times %zu db 0\n", GetBufferSize(table_array));
    }


#undef GET_CUR_FUNC
#undef GET_FUNC_NAME