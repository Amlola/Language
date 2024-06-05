#include "../inc/Frontend.h"


#define GET_TOKEN_VALUE tokens->data[Begin(tokens)].value

#define ADD_TO_GLOBAL_TABLE(type)                                                                                    \
        AddToNameTableIfNotFind(&table_array->Array[GLOBAL_TABLE_INDEX], name_func.form.id,                          \
                                FindInNameTable(&table_array->Array[GENERAL_TABLE_INDEX], name_func.form.id), type); \

#define PRINT_SYNTAX_ERROR(line, str_message, str_error)              \
        fprintf(stderr, "line: %zu\n", line);                         \
        fprintf(stderr, "error: %s '%s'\n", str_message, str_error);  \

#define FIND_SYNTAX_ERROR(ptr)       \
        if (!ptr)                    \
            return nullptr;          \

#define CREATE_OPERATION_NODE CreateNode(fict_token, KEYW_TYPE, oper_node, nullptr)
#define PRINT_OR_IDENT (oper.type == KEYW_TYPE && (oper.form.key_w == KEYW_PRINTF)) || oper.type == ID_TYPE


/*----------------------------------------------------PARSER--------------------------------------------------------*/


Node_t* GetGlobalNameTable(LIST* tokens, LangNameTableArray* table_array) 
	{
	NameTableCtor(&(table_array->Array[GLOBAL_TABLE_INDEX]));

	table_array->Array[GLOBAL_TABLE_INDEX].table_number = GLOBAL_TABLE_ID;

    table_array->ptr = 1;

	return GetGrammar(tokens, table_array);
	}


Node_t* GetGrammar(LIST* tokens, LangNameTableArray* table_array)
    {
    printf("I AM IN GET GRAMMAR\n\n");

    Node_t* func = GetDefFunc(tokens, table_array);

    FIND_SYNTAX_ERROR(func);

    List_type fict_token = GetFictToken(KEYW_END);

    if (GET_TOKEN_VALUE.form.key_w == KEYW_ENDOF)
        {
        Node_t* res = CreateNode(fict_token, KEYW_TYPE, func, nullptr);
        FIND_SYNTAX_ERROR(res);
        return res;
        }
    
    else 
        {
        Node_t* grammar = GetGrammar(tokens, table_array);
        FIND_SYNTAX_ERROR(grammar);
        func = CreateNode(fict_token, KEYW_TYPE, func, grammar);
        }

	return func;
    }


Node_t* GetDefFunc(LIST* tokens, LangNameTableArray* table_array) 
    {
    printf("I AM IN DEF_FUNC\n");

    FIND_SYNTAX_ERROR(CheckInitialization(GET_TOKEN_VALUE));

	List_type type = GET_TOKEN_VALUE;

	PopFront(tokens);

    List_type name_func = GET_TOKEN_VALUE;

    FIND_SYNTAX_ERROR(CheckIdName(name_func))

    Node_t* global_var = GetGlobalVariable(tokens, table_array, name_func, type);

    if (global_var)
        {
        PopFront(tokens);

        ADD_TO_GLOBAL_TABLE(VAR_NAME);

        return global_var;
        }

    ADD_TO_GLOBAL_TABLE(FUNC_NAME);

    CheckTableArraySize(table_array);                                                                            

    NameTableCtor(&(table_array->Array[table_array->ptr]));

    PopFront(tokens);

    FIND_SYNTAX_ERROR(CheckOpenRoundBracket(tokens));

    Node_t* left = GetParamSequence(tokens, table_array);

    FIND_SYNTAX_ERROR(left);

    FIND_SYNTAX_ERROR(CheckCloseRoundBracket(tokens));

    FIND_SYNTAX_ERROR(CheckOpenFigBracket(tokens));

    Node_t* right = GetOperatorsSequence(tokens, table_array);

    FIND_SYNTAX_ERROR(right);

	FIND_SYNTAX_ERROR(CheckCloseFigBracket(tokens));

	Node_t* type_func = CreateNode(type, KEYW_TYPE, nullptr, nullptr);

	Node_t* function  = CreateNode(GET_TOKEN_VALUE, PARAM_TYPE, left, right);

	return CreateNode(name_func, FUNC_DEF_TYPE, type_func, function);
    }


Node_t* GetGlobalVariable(LIST* tokens, LangNameTableArray* table_array, List_type var_name, List_type var_type)
    {
    printf("I AM IN GLOBAL VARS\n");

    PopFront(tokens);

    Node_t* rValue = CreateNode(var_name, ID_TYPE, nullptr, nullptr);

    Node_t* type = CreateNode(var_type, KEYW_TYPE, nullptr, nullptr);

    Node_t* var = CheckAssign(tokens, table_array, var_name, var_type, rValue, type);

    if (var)
        return var;

    free(rValue);

    free(type);

    PushFront(tokens, var_name);

    return nullptr;
    }


Node_t* GetOperatorsSequence(LIST* tokens, LangNameTableArray* table_array)
    {   
    printf("I AM IN GET OPERATOR SEQ\n");

	Node_t* first_oper = GetOperator(tokens, table_array);

    FIND_SYNTAX_ERROR(first_oper);
    
	Node_t* current_oper = first_oper;

    while (GET_TOKEN_VALUE.form.key_w == KEYW_END)
        {		
		PopFront(tokens);

        if (GET_TOKEN_VALUE.form.key_w == KEYW_ENDOF)
            {
            PRINT_SYNTAX_ERROR(GET_TOKEN_VALUE.line, "missing closing bracket", "sent");
            return nullptr;
            }

        if (GET_TOKEN_VALUE.form.key_w == KEYW_CLFIG)
            return first_oper;

        printf("I AM IN GET OPERATOR SEQ2\n");

		Node_t* second_oper = GetOperator(tokens, table_array);

        FIND_SYNTAX_ERROR(second_oper);

        current_oper->right = second_oper;   

		current_oper = second_oper;
        }

    return first_oper;
    }


Node_t* GetOperator(LIST* tokens, LangNameTableArray* table_array)
    {
    printf("I AM IN GET OPERATOR\n");

	List_type oper = GET_TOKEN_VALUE;

    Node_t* oper_node = nullptr;

	List_type fict_token = GetFictToken(KEYW_END);

    if (oper.type == NUM_TYPE)
        {
        fprintf(stderr, "line: %zu\n", oper.line);
        fprintf(stderr, "error: unknown use of number\n");
        return nullptr;
        }

    if (PRINT_OR_IDENT)
        {
        Node_t* oper_node = GetNotInitToken(tokens, table_array);

        FIND_SYNTAX_ERROR(oper_node);

        return CREATE_OPERATION_NODE;
        }

	if (CheckOperators(oper))
	    {
        switch (oper.form.key_w)
            {
            case KEYW_WHILE:
                {
                oper_node = GetWhile(tokens, table_array);
                
                break;
                }

            case KEYW_IF:
                {
                oper_node = GetIf(tokens, table_array);
                break;
                }

            case KEYW_RETURN:
                {
                oper_node = GetReturn(tokens, table_array);
                break;
                }

            case KEYW_INIT:
                {
                oper_node = GetInit(tokens, table_array);
                break;
                }

            case KEYW_BREAK:
            case KEYW_CONTINUE:
                {
                oper_node = GetCyclesOperators(tokens, oper);
                break;
                }
                        
            default:
                {
                return nullptr;
                break;
                }
            }
        }

    FIND_SYNTAX_ERROR(oper_node);

	return CREATE_OPERATION_NODE;
    }


Node_t* GetCyclesOperators(LIST* tokens, List_type token) 
    {
    Node_t* oper_node = CreateNode(token, KEYW_TYPE, nullptr, nullptr);

    PopFront(tokens);

    FIND_SYNTAX_ERROR(CheckSequentialOperator(GET_TOKEN_VALUE));

    return oper_node;
    }


Node_t* GetInit(LIST* tokens, LangNameTableArray* table_array)
    {	
    printf("I AM IN GET INIT\n");                             

    List_type type_var = GET_TOKEN_VALUE;

	Node_t* type = CreateNode(type_var, KEYW_TYPE, nullptr, nullptr);

	PopFront(tokens);

    List_type var_name = GET_TOKEN_VALUE;

	FIND_SYNTAX_ERROR(CheckIdName(var_name));

	Node_t* rValue = GetVar(tokens, table_array);

    FIND_SYNTAX_ERROR(GetInitVarInNameTable(table_array, var_name));

	Node_t* var = CheckAssign(tokens, table_array, var_name, type_var, rValue, type);

    if (var)
        return var;

	else
        {
        PRINT_SYNTAX_ERROR(var_name.line, "you need to initialize variable", var_name.form.id);
		return nullptr;
        }
    }


Node_t* GetNotInitToken(LIST* tokens, LangNameTableArray* table_array)
    {
    printf("I AM IN GET NOT INIT\n");
	List_type func = GET_TOKEN_VALUE;

	PopFront(tokens);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_OPRND)
        {
        if (func.type == ID_TYPE)
            FIND_SYNTAX_ERROR(CheckIdName(func));

        Node_t* func_node = GetFunc(tokens, func, table_array);

        FIND_SYNTAX_ERROR(CheckSequentialOperator(GET_TOKEN_VALUE));

		return func_node;
        }

	else
        {
        LangNameTable* table = &table_array->Array[table_array->ptr];

        size_t pos = FindInNameTable(table, func.form.id);

        size_t global_pos = FindInNameTable(&table_array->Array[GLOBAL_TABLE_INDEX], func.form.id);

        if (pos != -1 && table->Table[pos].init == 1 || global_pos != -1)
            {
            PushFront(tokens, func);

            Node_t* assign = GetAssign(tokens, table_array);

            FIND_SYNTAX_ERROR(CheckSequentialOperator(GET_TOKEN_VALUE));

            return assign;
            }
        
        else 
            {
            PRINT_SYNTAX_ERROR(func.line, "uninitialized variable", func.form.id);
            return nullptr;
            }
        }                                                          
    }


Node_t* GetBaseFunc(LIST* tokens, LangNameTableArray* table_array)
    {
    printf("I AM IN BASE FUNC\n");

	List_type func = GET_TOKEN_VALUE;

	PopFront(tokens);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_OPRND)
		return GetFunc(tokens, func, table_array);

	else 
		PushFront(tokens, func);

	return GetVar(tokens, table_array);
    }


Node_t* GetFunc(LIST* tokens, List_type func, LangNameTableArray* table_array)
    {
	printf("I AM IN GET_FUNC\n");

	PopFront(tokens);

	Node_t* name_func = CreateNode(func, ID_TYPE, nullptr, nullptr);

	Node_t* arg_branch = GetArgumentSequence(tokens, table_array);

	FIND_SYNTAX_ERROR(CheckCloseRoundBracket(tokens));

	if (func.type == KEYW_TYPE)
        {
        free(name_func);
		return CreateNode(func, KEYW_TYPE, nullptr, arg_branch);
        }

	else if (func.type == ID_TYPE)
		return CreateNode(func, CALL_TYPE, arg_branch, name_func);
    
    return nullptr;
    }


Node_t* GetArgumentSequence(LIST* tokens, LangNameTableArray* table_array)
	{
	printf("I AM IN GET_ARG_SEQUENCE\n");

	Node_t* first_arg = GetAddSub(tokens, table_array);

    if (GET_TOKEN_VALUE.form.key_w != KEYW_COMMA)
        return first_arg;

    List_type fict_token = GetFictToken(KEYW_COMMA);

    Node_t* comma = CreateNode(fict_token, KEYW_TYPE, first_arg, nullptr);

    Node_t* result = comma;

	while (GET_TOKEN_VALUE.form.key_w == KEYW_COMMA)
		{
		PopFront(tokens);

        Node_t* second_arg = GetAddSub(tokens, table_array);

        comma->right = CreateNode(fict_token, KEYW_TYPE, second_arg, nullptr);

        comma = comma->right;
		}

	return result;
	}


Node_t* GetOr(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET OR\n");

	Node_t* first_expression = GetAnd(tokens, table_array);

	while (GET_TOKEN_VALUE.form.key_w == KEYW_OR)
	    {
        List_type token = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* second_expression = GetAnd(tokens, table_array);

		first_expression = CreateNode(token, KEYW_TYPE, first_expression, second_expression);
	    }

	return first_expression;
    }


Node_t* GetAnd(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET AND\n");

	Node_t* first_expression = GetLogicalExpression(tokens, table_array);

	while (GET_TOKEN_VALUE.form.key_w == KEYW_AND)
	    {
        List_type and_oper = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* second_expression = GetLogicalExpression(tokens, table_array);

		first_expression = CreateNode(and_oper, KEYW_TYPE, first_expression, second_expression);
	    }

	return first_expression;	
    }


Node_t* GetLogicalExpression(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET LOGICAL EXPRESSION\n");

	if (GET_TOKEN_VALUE.form.key_w == KEYW_OPRND)
	    {
		PopFront(tokens);

		Node_t* expression = GetOr(tokens, table_array);

		FIND_SYNTAX_ERROR(CheckCloseRoundBracket(tokens));

		return expression;
	    }

	else 
        return GetComparison(tokens, table_array);	
    }


Node_t* GetComparison(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN Comparison\n");

	Node_t* first_expression = GetAddSub(tokens, table_array);
	
	List_type cmp_oper = GET_TOKEN_VALUE;

	PopFront(tokens);

	Node_t* second_expression = GetAddSub(tokens, table_array);

	return CreateNode(cmp_oper, KEYW_TYPE, first_expression, second_expression);
    }


Node_t* GetMultDiv(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET MultDiv\n");

	Node_t* first_expression = GetPower(tokens, table_array);

	while (GET_TOKEN_VALUE.form.key_w == KEYW_MULT || GET_TOKEN_VALUE.form.key_w == KEYW_DIV)
	    {
	    List_type operation = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* second_expression = GetPower(tokens, table_array);

		first_expression = CreateNode(operation, KEYW_TYPE, first_expression, second_expression);
	    }

	return first_expression;
    }


Node_t* GetPower(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN POWER\n");

	Node_t* first_expression = GetExpression(tokens, table_array);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_POW)
	    {
        List_type operation = GET_TOKEN_VALUE;

        PopFront(tokens);

		Node_t* second_expression = GetPower(tokens, table_array);

		first_expression = CreateNode(operation, KEYW_TYPE, first_expression, second_expression);
	    }

	return first_expression;
    }


Node_t* GetAddSub(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN AddSub\n");

	Node_t* first_expression = GetMultDiv(tokens, table_array);

	while (GET_TOKEN_VALUE.form.key_w == KEYW_ADD || GET_TOKEN_VALUE.form.key_w == KEYW_SUB)
	    {
		List_type operation = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* second_expression = GetMultDiv(tokens, table_array);

		first_expression = CreateNode(operation, KEYW_TYPE, first_expression, second_expression);
	    }

	return first_expression; 
    }
    

Node_t* GetExpression(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN EXPRESSION\n");

	if (GET_TOKEN_VALUE.form.key_w == KEYW_OPRND)
	    {
		PopFront(tokens);

		Node_t* expression = GetAddSub(tokens, table_array);

		FIND_SYNTAX_ERROR(CheckCloseRoundBracket(tokens));

		return expression;
	    }

	else 
        return GetBaseFunc(tokens, table_array);
    }


Node_t* GetAssign(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN ASSIGN\n");

	Node_t* rValue = GetVar(tokens, table_array);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_ASSIGN)
	    {
        List_type assign_token = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* lValue = GetAddSub(tokens, table_array);

		return CreateNode(assign_token, KEYW_TYPE, lValue, rValue);
	    }

	else
        {
        fprintf(stderr, "line: %zu\n", GET_TOKEN_VALUE.line);
		fprintf(stderr, "error: unknown operator\n");
        }

    return nullptr;
    }


Node_t* GetWhile(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN WHILE\n");

	PopFront(tokens);

    int init_array[NAME_TABLE_CAPACITY] = {};
    GetInitArray(init_array, table_array);

	FIND_SYNTAX_ERROR(CheckOpenRoundBracket(tokens));

	Node_t* state_while = GetOr(tokens, table_array);

	FIND_SYNTAX_ERROR(CheckCloseRoundBracket(tokens));

	FIND_SYNTAX_ERROR(CheckOpenFigBracket(tokens));

	Node_t* body_while = GetOperatorsSequence(tokens, table_array);

    FIND_SYNTAX_ERROR(body_while);

	FIND_SYNTAX_ERROR(CheckCloseFigBracket(tokens));

	List_type while_branch = {};

	while_branch.type = KEYW_TYPE;

	while_branch.form.key_w = KEYW_WHILE;

    List_type fict_token = GetFictToken(KEYW_END);

    PushFront(tokens, fict_token);

    LangNameTable* table = &table_array->Array[table_array->ptr];

    BeginningOfInitVar(table, init_array);

	return CreateNode(while_branch, KEYW_TYPE, state_while, body_while);
    }


Node_t* GetIf(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN IF\n");

	PopFront(tokens);

    int init_array[NAME_TABLE_CAPACITY] = {};

    GetInitArray(init_array, table_array);

	FIND_SYNTAX_ERROR(CheckOpenRoundBracket(tokens));

	Node_t* state_if = GetOr(tokens, table_array);

	FIND_SYNTAX_ERROR(CheckCloseRoundBracket(tokens));

	Node_t* body_if = GetElse(tokens, table_array);

    FIND_SYNTAX_ERROR(body_if);

	List_type if_type = {};

	if_type.type = KEYW_TYPE;                                 

	if_type.form.key_w = KEYW_IF;

    LangNameTable* table = &table_array->Array[table_array->ptr];

    BeginningOfInitVar(table, init_array);

	return CreateNode(if_type, KEYW_TYPE, state_if, body_if);
    }


Node_t* GetElse(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN ELSE\n");

	FIND_SYNTAX_ERROR(CheckOpenFigBracket(tokens));

	Node_t* if_branch = GetOperatorsSequence(tokens, table_array);

	FIND_SYNTAX_ERROR(CheckCloseFigBracket(tokens));

	if (GET_TOKEN_VALUE.form.key_w == KEYW_ELSE)
	    {
		PopFront(tokens);

		Node_t* else_branch = nullptr;

		List_type branch = {};

		branch.type = KEYW_TYPE;

		if (GET_TOKEN_VALUE.form.key_w != KEYW_IF)
		    {
			FIND_SYNTAX_ERROR(CheckOpenFigBracket(tokens));

			else_branch = GetOperatorsSequence(tokens, table_array);

            FIND_SYNTAX_ERROR(else_branch);

			FIND_SYNTAX_ERROR(CheckCloseFigBracket(tokens));

            branch.form.key_w = KEYW_ELSE;

            List_type fict_token = GetFictToken(KEYW_END);

            PushFront(tokens, fict_token);
		    }

		else
		    {
			branch.form.key_w = KEYW_IF;

			else_branch = GetIf(tokens, table_array);

            FIND_SYNTAX_ERROR(else_branch);
		    }

		return CreateNode(branch, KEYW_TYPE, if_branch, else_branch);
	    }

    else 
        {
        List_type fict_token = GetFictToken(KEYW_END);

        PushFront(tokens, fict_token);
        }

	return if_branch;
    }
  

Node_t* GetParamSequence(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN PARAM SEQ\n");

	List_type fict_token = GetFictToken(KEYW_COMMA);

    if (GET_TOKEN_VALUE.type == KEYW_TYPE && GET_TOKEN_VALUE.form.key_w == KEYW_CLRND)
        return CreateNode(fict_token, KEYW_TYPE, nullptr, nullptr);

	Node_t* fisrt_parametr = GetParam(tokens, table_array);

    FIND_SYNTAX_ERROR(fisrt_parametr);

	Node_t* comma = CreateNode(fict_token, KEYW_TYPE, fisrt_parametr, nullptr);

    Node_t* result = comma;

    if (GET_TOKEN_VALUE.form.key_w == KEYW_COMMA)
        {
        while (GET_TOKEN_VALUE.form.key_w == KEYW_COMMA)
            {
            List_type fict_token = GET_TOKEN_VALUE;

            PopFront(tokens);

            Node_t* second_parametr = GetParam(tokens, table_array);

            FIND_SYNTAX_ERROR(second_parametr);

            comma->right = CreateNode(fict_token, KEYW_TYPE, second_parametr, nullptr);

            comma = comma->right;
            }
        }
    else
        {
        if (GET_TOKEN_VALUE.type != KEYW_TYPE || GET_TOKEN_VALUE.form.key_w != KEYW_CLRND)
            {
            PRINT_SYNTAX_ERROR(GET_TOKEN_VALUE.line, "in function definition missing enumeration operator", ",");
            return nullptr;
            }
        }

	return result;
    }


Node_t* GetParam(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GetParam\n");

    FIND_SYNTAX_ERROR(CheckInitialization(GET_TOKEN_VALUE));

    if ((GET_TOKEN_VALUE.type == KEYW_TYPE) && (GET_TOKEN_VALUE.form.key_w == KEYW_INIT))
        {
        List_type fict_token = GET_TOKEN_VALUE;

        Node_t* type_var = CreateNode(fict_token, KEYW_TYPE, nullptr, nullptr);

        PopFront(tokens);

        List_type var_name = GET_TOKEN_VALUE;

        FIND_SYNTAX_ERROR(CheckIdName(var_name));
        
        Node_t* node_var = GetVar(tokens, table_array);

        FIND_SYNTAX_ERROR(GetInitVarInNameTable(table_array, var_name));

        return CreateNode(var_name, VAR_DECL_TYPE, type_var, node_var);
        }

    else
        return GetVar(tokens, table_array);
    }


Node_t* GetVar(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET VAR\n");

	if (GET_TOKEN_VALUE.type == ID_TYPE)
        {
        List_type var = GET_TOKEN_VALUE;

        size_t position = FindInNameTable(&table_array->Array[GENERAL_TABLE_INDEX], var.form.id);

        AddToNameTableIfNotFind(&table_array->Array[table_array->ptr], var.form.id, position, VAR_NAME);        

        PopFront(tokens);

		return CreateNode(var, ID_TYPE, nullptr, nullptr);
        } 

    else
		return GetNumber(tokens, table_array);
    }


Node_t* GetReturn(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN RETURN\n");

    List_type return_token = GET_TOKEN_VALUE;

    PopFront(tokens);

    Node_t* returns_expression = CreateNode(return_token, KEYW_TYPE, nullptr, GetAddSub(tokens, table_array));

    FIND_SYNTAX_ERROR(CheckSequentialOperator(GET_TOKEN_VALUE));

	return returns_expression;
    }


Node_t* GetNumber(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET NUMBER\n");

    if (GET_TOKEN_VALUE.type == NUM_TYPE)
        {
        printf("I INSITE NUMBER\n");

		List_type number = GET_TOKEN_VALUE;

        PopFront(tokens);

        return CreateNode(number, NUM_TYPE, nullptr, nullptr);
        }

	return nullptr;
    }


Node_t* CheckAssign(LIST* tokens, LangNameTableArray* table_array, 
                    List_type var_name, List_type var_type, Node_t* rValue, Node_t* type) 
    {
    if (GET_TOKEN_VALUE.type == KEYW_TYPE && (GET_TOKEN_VALUE.form.key_w == KEYW_ASSIGN))
        {
        List_type assign_token = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* lValue = GetAddSub(tokens, table_array);

		Node_t* assign_node = CreateNode(assign_token, KEYW_TYPE, lValue, rValue);

        Node_t* var_node = CreateNode(var_name, VAR_DECL_TYPE, type, assign_node);

        FIND_SYNTAX_ERROR(CheckSequentialOperator(GET_TOKEN_VALUE));

        return var_node;
        }

    return nullptr;
    }


/*------------------------------------------------CHECK_VALID---------------------------------------------------------*/


Node_t* CheckInitialization(List_type token)
    {
	printf("I CHECK INIT\n");

	if (token.type != KEYW_TYPE || token.form.key_w != KEYW_INIT)
        {
        switch(token.type)
            {
            case ID_TYPE:
                {
                PRINT_SYNTAX_ERROR(token.line, "use of undeclared identifier", token.form.id);
                return nullptr;
                break;
                }

            case KEYW_TYPE:
                {
                PRINT_SYNTAX_ERROR(token.line, "incorrect use of operator", GetKeywordByNumber(token.form.key_w));
                return nullptr;
                break;
                }

            default: return nullptr;
            }
        }

    return NO_ERROR_PTR;
    }


Node_t* CheckOpenRoundBracket(LIST* tokens)
    {
	printf("I CHECK (\n");

	if (GET_TOKEN_VALUE.type != KEYW_TYPE || GET_TOKEN_VALUE.form.key_w != KEYW_OPRND)
        {
		PRINT_SYNTAX_ERROR(GET_TOKEN_VALUE.line,  "missing opening bracket", "(");
        return nullptr;
        }

	else 
        PopFront(tokens);

    return NO_ERROR_PTR;
    }


Node_t* CheckIdName(List_type token)
    {
	printf("I CHECK ID NAME\n");

	if (token.type != ID_TYPE)
        {
		PRINT_SYNTAX_ERROR(token.line, "using a keyword as a variable", GetKeywordByNumber(token.form.key_w));
        return nullptr;
        }

    return NO_ERROR_PTR;
    }


Node_t* CheckCloseRoundBracket(LIST* tokens)
    {
	printf("I CHECK )\n");

	if (GET_TOKEN_VALUE.type != KEYW_TYPE || GET_TOKEN_VALUE.form.key_w != KEYW_CLRND)
        {
		PRINT_SYNTAX_ERROR(GET_TOKEN_VALUE.line, "missing closing bracket", ")");
        return nullptr;
        }

	else 
        PopFront(tokens);

    return NO_ERROR_PTR;
    }


Node_t* CheckOpenFigBracket(LIST* tokens)
    {
	printf("I CHECK {\n");

	if (GET_TOKEN_VALUE.type != KEYW_TYPE || GET_TOKEN_VALUE.form.key_w != KEYW_OPFIG)
		{
        PRINT_SYNTAX_ERROR(GET_TOKEN_VALUE.line - 1, "missing opening bracket", "type");
        return nullptr;
        }

	else 
        PopFront(tokens);

    return NO_ERROR_PTR;
    }


Node_t* CheckCloseFigBracket(LIST* tokens)
    {
	printf("I CHECK }\n");
	
	if (GET_TOKEN_VALUE.type != KEYW_TYPE || GET_TOKEN_VALUE.form.key_w != KEYW_CLFIG)
		{
        PRINT_SYNTAX_ERROR(GET_TOKEN_VALUE.line, "missing closing bracket", "sent");
        return nullptr;
        }

	else 
        PopFront(tokens);

    return NO_ERROR_PTR;
    }


Node_t* CheckSequentialOperator(List_type oper)
    {
    printf("I CHECK SEQ_OPERATOR\n");

    if (oper.type != KEYW_TYPE || oper.form.key_w != KEYW_END)
        {
        PRINT_SYNTAX_ERROR(oper.line - 1, "missing operator", ";");
        return nullptr;
        } 

    return NO_ERROR_PTR;
    }


bool CheckOperators(List_type oper) 
    {
    if (oper.type == KEYW_TYPE && (oper.form.key_w == KEYW_WHILE  || oper.form.key_w == KEYW_IF       || 
                                   oper.form.key_w == KEYW_RETURN || oper.form.key_w == KEYW_INIT     || 
                                   oper.form.key_w == KEYW_PRINTF ||
                                   oper.form.key_w == KEYW_BREAK  || oper.form.key_w == KEYW_CONTINUE))
        {
        return true;
        }

    else if (oper.form.key_w == KEYW_ELSE)
        {
        fprintf(stderr, "line: %zu\n", oper.line);
        fprintf(stderr, "error: using else without if\n");
        }

    else if (oper.form.key_w == KEYW_INPUT)
        {
        PRINT_SYNTAX_ERROR(oper.line, "incorrect using of operator", "input");
        }

    else if (oper.form.key_w == KEYW_CLFIG)
        return false;

    else
        {
        PRINT_SYNTAX_ERROR(oper.line, "improper use of the operator", GetKeywordByNumber(oper.form.key_w));
        }
    
    return false;
    }


/*------------------------------------------------Help_Functions----------------------------------------------------*/


List_type GetFictToken(OperatorCode oper) 
    {
    List_type fict_node = {};
    fict_node.type = KEYW_TYPE;
	fict_node.form.key_w = oper;

    return fict_node;
    }


void BeginningOfInitVar(LangNameTable* table, int init_array[])
    {
    for (size_t i = 0; i < NAME_TABLE_CAPACITY; i++)
        table->Table[i].init = init_array[i];
    }


void GetInitArray(int* init_array, LangNameTableArray* table_array)
    {
    for (size_t i = 0; i < NAME_TABLE_CAPACITY; i++)
        init_array[i] = table_array->Array[table_array->ptr].Table[i].init;
    }


Node_t* GetInitVarInNameTable(LangNameTableArray* table_array, List_type token)
    {
    size_t pos_global = FindInNameTable(&table_array->Array[GLOBAL_TABLE_INDEX], token.form.id);

    if (pos_global != -1)
        {
        PRINT_SYNTAX_ERROR(token.line, "initializing a global variable", token.form.id);
        return nullptr;
        }

    LangNameTable* table = &table_array->Array[table_array->ptr];

    size_t pos = FindInNameTable(table, token.form.id);

    table->Table[pos].init += 1;

    return NO_ERROR_PTR;
    }


/*------------------------------------------------------------------------------------------------------------------*/


#undef GET_TOKEN_VALUE
#undef ADD_TO_GLOBAL_TABLE
#undef PRINT_SYNTAX_ERROR
#undef FIND_SYNTAX_ERROR
#undef CREATE_OPERATION_NODE
#undef PRINT_OR_IDENT