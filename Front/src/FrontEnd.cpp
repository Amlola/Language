#include "../inc/Frontend.h"


#define GET_TOKEN_VALUE tokens->data[Begin(tokens)].value
#define ADD_TO_GLOBAL_TABLE AddToNameTableIfNotFind(&table_array->Array[GLOBAL_TABLE_INDEX], name_func.form.id, FindInNameTable(&table_array->Array[GENERAL_TABLE_INDEX], name_func.form.id), FUNC_NAME)


Node_t* GetGlobalNameTable(LIST* tokens, LangNameTableArray* table_array) 
	{
	NameTableCtor(&(table_array->Array[GLOBAL_TABLE_INDEX]));

	table_array->Array[GLOBAL_TABLE_INDEX].table_number = GLOBAL_TABLE_ID;

    table_array->ptr = 1;

	return GetGrammar(tokens, table_array);
	}


Node_t* GetGrammar(LIST* tokens, LangNameTableArray* table_array)
    {
    Node_t* func = GetDefFunc(tokens, table_array);

    if (GET_TOKEN_VALUE.form.key_w == KEYW_ENDOF) 
        return func;
    
    else 
        {
		List_type fict_token = GetFictToken();

        Node_t* fict_node = CreateNode(fict_token, KEYW_TYPE, GetGrammar(tokens, table_array), nullptr);

        func = CreateNode(fict_token, KEYW_TYPE, func, fict_node);
        }

	return func;
    }


Node_t* GetDefFunc(LIST* tokens, LangNameTableArray* table_array) 
    {
	printf("I AM IN DEF FUNC\n");

    CheckValidFunc(tokens);

	List_type type = GET_TOKEN_VALUE;

	PopFront(tokens);

    List_type name_func = GET_TOKEN_VALUE;

    ADD_TO_GLOBAL_TABLE;

    CheckTableArraySize(table_array);

    NameTableCtor(&(table_array->Array[table_array->ptr]));

    PopFront(tokens);

/*--------------------GetParameters---------------------*/

    CheckOpenRoundBracket(tokens);

    Node_t* left = GetParamSequence(tokens, table_array);

    CheckCloseRoundBracket(tokens);

/*---------------------GetFuncBody----------------------*/

    CheckOpenFigBracket(tokens);

    Node_t* right = GetOperatorsSequence(tokens, table_array);

	CheckCloseFigBracket(tokens);

/*------------------------------------------------------*/

	Node_t* type_func = CreateNode(type, KEYW_TYPE, nullptr, nullptr);

	Node_t* function  = CreateNode(GET_TOKEN_VALUE, PARAM_TYPE, left, right);

	return CreateNode(name_func, FUNC_DEF_TYPE, type_func, function);
    }


Node_t* GetOperatorsSequence(LIST* tokens, LangNameTableArray* table_array)
    {   
	printf("I AM IN OP Sequence\n");	

	Node_t* first_oper = GetOperator(tokens, table_array);
    
	Node_t* current_oper = first_oper;

    while (GET_TOKEN_VALUE.form.key_w == KEYW_END)
        {		
		PopFront(tokens);

		Node_t* second_oper = GetOperator(tokens, table_array);

        current_oper->right = second_oper;   

		current_oper = second_oper;
        }

    return first_oper;
    }


Node_t* GetOperator(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET OPERATOR\n");

	List_type oper = GET_TOKEN_VALUE;

	List_type fict_token = GetFictToken();

	if (oper.form.key_w == KEYW_WHILE  || oper.form.key_w == KEYW_IF    || 
        oper.form.key_w == KEYW_RETURN || oper.form.key_w == KEYW_INIT  || 
        oper.form.key_w == KEYW_PRINTF || oper.form.key_w == KEYW_INPUT ||
        oper.type       == ID_TYPE)

	    {
		if (oper.form.key_w == KEYW_WHILE)
		    {
			Node_t* oper_node = GetWhile(tokens, table_array);

			return CreateNode(fict_token, KEYW_TYPE, oper_node, nullptr);
		    }	

		else if (oper.form.key_w == KEYW_IF)
		    {
			Node_t* oper_node = GetIf(tokens, table_array);

			return CreateNode(fict_token, KEYW_TYPE, oper_node, nullptr);
		    }	

		else if (oper.form.key_w == KEYW_RETURN)
		    {
            Node_t* oper_node = GetReturn(tokens, table_array);
            
			return CreateNode(fict_token, KEYW_TYPE, oper_node, nullptr);
		    }

		else if (oper.form.key_w == KEYW_INIT)
		    {
            Node_t* oper_node = GetInit(tokens, table_array);

			return CreateNode(fict_token, KEYW_TYPE, oper_node, nullptr);
		    }

		else 
		    {
            Node_t* oper_node = GetNotInitToken(tokens, table_array);

			return CreateNode(fict_token, KEYW_TYPE, oper_node, nullptr);
		    }

	    }

	return nullptr;
    }


Node_t* GetInit(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET_INIT\n");
	
    List_type type_var = GET_TOKEN_VALUE;

	Node_t* type = CreateNode(type_var, KEYW_TYPE, nullptr, nullptr);

	PopFront(tokens);

	CheckIdName(tokens);

	List_type var_name = GET_TOKEN_VALUE;

	Node_t* rValue = GetVar(tokens, table_array);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_ASSIGN)
	    {
        List_type assign_token = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* lValue = GetAddSub(tokens, table_array);

		Node_t* assign_node = CreateNode(assign_token, KEYW_TYPE, lValue, rValue);

		return CreateNode(var_name, VAR_DECL_TYPE, type, assign_node);
	    }

	else
		return CreateNode(var_name, VAR_DECL_TYPE, nullptr, rValue);
    }


Node_t* GetNotInitToken(LIST* tokens, LangNameTableArray* table_array)
    {
    printf("I AM IN GET NOT INIT\n");

	List_type func = GET_TOKEN_VALUE;

	PopFront(tokens);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_OPRND)
		return GetFunc(tokens, func, table_array);

	else                                                            // if not call_func, var = ...;
	    {
		PushFront(tokens, func);

		return GetAssign(tokens, table_array);
	    }
    }


Node_t* GetBaseFunc(LIST* tokens, LangNameTableArray* table_array)  // unary and input()
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


Node_t* GetFunc(LIST* tokens, List_type func, LangNameTableArray* table_array)  // functions and printf()
    {
	printf("I AM IN GET_FUNC\n");

	PopFront(tokens);

	Node_t* name_func = CreateNode(func, ID_TYPE, nullptr, nullptr);

	Node_t* arg_branch = GetArgumentSequence(tokens, table_array);

	CheckCloseRoundBracket(tokens);

	if (func.type == KEYW_TYPE)
		return CreateNode(func, KEYW_TYPE, nullptr, arg_branch);

	else if (func.type == ID_TYPE)
		return CreateNode(func, CALL_TYPE, arg_branch, name_func);

	else
		fprintf(stderr, "Error: bad arguments in function\n");
    
    return nullptr;
    }


Node_t* GetArgumentSequence(LIST* tokens, LangNameTableArray* table_array)
	{
	printf("I AM IN GET_ARG_SEQUENCE\n");

	Node_t* first_arg = GetAddSub(tokens, table_array);

	while (GET_TOKEN_VALUE.form.key_w == KEYW_COMMA)
		{
		List_type fict_token = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* second_arg = CreateNode(fict_token, KEYW_TYPE, nullptr, GetAddSub(tokens, table_array));

		first_arg = CreateNode(fict_token, KEYW_TYPE, second_arg, first_arg);
		}

	return first_arg;
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

		CheckCloseRoundBracket(tokens);

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

		CheckCloseRoundBracket(tokens);

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
		fprintf(stderr, "Unknown operator\n");

    return nullptr;
    }


Node_t* GetWhile(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN WHILE\n");

	PopFront(tokens);

	CheckOpenRoundBracket(tokens);

	Node_t* state_while = GetOr(tokens, table_array);

	CheckCloseRoundBracket(tokens);

	CheckOpenFigBracket(tokens);

	Node_t* body_while = GetOperatorsSequence(tokens, table_array);

	CheckCloseFigBracket(tokens);

	List_type while_branch = {};

	while_branch.type = KEYW_TYPE;

	while_branch.form.key_w = KEYW_WHILE;

    List_type fict_token = GetFictToken();

    PushFront(tokens, fict_token);

	return CreateNode(while_branch, KEYW_TYPE, state_while, body_while);
    }


Node_t* GetIf(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN IF\n");

	PopFront(tokens);

	CheckOpenRoundBracket(tokens);

	Node_t* state_if = GetOr(tokens, table_array);

	CheckCloseRoundBracket(tokens);

	Node_t* body_if = GetElse(tokens, table_array);

	List_type if_type = {};

	if_type.type = KEYW_TYPE;                                       // maybe fix

	if_type.form.key_w = KEYW_IF;

	return CreateNode(if_type, KEYW_TYPE, state_if, body_if);
    }


Node_t* GetElse(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN ELSE\n");

	CheckOpenFigBracket(tokens);

	Node_t* if_branch = GetOperatorsSequence(tokens, table_array);

	CheckCloseFigBracket(tokens);

	if (GET_TOKEN_VALUE.form.key_w == KEYW_ELSE)
	    {
		PopFront(tokens);

		Node_t* else_branch = nullptr;

		List_type branch = {};

		branch.type = KEYW_TYPE;

		if (GET_TOKEN_VALUE.form.key_w != KEYW_IF)
		    {
			CheckOpenFigBracket(tokens);

			else_branch = GetOperatorsSequence(tokens, table_array);

			branch.form.key_w = KEYW_ELSE;

			CheckCloseFigBracket(tokens);

            List_type fict_token = GetFictToken();

            PushFront(tokens, fict_token);
		    }

		else
		    {
			branch.form.key_w = KEYW_IF;

			else_branch = GetIf(tokens, table_array);
		    }

		return CreateNode(branch, KEYW_TYPE, if_branch, else_branch);
	    }

    else 
        {
        List_type fict_token = GetFictToken();

        PushFront(tokens, fict_token);
        }

	return if_branch;
    }
  

Node_t* GetParamSequence(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN PARAM SEQ\n");

	List_type fict_token = {};

	fict_token.type = KEYW_TYPE;

	fict_token.form.key_w = KEYW_COMMA;

	Node_t* first_parametr = GetParam(tokens, table_array);

	Node_t* comma = CreateNode(fict_token, KEYW_TYPE, first_parametr, nullptr);

	while (GET_TOKEN_VALUE.form.key_w == KEYW_COMMA)
	    {
        List_type token = GET_TOKEN_VALUE;

		PopFront(tokens);

		Node_t* second_parametr = GetParam(tokens, table_array);

		comma = CreateNode(token, KEYW_TYPE, second_parametr, comma);
	    }

	return comma;
    }


Node_t* GetParam(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GetParam\n");

	if ((GET_TOKEN_VALUE.type == KEYW_TYPE) && (GET_TOKEN_VALUE.form.key_w == KEYW_INIT))
	    {
		List_type fict_token = GET_TOKEN_VALUE;

		Node_t* type_var = CreateNode(fict_token, KEYW_TYPE, nullptr, nullptr);

		PopFront(tokens);

		List_type name_var = GET_TOKEN_VALUE;

		return CreateNode(name_var, VAR_DECL_TYPE, type_var, GetVar(tokens, table_array));
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

    List_type fict_token = GET_TOKEN_VALUE;

    PopFront(tokens);

	return CreateNode(fict_token, KEYW_TYPE, nullptr, GetAddSub(tokens, table_array));
    }


Node_t* GetNumber(LIST* tokens, LangNameTableArray* table_array)
    {
	printf("I AM IN GET NUMBER\n");

    if (GET_TOKEN_VALUE.type == NUM_TYPE)
        {
		List_type number = GET_TOKEN_VALUE;

        PopFront(tokens);

        return CreateNode(number, NUM_TYPE, nullptr, nullptr);
        }

	return nullptr;
    }


/*------------------------------------------------CHECK_VALID---------------------------------------------------------*/


void CheckValidFunc(LIST* tokens)
    {
	printf("I CHECK VALID FUNC\n");

	if (GET_TOKEN_VALUE.type != KEYW_TYPE || GET_TOKEN_VALUE.form.key_w != KEYW_INIT)
		fprintf(stderr, "Functions not found\n");
    }


void CheckOpenRoundBracket(LIST* tokens)
    {
	printf("I CHECK (\n");

	if (GET_TOKEN_VALUE.form.key_w != KEYW_OPRND)
		fprintf(stderr, "Missing opening bracket '('\n");

	else 
        PopFront(tokens);
    }


void CheckIdName(LIST* tokens)
    {
	printf("I CHECK WORD OR NO\n");

	if (GET_TOKEN_VALUE.type != ID_TYPE)
		fprintf(stderr, "It's must be word\n");
    }


void CheckCloseRoundBracket(LIST* tokens)
    {
	printf("I CHECK )\n");

	if (GET_TOKEN_VALUE.form.key_w != KEYW_CLRND)
		fprintf(stderr, "Missing closing bracket ')'\n");

	else 
        PopFront(tokens);
    }


void CheckOpenFigBracket(LIST* tokens)
    {
	printf("I CHECK {\n");

	if (GET_TOKEN_VALUE.form.key_w != KEYW_OPFIG)
		fprintf(stderr, "Missing opening bracket '('\n");

	else 
        PopFront(tokens);
    }


void CheckCloseFigBracket(LIST* tokens)
    {
	printf("I CHECK }\n");
	
	if (GET_TOKEN_VALUE.form.key_w != KEYW_CLFIG)
		fprintf(stderr, "Missing closing bracket 'sent'\n");

	else 
        PopFront(tokens);
    }


#undef GET_TOKEN_VALUE
#undef ADD_TO_GLOBAL_TABLE


/*------------------------------------------------Help_Functions----------------------------------------------------*/


void CheckTableArraySize(LangNameTableArray* table_array) 
    {
    table_array->ptr++;

    if (table_array->ptr >= table_array->capacity)
        NameTableArrayRealloc(table_array);
    }


List_type GetFictToken() 
    {
    List_type fict_node = {};

    fict_node.type = KEYW_TYPE;

	fict_node.form.key_w = KEYW_END;

    return fict_node;
    }