#pragma once

#define GET_CUR_FUNC ir->funcs[ir->cur_num_func]

#define GET_FUNC_NAME(num_func) FindInNameTableByCode(&table_array->Array[GENERAL_TABLE_INDEX], num_func)


#define GET_COMMAND_VALUE ir->funcs[cur_func].commands.data[cur_command].value

#define PRINT_TO_NASM(str) fprintf(file, "\t\t%s\n", str);

#define PRINT_NASM_NEW_LINE fprintf(file, "\n");



#define ADD_LABEL_JMP(instr, label, name) AddCommand(&GET_CUR_FUNC, instr, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, label, nullptr, name)

#define ADD_LABEL(label, name)            AddCommand(&GET_CUR_FUNC, IR_LABEL, IR_NUM, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, label, nullptr, name)

#define ADD_MOV_REG_NUM(reg, num)         AddCommand(&GET_CUR_FUNC, IR_MOV, IR_REG, IR_NO_ARG, reg, IR_NO_REG, num, nullptr, 0, nullptr, nullptr)

#define ADD_PUSH_REG(reg, comment)        AddCommand(&GET_CUR_FUNC, IR_PUSH, IR_REG, IR_NO_ARG, reg, IR_NO_REG, 0, comment, 0, nullptr, nullptr)

#define ADD_IN                            AddCommand(&GET_CUR_FUNC, IR_IN, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, nullptr)

#define ADD_POP_REG(reg, comment)         AddCommand(&GET_CUR_FUNC, IR_POP, IR_REG, IR_NO_ARG, reg, IR_NO_REG, 0, comment, 0, nullptr, nullptr)

#define ADD_CMP                           AddCommand(&GET_CUR_FUNC, IR_CMP, IR_REG, IR_REG, IR_R10, IR_R11, 0, nullptr, 0, nullptr, nullptr)

#define ADD_CMP_NUM(reg, num)             AddCommand(&GET_CUR_FUNC, IR_CMP, IR_REG, IR_NUM, reg, IR_NO_REG, num, nullptr, 0, nullptr, nullptr)

#define ADD_SUB(reg, num)                 AddCommand(&GET_CUR_FUNC, IR_SUB, IR_REG, IR_NUM, reg, IR_NO_REG, 0, nullptr, num, nullptr, nullptr)

#define ADD_PRINT                         AddCommand(&GET_CUR_FUNC, IR_OUT, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, nullptr)
 
#define ADD_ADD(reg, num)                 AddCommand(&GET_CUR_FUNC, IR_ADD, IR_REG, IR_NUM, reg, IR_NO_REG, 0, nullptr, num, nullptr, nullptr)

#define ADD_POP_MEM_REG_NUM(reg, num)     AddCommand(&GET_CUR_FUNC, IR_POP, IR_MEM_REG_NUM, IR_NUM, reg, IR_NO_REG, 0, nullptr, num, nullptr, nullptr)

#define ADD_RET                           AddCommand(&GET_CUR_FUNC, IR_RET, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, nullptr)

#define ADD_SQRT                          AddCommand(&GET_CUR_FUNC, IR_SQRT, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, nullptr)

#define ADD_CALL(name)                    AddCommand(&GET_CUR_FUNC, IR_CALL, IR_FUNC_TYPE, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, name, nullptr)

#define PUSH_NUM(num)                     AddCommand(&GET_CUR_FUNC, IR_PUSH, IR_NUM, IR_NO_ARG, IR_NO_REG, IR_NO_REG, num, nullptr, 0, nullptr, nullptr)

#define ADD_PUSH_MEM_REG_NUM(reg, num)    AddCommand(&GET_CUR_FUNC, IR_PUSH, IR_MEM_REG_NUM, IR_NUM, reg, IR_NO_REG, 0, nullptr, num, nullptr, nullptr)

#define ADD_MATH_OPERATION                AddCommand(&GET_CUR_FUNC, instr, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, nullptr)

#define ADD_FUNC                          AddCommand(&GET_CUR_FUNC, IR_FUNC, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, nullptr)

#define ADD_GLOBAL_FUNC(name)             AddCommand(&GET_CUR_FUNC, IR_LABEL, IR_NO_ARG, IR_NO_ARG, IR_NO_REG, IR_NO_REG, 0, nullptr, 0, nullptr, name)