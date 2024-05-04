#include "../inc/reverse_front.h"


#define PRINT_MY_LANG(str) fprintf(file, "%s", str)
#define PRINT_NUMBER(num) fprintf(file, "%lg", num)
#define PRINT_NEW_LINE fprintf(file, "\n")
#define PRINT_SPACE fprintf(file, " ")
#define PRINT_SPECIAL_SYMBOL(symbol) fprintf(file, symbol)

#define CHECK_VALID                             \
        if (errors != NOT_FIND_REVERSE_ERRORS)  \
            return errors;                      \


ReverseErrors GetMySyntax(Node_t* node, FILE* file) 
    {
    assert(file);

    Node_t* func_declaration = node;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    size_t indent = 0; 

    bool flag = true;

    while (func_declaration) 
        {
        errors = PrintDecl(func_declaration->left, file, &indent, &flag);
        CHECK_VALID;

        func_declaration = func_declaration->right;

        PRINT_NEW_LINE;
        }

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintDecl(Node_t* node, FILE* file, size_t* indent, bool* flag)
    {
    assert(file);

    const char* name_type = GetKeyword(node->left);

    if (!name_type)
        return UNKNOWN_KEYWORD;

    PRINT_MY_LANG(name_type);

    PRINT_SPACE;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    if (node->kind.type == VAR_DECL_TYPE)
        {
        errors = PrintTypeNode(node->right, file, indent, flag);
        PRINT_SPECIAL_SYMBOL(";");
        PRINT_NEW_LINE;
        }
        
    else if (node->kind.type == FUNC_DEF_TYPE)
        {
        PRINT_MY_LANG(node->kind.form.id);
        errors = PrintParameters(node->right, file, indent, flag);
        }

    else
        return UNKNOWN_NODE_TYPE;

    CHECK_VALID;

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintParameters(Node_t* node, FILE* file, size_t* indent, bool* flag)
    {
    assert(file);

    PRINT_SPECIAL_SYMBOL("(");

    if (node->kind.type != PARAM_TYPE)
        return UNKNOWN_NODE_TYPE;

    Node_t* comma = node->left;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    while (comma)
        {     
        if (!comma->left)
            break;
                 
        errors = PrintVarDecl(comma->left, file, indent, flag);
        CHECK_VALID;

        comma = comma->right;

        if (!comma)
            break;

        PRINT_SPECIAL_SYMBOL(",");

        PRINT_SPACE;
        }

    PRINT_SPECIAL_SYMBOL(")");

    PRINT_NEW_LINE;

    PRINT_SPECIAL_SYMBOL("type");

    PRINT_NEW_LINE;

    (*indent)++;

    errors = PrintBodyFunc(node->right, file, indent, flag);
    CHECK_VALID;

    (*indent)--;

    PRINT_SPECIAL_SYMBOL("sent");

    PRINT_NEW_LINE;

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintVarDecl(Node_t* node, FILE* file, size_t* indent, bool* flag) 
    {
    assert(file);

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    if (node->kind.type != VAR_DECL_TYPE)
        return UNKNOWN_NODE_TYPE;

    const char* name_type = GetKeyword(node->left);

    if (!name_type)
        return UNKNOWN_KEYWORD;

    PRINT_MY_LANG(name_type);

    PRINT_SPACE;

    errors = PrintTypeNode(node->right, file, indent, flag);
    CHECK_VALID;

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintTypeNode(Node_t* node, FILE* file, size_t* indent, bool* flag)
    {
    assert(file);

    if (!node)
        return NOT_FIND_REVERSE_ERRORS;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

     switch (node->kind.type)
        {
        case NUM_TYPE:
            {
            PRINT_NUMBER(node->kind.form.num);
            break;
            }

        case ID_TYPE:
            {
            PRINT_MY_LANG(node->kind.form.id);
            break;
            }

        case KEYW_TYPE:
            {
            errors = PrintKeyword(node, file, indent, flag);
            break;
            }

        case VAR_DECL_TYPE:
            {
            errors = PrintVarDecl(node, file, indent, flag);
            break;
            }

        case CALL_TYPE:
            {
            errors = PrintFunc(node, file, indent, flag);
            break;
            }

        default: 
            {
            return UNEXPECTED_TYPE;
            break;
            }
        }

    CHECK_VALID;

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintFunc(Node_t* node, FILE* file, size_t* indent, bool* flag) 
    {
    assert(file);

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    if (node->kind.type != CALL_TYPE)
        return UNKNOWN_NODE_TYPE;

    PRINT_MY_LANG(node->right->kind.form.id);

    PRINT_SPECIAL_SYMBOL("(");
    
    errors = PrintArgumentSequence(node->left, file, indent, flag);

    CHECK_VALID;

    PRINT_SPECIAL_SYMBOL(")");

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintArgumentSequence(Node_t* node, FILE* file, size_t* indent, bool* flag)
    {
    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    Node_t* comma = node;

    if (comma)
        {
        if (comma->kind.form.key_w == KEYW_COMMA)
            {
            while (comma->kind.form.key_w == KEYW_COMMA)
                {
                errors = PrintTypeNode(comma->left, file, indent, flag);
                CHECK_VALID;

                comma = comma->right;

                if (!comma)
                    break;

                PRINT_SPECIAL_SYMBOL(",");

                PRINT_SPACE;
                }
            }
            
        else 
            {
            errors = PrintTypeNode(comma, file, indent, flag);
            CHECK_VALID;
            }
        }

	return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintBodyFunc(Node_t* node, FILE* file, size_t* indent, bool* flag)
    {
    assert(file);

    Node_t* sequential_operator = node;

    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    while (sequential_operator)
        {
        PrintIndent(indent, file);

        errors = PrintTypeNode(sequential_operator->left, file, indent, flag);
        CHECK_VALID;

        sequential_operator = sequential_operator->right;

        if (*flag)
            PRINT_SPECIAL_SYMBOL(";");

        PRINT_NEW_LINE;

        *flag = true;
        }

    return NOT_FIND_REVERSE_ERRORS;
    }


ReverseErrors PrintKeyword(Node_t* node, FILE* file, size_t* indent, bool* flag)
    {
    ReverseErrors errors = NOT_FIND_REVERSE_ERRORS;

    switch (node->kind.form.key_w)
        {
        case KEYW_ADD:
        case KEYW_SUB:
        case KEYW_MULT:
        case KEYW_DIV:
        case KEYW_JA:
        case KEYW_JAE:
        case KEYW_JB:
        case KEYW_JBE:
        case KEYW_JNE:
        case KEYW_JE:
        case KEYW_AND:
        case KEYW_OR:
        case KEYW_POW:
            {
            errors = PrintTypeNode(node->left, file, indent, flag);
            CHECK_VALID;

            PRINT_SPACE;

            PRINT_MY_LANG(GetKeyword(node));

            PRINT_SPACE;

            errors = PrintTypeNode(node->right, file, indent, flag);
            CHECK_VALID

            break;
            }

        case KEYW_FLOOR:
        case KEYW_DIFF:
        case KEYW_SIN:
        case KEYW_COS:
        case KEYW_SQRT:
            {
            PRINT_MY_LANG(GetKeyword(node));
            
            PRINT_SPECIAL_SYMBOL("(");

            errors = PrintTypeNode(node->right, file, indent, flag);
            CHECK_VALID;

            PRINT_SPECIAL_SYMBOL(")");

            break;
            }

        case KEYW_INPUT:
            {
            PRINT_MY_LANG(GetKeyword(node));
            PRINT_SPECIAL_SYMBOL("(");
            PRINT_SPECIAL_SYMBOL(")");
            break;
            }

        case KEYW_PRINTF:
        case KEYW_RETURN:
            {
            PRINT_MY_LANG(GetKeyword(node));

            if (node->kind.form.key_w == KEYW_PRINTF)
                PRINT_SPECIAL_SYMBOL("(");

            else
                PRINT_SPACE;

            errors = PrintTypeNode(node->right, file, indent, flag);
            CHECK_VALID;

            if (node->kind.form.key_w == KEYW_PRINTF)
                PRINT_SPECIAL_SYMBOL(")");

            break;
            }

        case KEYW_ASSIGN:
            {
            errors = PrintTypeNode(node->right, file, indent, flag);
            CHECK_VALID;

            PRINT_SPACE;

            PRINT_MY_LANG(GetKeyword(node));

            PRINT_SPACE;

            errors = PrintTypeNode(node->left, file, indent, flag);
            CHECK_VALID;

            break;
            }

        case KEYW_WHILE:
        case KEYW_IF:
            {
            PRINT_MY_LANG(GetKeyword(node));
            PRINT_SPACE;

            PRINT_SPECIAL_SYMBOL("(");
            errors = PrintTypeNode(node->left, file, indent, flag);
            CHECK_VALID;
            PRINT_SPECIAL_SYMBOL(")");

            PRINT_NEW_LINE;

            PrintIndent(indent, file);

            PRINT_SPECIAL_SYMBOL("type");

            (*indent)++;

            PRINT_NEW_LINE;
            errors = PrintBodyFunc(node->right, file, indent, flag);
            CHECK_VALID;

            (*indent)--;

            PrintIndent(indent, file);
                
            PRINT_SPECIAL_SYMBOL("sent");

            *flag = false;

            break;
            }

        case KEYW_BREAK:
        case KEYW_CONTINUE:
            {
            PRINT_MY_LANG(GetKeyword(node));
            break;
            }

        case KEYW_END:
            {
            errors = PrintTypeNode(node->left, file, indent, flag);
            CHECK_VALID;

            PRINT_SPECIAL_SYMBOL(";");

            *flag = false;

            PRINT_NEW_LINE;
            PrintIndent(indent, file);

            errors = PrintTypeNode(node->right, file, indent, flag);
            CHECK_VALID;
            break;
            }

        default:
            {
            return UNEXPECTED_KEYWORD;
            break;
            }
        }

    return NOT_FIND_REVERSE_ERRORS;
    }


void PrintIndent(size_t* indent, FILE* file)
    {
    for (size_t i = 0; i < (*indent); i++)
        fprintf(file, "\t");
    }


#undef PRINT_MY_LANG
#undef PRINT_SPACE
#undef PRINT_NEW_LINE
#undef PRINT_SPECIAL_SYMBOL
#undef CHECK_VALID
#undef PRINT_NUMBER