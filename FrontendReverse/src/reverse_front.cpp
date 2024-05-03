#include "../inc/reverse_front.h"


#define PRINT_MY_LANG(str) fprintf(file, "%s", str)
#define PRINT_NEW_LINE fprintf(file, "\n")
#define PRINT_SPACE fprintf(file, " ")
#define PRINT_SPECIAL_SYMBOL(symbol) fprintf(file, symbol)

#define CHECK_VALID                             \
        if (errors != NOT_FIND_REVERSE_ERRORS)  \
            return errors;                      \


ReverseErrors GetMySyntax(Node_t* node, FILE* file) 
    {
    assert(node);
    assert(file);

    Node_t* func_declaration = node;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS; 

    while (func_declaration != nullptr) 
        {
        errors = PrintFuncDecl(func_declaration->left, file);
       
        CHECK_VALID;

        func_declaration = node->right;
        }

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintFuncDecl(Node_t* node, FILE* file)
    {
    assert(node);
    assert(file);

    if (node->kind.type != FUNC_DEF_TYPE)
        return UNKNOWN_NODE_TYPE;

    const char* name_type = GetKeyword(node->left);

    if (!name_type)
        return UNKNOWN_KEYWORD;

    PRINT_MY_LANG(name_type);

    PRINT_SPACE;

    PRINT_MY_LANG(node->kind.form.id);

    PRINT_SPECIAL_SYMBOL("(");

    ReverseErrors errors = PrintParameters(node->right, file);

    CHECK_VALID;

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintParameters(Node_t* node, FILE* file)
    {
    assert(node);
    assert(file);

    if (node->kind.type != PARAM_TYPE)
        return UNKNOWN_NODE_TYPE;

    Node_t* param_node = node->left;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    while (param_node != nullptr)
        {
        errors = PrintVarDecl(param_node->left, file);

        CHECK_VALID;

        param_node = param_node->right;

        if (!param_node)
            break;

        PRINT_SPECIAL_SYMBOL(",");

        PRINT_SPACE;
        }

    PRINT_SPECIAL_SYMBOL(")");

    PRINT_NEW_LINE;

    PRINT_SPECIAL_SYMBOL("{");

    PRINT_NEW_LINE;

    fprintf(file, "\t");

    PrintNode(node->right, file);

    PRINT_NEW_LINE;

    PRINT_SPECIAL_SYMBOL("}");

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintVarDecl(Node_t* node, FILE* file) 
    {
    assert(node);
    assert(file);

    if (node->kind.type != VAR_DECL_TYPE)
        return UNKNOWN_NODE_TYPE;

    const char* name_type = GetKeyword(node->left);

    if (!name_type)
        return UNKNOWN_KEYWORD;

    PRINT_MY_LANG(name_type);

    PRINT_SPACE;

    if (node->right->kind.type == KEYW_TYPE)
        return NOT_FIND_REVERSE_ERRORS;

    else 
        PRINT_MY_LANG(node->right->kind.form.id);

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintNode(Node_t* node, FILE* file)
    {
    assert(node);
    assert(file);

    Node_t* body_func = node;

    printf("TYPE: %d\n", body_func->kind.type);

    while (body_func != nullptr)
        {
        switch(body_func->kind.type)
            {
            case KEYW_TYPE:
                {
                if (body_func->left->kind.form.key_w == KEYW_RETURN ||
                    body_func->left->kind.form.key_w == KEYW_PRINTF)
                    {
                    PrintReturnsFunc(body_func->left, file);
                    }
                }

            default: return NOT_FIND_REVERSE_ERRORS;
            }

        body_func = body_func->right;
        }

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintReturnsFunc(Node_t* node, FILE* file) 
    {
    assert(node);
    assert(file);

    const char* name_func = GetKeyword(node);

    printf("NAME: %s\n", name_func);

    if (!name_func)
        return UNKNOWN_KEYWORD;

    PRINT_MY_LANG(name_func);

    if (node->kind.form.key_w == KEYW_PRINTF) 
        PRINT_SPECIAL_SYMBOL("(");

    Node_t* node_return_value = node->right;
    
    switch (node_return_value->kind.type)
        {
        case ID_TYPE:
            {
            PRINT_SPACE;
            PRINT_MY_LANG(node_return_value->kind.form.id);
            }
        }

    if (node->kind.form.key_w == KEYW_PRINTF) 
        PRINT_SPECIAL_SYMBOL(")");

    PRINT_SPECIAL_SYMBOL(";");

    return NOT_FIND_REVERSE_ERRORS;
    }


/*ReverseErrors PrintAssign(Node_t* node, FILE* file)
    {
    assert(node);
    assert(file);


    }*/


#undef PRINT_MY_LANG
#undef PRINT_SPACE
#undef PRINT_NEW_LINE
#undef PRINT_SPECIAL_SYMBOL
#undef CHECK_VALID