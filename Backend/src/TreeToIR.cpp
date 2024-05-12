#include "../inc/ir.h"


void IrCtor(Ir* ir, Tree* tree, LangNameTableArray* table_array) 
    {
    ir->num_main_func = tree->main_func - 2;

    ir->name_main_func = GET_FUNC_NAME(tree->main_func);

    ir->funcs = (Ir_funcs*)calloc(table_array->ptr - 2, sizeof(Ir_funcs));

    for (size_t i = 0; i < table_array->ptr - 2; i++)
        ListCtor(&ir->funcs[i].commands);

    ir->cur_num_func   = 0;

    ir->if_label       = 0;

    ir->logical_label  = 0;

    ir->while_label    = 0;

    ir->glob_vars_size = 0;
    }


void IrDtor(Ir* ir)                              
    {
    ir->num_main_func = 0;

    for (size_t i = 0; i < ir->cur_num_func; i++)
        ListDtor(&ir->funcs[i].commands);

    free(ir->funcs);

    ir->name_main_func = nullptr;

    ir->cur_num_func   = IR_POIZON_VALUE;

    ir->if_label       = IR_POIZON_VALUE;

    ir->logical_label  = IR_POIZON_VALUE;

    ir->while_label    = IR_POIZON_VALUE;

    ir->glob_vars_size = IR_POIZON_VALUE;
    }


IrError GetIr(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    Node_t* func_declaration = node;

    while (func_declaration) 
        {
        GetIrDecl(ir, func_declaration->left, table_array);

        func_declaration = func_declaration->right;
        }

    return NO_IR_ERROR;
    }


IrError GetIrDecl(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    if (node->kind.type == VAR_DECL_TYPE)                           
        {
        ADD_GLOBAL_FUNC(node->kind.form.id);

        GetGlobalVars(ir, node->right, table_array);

        return NO_IR_ERROR;
        }
   
    else
        {
        GET_CUR_FUNC.func_num_vars = table_array->Array[ir->cur_num_func + 2].ptr;

        GET_CUR_FUNC.num_func = ir->cur_num_func;

        GET_CUR_FUNC.name_func = GET_FUNC_NAME(table_array->Array[ir->cur_num_func + 2].table_number);

        ADD_FUNC;

        ADD_POP_REG(IR_R15, "\t\t\t\t\t\t; get ret adress\n");

        if (ir->cur_num_func == ir->num_main_func)
            {
            for (size_t i = 0; i < ir->glob_vars_size; i++)
                ADD_CALL(ir->glob_vars[i]);
            }

        GetIrParameters(ir, node->right, table_array);        

        ir->cur_num_func++;
        }

    return NO_IR_ERROR;
    }


IrError GetIrParameters(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    Node_t* saved_comma = node->left;

    Node_t* comma = node->left;

    Node_t* tmp_node = node->left;

    while (comma)
        comma = comma->right;
   
    if (saved_comma->left)
        {
        while (saved_comma != comma)
            {        
            while (tmp_node->right != comma) 
                tmp_node = tmp_node->right;

            comma = tmp_node;

            tmp_node = saved_comma;

            GetIrId(ir, comma->left, table_array);
            }
        }

    GetIrBodyFunc(ir, node->right, table_array);

    return NO_IR_ERROR;
    }


IrError GetIrTypeNode(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    if (!node)
        return NO_IR_ERROR;

     switch (node->kind.type)
        {
        case NUM_TYPE:
            {
            GetIrNum(ir, node, table_array);
            break;
            }

        case ID_TYPE:
            {
            GetIrId(ir, node, table_array);
            break;
            }

        case KEYW_TYPE:
            {
            GetIrKeyword(ir, node, table_array);
            break;
            }

        case VAR_DECL_TYPE:
            {
            GetIrVarDecl(ir, node, table_array);
            break;
            }

        case CALL_TYPE:
            {
            GetIrCall(ir, node, table_array);
            break;
            }

        default: 
            {
            printf("error: unexpected type\n");
            return IR_UNEXPECTED_TYPE;
            break;
            }
        }

    return NO_IR_ERROR;
    }


IrError GetIrNum(Ir* ir, Node_t* node, LangNameTableArray* table_array)   // done
    {
    assert(ir);
    assert(table_array);

    PUSH_NUM(node->kind.form.num);

    return NO_IR_ERROR;
    }


IrError GetIrId(Ir* ir, Node_t* node, LangNameTableArray* table_array)    // done
    {
    assert(ir);
    assert(table_array);

    int var_pos = GetPos(ir, node, table_array);

    size_t index_func = 0;

    size_t cur_var = 0;
       
    if (var_pos == -1)
        {
        index_func = GLOBAL_TABLE_INDEX;

        cur_var = FindInNameTable(&table_array->Array[index_func], node->kind.form.id);
            
        var_pos = GetGlobalStart(table_array) + cur_var - 2;
        }
    
    else
        {
        index_func = ir->cur_num_func + 2;

        cur_var = var_pos;
        }

    if (table_array->Array[index_func].Table[cur_var].init != 0)
            ADD_PUSH_MEM_REG_NUM(IR_R14, (var_pos + 2) * 8);

     else 
        {
        table_array->Array[index_func].Table[cur_var].init++;

        ADD_POP_MEM_REG_NUM(IR_R14, (var_pos + 2) * 8);
        }
    
    return NO_IR_ERROR;
    }


IrError GetIrGlobalId(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    size_t index_func = GLOBAL_TABLE_INDEX;

    size_t cur_var = FindInNameTable(&table_array->Array[index_func], node->kind.form.id);

    int var_pos = GetGlobalStart(table_array) + cur_var - 2;

    table_array->Array[index_func].Table[cur_var].init++;

    ADD_POP_MEM_REG_NUM(IR_R14, (var_pos + 2) * 8);

    return NO_IR_ERROR;
    }


IrError GetIrVarDecl(Ir* ir, Node_t* node, LangNameTableArray* table_array) // done 
    {
    assert(ir);
    assert(table_array);

    if (node->kind.type != VAR_DECL_TYPE)
        return IR_UNEXPECTED_TYPE;

    GetIrAssign(ir, node->right, table_array);

    return NO_IR_ERROR;
    }


IrError GetIrCall(Ir* ir, Node_t* node, LangNameTableArray* table_array) //done
    {
    assert(ir);
    assert(table_array);

    if (node->kind.type != CALL_TYPE)
        return IR_UNEXPECTED_TYPE;

    ADD_PUSH_REG(IR_R15, "\t\t\t\t\t; save ret adress this func\n"); 

    GetIrArgumentSequence(ir, node->left, table_array);

    ADD_ADD(IR_R14, GET_CUR_FUNC.func_num_vars * 8);

    ADD_CALL(node->right->kind.form.id);

    ADD_SUB(IR_R14, GET_CUR_FUNC.func_num_vars * 8);

    ADD_POP_REG(IR_R15, "\t\t\t\t\t\t; get ret adress this func\n"); 

    return NO_IR_ERROR;
    }


IrError GetIrArgumentSequence(Ir* ir, Node_t* node, LangNameTableArray* table_array) // done
    {
    assert(ir);
    assert(table_array);

    Node_t* comma = node;

    if (comma)
        {
        if (comma->kind.form.key_w == KEYW_COMMA)
            {
            while (comma->kind.form.key_w == KEYW_COMMA)
                {
                GetIrTypeNode(ir, comma->left, table_array);

                comma = comma->right;

                if (!comma)
                    break;
                }
            }
            
        else 
            GetIrTypeNode(ir, comma, table_array);
        }

	return NO_IR_ERROR;
    }


IrError GetIrBodyFunc(Ir* ir, Node_t* node, LangNameTableArray* table_array) // done
    {
    assert(ir);
    assert(table_array);

    Node_t* sequential_operator = node;

    while (sequential_operator)
        {
        GetIrTypeNode(ir, sequential_operator->left, table_array);

        sequential_operator = sequential_operator->right;
        }

    return NO_IR_ERROR;
    }


IrError GetIrMathOperation(Ir* ir, Node_t* node, LangNameTableArray* table_array, Instruction instr)
    {
    assert(ir);
    assert(table_array);

    GetIrTypeNode(ir, node->left, table_array);

    if (node->left->kind.type == CALL_TYPE)
        ADD_PUSH_REG(IR_RAX, "\t\t\t\t\t; get return value\n");

    GetIrTypeNode(ir, node->right, table_array);

    if (node->right->kind.type == CALL_TYPE)
        ADD_PUSH_REG(IR_RAX, "\t\t\t\t\t; get return value\n");

    ADD_MATH_OPERATION;

    return NO_IR_ERROR;
    }


IrError GetIrKeyword(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    switch (node->kind.form.key_w)
        {
        case KEYW_ADD:  GetIrMathOperation(ir, node, table_array, IR_ADD);  break;
        case KEYW_SUB:  GetIrMathOperation(ir, node, table_array, IR_SUB);  break;
        case KEYW_MULT: GetIrMathOperation(ir, node, table_array, IR_MULT); break;
        case KEYW_DIV:  GetIrMathOperation(ir, node, table_array, IR_DIV);  break;
        
        case KEYW_JA:   GetIrComparison(ir, node, table_array, IR_JA,  &ir->cond.more,       "more");       break;
        case KEYW_JAE:  GetIrComparison(ir, node, table_array, IR_JAE, &ir->cond.more_equal, "more_equal"); break;
        case KEYW_JB:   GetIrComparison(ir, node, table_array, IR_JB,  &ir->cond.less,       "less");       break;
        case KEYW_JBE:  GetIrComparison(ir, node, table_array, IR_JBE, &ir->cond.less_equal, "less_equal"); break;
        case KEYW_JNE:  GetIrComparison(ir, node, table_array, IR_JNE, &ir->cond.not_equal,  "not_equal");  break;
        case KEYW_JE:   GetIrComparison(ir, node, table_array, IR_JE,  &ir->cond.equal,      "equal");      break;

        case KEYW_AND:  GetLogicalAnd(ir, node, table_array, &ir->cond.and_op, "and"); break;
        case KEYW_OR:   GetLogicalOr (ir, node, table_array, &ir->cond.or_op,  "or"); break;

        case KEYW_SQRT:
            {
            GetIrTypeNode(ir, node->right, table_array);

            ADD_SQRT;

            break;
            }

        case KEYW_INPUT:
            {
            GetIrInput(ir, node, table_array);

            break;
            }

        case KEYW_PRINTF:
            {       
            GetIrPrintf(ir, node, table_array);

            break;
            }

        case KEYW_RETURN:
            {
            GetIrTypeNode(ir, node->right, table_array);

            if (node->right->kind.type == CALL_TYPE)
                ADD_PUSH_REG(IR_RAX, "\t\t\t\t\t; get return value\n");

            ADD_POP_REG(IR_RAX, "\t\t\t\t\t\t; save return value\n"); 
            ADD_PUSH_REG(IR_R15, "\t\t\t\t\t; get ret adress this func\n"); 
            ADD_RET;

            break;
            }

        case KEYW_ASSIGN:
            {
            GetIrAssign(ir, node, table_array);

            break;
            }

        case KEYW_WHILE: 
            {
            GetIrWhile(ir, node, table_array);

            break;
            }

        case KEYW_IF:
            {
            GetIrIf(ir, node, table_array);

            break;
            }

        case KEYW_BREAK:
            {
            ADD_LABEL_JMP(IR_JMP, ir->while_label - 1, "end_while");

            break;
            }

        case KEYW_CONTINUE:
            {
            ADD_LABEL_JMP(IR_JMP, ir->while_label - 1, "start_while");

            break;
            }

        default:
            {
            printf("error: unknown keyword\n");
            return IR_UNKNOWN_KEYWORD;
            break;
            }
        }

    return NO_IR_ERROR;
    }


IrError GetGlobalVars(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    GetIrTypeNode(ir, node->left, table_array);

    GetIrGlobalId(ir, node->right, table_array); 

    ADD_RET;

    strcpy(ir->glob_vars[ir->glob_vars_size++], node->right->kind.form.id);

    return NO_IR_ERROR;
    }


IrError GetIrAssign(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);
        
    GetIrTypeNode(ir, node->left, table_array);

    int var_pos = GetPos(ir, node->right, table_array);

    size_t index_func = 0;
       
    if (var_pos == -1)
        {
        var_pos = FindInNameTable(&table_array->Array[GLOBAL_TABLE_INDEX], node->right->kind.form.id);

        table_array->Array[GLOBAL_TABLE_INDEX].Table[var_pos].init = 0;
        }
    
    else
        {
        index_func = ir->cur_num_func + 2;

        table_array->Array[index_func].Table[var_pos].init = 0;
        }

    if (node->left->kind.type == CALL_TYPE)
        ADD_PUSH_REG(IR_RAX, "\t\t\t\t\t; get return value\n");

    GetIrTypeNode(ir, node->right, table_array); 

    return NO_IR_ERROR;
    }


IrError GetIrPrintf(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    GetIrTypeNode(ir, node->right, table_array);
        
    if (node->right->kind.type == CALL_TYPE)
        ADD_PUSH_REG(IR_RAX, "\t\t\t\t\t; get return value\n");

    ADD_POP_MEM_REG_NUM(IR_R14, GetBufferSize(table_array));

    ADD_ADD(IR_R14, GetBufferSize(table_array));

    ADD_PRINT;

    ADD_SUB(IR_R14, GetBufferSize(table_array));

    return NO_IR_ERROR;
    }


IrError GetIrComparison(Ir* ir, Node_t* node, LangNameTableArray* table_array, Instruction instr, size_t* cond, const char* name)
    {
    assert(ir);
    assert(table_array);

    GetIrTypeNode(ir, node->left, table_array);

    GetIrTypeNode(ir, node->right, table_array);

    ADD_POP_REG(IR_R11, "\t\t\t\t\t\t; get fisrt cmp reg\n");
    
    ADD_POP_REG(IR_R10, "\t\t\t\t\t\t; get second cmp reg\n");

    ADD_CMP;

    ADD_LABEL_JMP(instr, *cond, name);

    ADD_LABEL_JMP(IR_JMP, ir->logical_label, "logic_end");

    ADD_LABEL(*cond, name);

    ADD_MOV_REG_NUM(IR_R12, 1);

    ADD_LABEL(ir->logical_label++, "logic_end");

    (*cond)++;

    return NO_IR_ERROR;
    }


IrError GetLogicalAnd(Ir* ir, Node_t* node, LangNameTableArray* table_array, size_t* cond, const char* name)
    {
    assert(ir);
    assert(table_array);

    GetIrTypeNode(ir, node->left, table_array);

    ADD_CMP_NUM(IR_R12, 1);

    ADD_LABEL_JMP(IR_JNE, *cond, name);

    ADD_MOV_REG_NUM(IR_R12, 0);

    GetIrTypeNode(ir, node->right, table_array);

    ADD_CMP_NUM(IR_R12, 1);

    ADD_LABEL_JMP(IR_JNE, *cond, name);

    ADD_LABEL(*cond, name);

    (*cond)++;

    return NO_IR_ERROR;
    }


IrError GetLogicalOr(Ir* ir, Node_t* node, LangNameTableArray* table_array, size_t* cond, const char* name)
    {
    assert(ir);
    assert(table_array);

    GetIrTypeNode(ir, node->left, table_array);

    ADD_CMP_NUM(IR_R12, 1);

    ADD_LABEL_JMP(IR_JE, *cond, name);

    GetIrTypeNode(ir, node->right, table_array);

    ADD_CMP_NUM(IR_R12, 1);

    ADD_LABEL_JMP(IR_JE, *cond, name);

    ADD_LABEL(*cond, name);

    (*cond)++;

    return NO_IR_ERROR;
    }


IrError GetIrInput(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

    ADD_IN;
    
    ADD_PUSH_REG(IR_RCX, "\t\t\t\t\t; get input value\n");

    return NO_IR_ERROR;
    }


IrError GetIrIf(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    ADD_MOV_REG_NUM(IR_R12, 0);

    GetIrTypeNode(ir, node->left, table_array);

    ADD_CMP_NUM(IR_R12, 1);

    ADD_LABEL_JMP(IR_JNE, ir->if_label, "end_if");

    ADD_MOV_REG_NUM(IR_R12, 0);

    size_t cur_if_count = ir->if_label;

    Node_t* cur_oper = node->right;

    ir->if_label++;

    while (cur_oper)
        {
        GetIrTypeNode(ir, cur_oper->left, table_array);

        cur_oper = cur_oper->right;
        }

    ADD_LABEL(cur_if_count, "end_if");

    return NO_IR_ERROR;
    }


IrError GetIrWhile(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);

   ir->while_label++;

    ADD_LABEL(ir->while_label, "start_while");

    ADD_MOV_REG_NUM(IR_R12, 0);

    GetIrTypeNode(ir, node->left, table_array);

    size_t cur_cycle = ir->while_label++;

    ADD_CMP_NUM(IR_R12, 1);

    ADD_LABEL_JMP(IR_JNE, ir->while_label - 1, "end_while");

    ADD_MOV_REG_NUM(IR_R12, 0);

    Node_t* cur_oper = node->right;

    while (cur_oper)
        {
        GetIrTypeNode(ir, cur_oper->left, table_array);

        cur_oper = cur_oper->right;
        }

    ADD_LABEL_JMP(IR_JMP, cur_cycle, "start_while");

    ADD_LABEL(cur_cycle, "end_while");


    return NO_IR_ERROR;
    }


int GetPos(Ir* ir, Node_t* node, LangNameTableArray* table_array)
    {
    assert(ir);
    assert(table_array);
    
    return FindInNameTable(&table_array->Array[ir->cur_num_func + 2], node->kind.form.id);
    }


void AddCommand(Ir_funcs* func, Instruction instr, ArgumentType type_arg1, 
                                ArgumentType type_arg2, Registers arg1,
                                Registers arg2, double num, const char* comment, 
                                size_t offset_var, const char* call_func, const char* name_label)       
    {
    List_type command = {};

    command.instr     = instr;

    command.arg1_type = type_arg1;
    command.arg2_type = type_arg2;
    command.reg1      = arg1;
    command.reg2      = arg2;
    command.num       = num;

    if (comment)
        strcpy(command.comment, comment);

    command.offset_var = offset_var;

    command.call_func = call_func;

    command.name_label = name_label;

    PushBack(&func->commands, command);

    func->cur_command++;
    }


size_t GetBufferSize(LangNameTableArray* table_array)
    {
    assert(table_array);

    return ((table_array->Array[GENERAL_TABLE_INDEX].ptr - (table_array->ptr - 2)) * (table_array->ptr - 2) + 4) * 8;  // (num_id - num_func) * num_func + input + print
    }


size_t GetGlobalStart(LangNameTableArray* table_array)
    {
    assert(table_array);

    return ((table_array->Array[GENERAL_TABLE_INDEX].ptr - table_array->Array[GLOBAL_TABLE_INDEX].ptr) * (table_array->ptr - 2) + 2);
    }