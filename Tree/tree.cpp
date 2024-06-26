#include "tree.h"


void TreeCtor(Tree* tree)
    {   
    tree->status = TREE_OK;

    tree->root = nullptr;

    tree->size = 0;
    }


Node_t* CreateNode(Token token, Types type, Node_t* left, Node_t* right)
    {
    Node_t* new_node = (Node_t*)calloc(1, sizeof(Node_t));

    if (left)
        left->parent = new_node; 
        
    if (right)
        right->parent = new_node;
    
    new_node->left  = left;
    
    new_node->right = right;

    new_node->kind.type = type;

    if (type == KEYW_TYPE)
        new_node->kind.form.key_w = token.form.key_w;

    else if (type == NUM_TYPE)
        new_node->kind.form.num = token.form.num;

    else if (type == ID_TYPE || type == VAR_DECL_TYPE || type == FUNC_DEF_TYPE)
        strcpy(new_node->kind.form.id, token.form.id); 

    return new_node;    
    }


Type_error TreeDelete(Tree* tree, Node_t* node)  
    {
    if (node == nullptr) 
        {
        return NODE_PTR_IS_NULL;
        }

    if (node->left) 
        {        
        TreeDelete(tree, node->left);
        }

    if (node->right) 
        {
        TreeDelete(tree, node->right);
        }

    node->right  = nullptr;

    node->left   = nullptr;

    node->parent = nullptr;

    free(node);  
 
    tree->size--; 

    return tree->status;  
    }


Type_error TreePrefixPrint(Tree* tree, Node_t* node, FILE* file, LangNameTableArray* table_array)
    {
    char main_name[] = "main";

    size_t main_func = FindInNameTable(&table_array->Array[GENERAL_TABLE_INDEX], main_name);

    fprintf(file, "%zu ", main_func);

    TreePrefixPrintNode(tree, node, file, table_array);

    return tree->status;
    }


Type_error TreePrefixPrintNode(Tree* tree, Node_t* node, FILE* file, LangNameTableArray* table_array)
    {
    if (node == nullptr) 
        fprintf(file, "_ ");
    
    fprintf(file, "( ");

    if (node->kind.type == NUM_TYPE)
        fprintf(file, "%d %lg ", node->kind.type, node->kind.form.num);

    else if (node->kind.type == ID_TYPE || node->kind.type == VAR_DECL_TYPE || node->kind.type == FUNC_DEF_TYPE)
        fprintf(file, "%d %d ", node->kind.type, FindInNameTable(&table_array->Array[GENERAL_TABLE_INDEX], node->kind.form.id));

    else if (node->kind.type == KEYW_TYPE)  
        fprintf(file, "%d %d ", KEYW_TYPE, node->kind.form.key_w);

    else 
        fprintf(file, "%d ", node->kind.type);

    if (node->left) 
        {
        TreePrefixPrintNode(tree, node->left, file, table_array);
        }
    
    else
        fprintf(file, "_ ");

    if (node->right)
        {
        TreePrefixPrintNode(tree, node->right, file, table_array);
        }

    else 
        fprintf(file, "_ ");

    fprintf(file, ") ");

    return tree->status;
    }


Type_error TreeDtor(Tree* tree) 
    {
    if (tree->root == nullptr) 
        {
        tree->status |= TREE_ROOT_IS_NULL;

        return TREE_ROOT_IS_NULL;
        }

    TreeDelete(tree, tree->root);

    tree->root = nullptr;

    free(tree->root);

    tree->size = 0;

    return tree->status;
    }


Type_error CheckTreeLinks(Tree* tree, Node_t* node)
    {    
    if (node->left != nullptr && node->right != nullptr && (node->left->parent != node || node->right->parent != node))
        {
        tree->status |= TREE_LINKING_ERROR;
        return TREE_LINKING_ERROR;
        }

    if (node->left != nullptr)
        CheckTreeLinks(tree, node->left);
    
    if (node->right != nullptr)
        CheckTreeLinks(tree, node->right);

    return TREE_OK;
    }


bool TreeVerify(Tree* tree) 
    {
    if (tree == nullptr) 
        {
        tree->status |= TREE_IS_NULL;
        }

    if (tree->root == nullptr) 
        {
        tree->status |= TREE_ROOT_IS_NULL;
        }

    else 
        {
        CheckTreeLinks(tree, tree->root);
        }

    if (tree->size < 0) 
        {
        tree->status != TREE_SIZE_LESS_THAN_ZERO;
        }

    return tree->status;
    }


/*---------------------------------------------------TREE_DUMP--------------------------------------------------------*/


void NodeDump(Node_t* node, size_t* number_of_node, Child child, const char* color) 
    {
    PrintGraphNode(node, number_of_node, child, color);

    size_t current_number_of_node = *number_of_node;

    if (node->left)
        {
        PrintGraphEdge(current_number_of_node, ++(*number_of_node), L_CHILD, "lime");

        NodeDump(node->left, number_of_node, L_CHILD, "lime");
        }

    if (node->right)
        {
        PrintGraphEdge(current_number_of_node, ++(*number_of_node), R_CHILD, "aqua");

        NodeDump(node->right, number_of_node, R_CHILD, "aqua");
        } 
    }


void PrintGraphEdge(size_t from, size_t to, Child child, const char* color) 
    {
    print("node%zu->node%zu [color = \"%s\"];\n", from, to, color);
    }


void PrintGraphNode(Node_t* node, size_t* number_of_node, Child child, const char* color) 
    {
    int i = 0;

    if (node->kind.type == KEYW_TYPE)
        {
        size_t pos = 0;
        for (size_t j = 0; j < NUMBER_OF_KEYWORD; j++)
            {
            if (node->kind.form.key_w == keyword_array[j].num) 
                {
                pos = j;
                break;
                }
            }

        print("node%zu[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %s | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %s}\"];\n", 
                                              *number_of_node, color, node, keyword_array[pos].name, node->parent, node->left, node->right, str_types[node->kind.type - 1]);
        }

    else if (node->kind.type == NUM_TYPE) 
        {
        print("node%zu[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %lf | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %s}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.num, node->parent, node->left, node->right, str_types[node->kind.type - 1]);  
        }

    else if (node->kind.type == ID_TYPE || node->kind.type == VAR_DECL_TYPE || node->kind.type == FUNC_DEF_TYPE) 
        {
        print("node%zu[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %s | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %s}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.id, node->parent, node->left, node->right, str_types[node->kind.type - 1]);  
        }
    
    else 
        {
        print("node%zu[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %s}\"];\n", 
                                              *number_of_node, color, node, node->parent, node->left, node->right, str_types[node->kind.type - 1]);
        }
    }


void TreeDumpFunction(Tree* tree, Node_t* node, const char* path, const char* signature, unsigned line) 
    {
    logfile = fopen("logfile.html", "a");

    fprintf(logfile, "<pre>\n");

    fprintf(logfile, "<font size = \"+1\">path: %s\n</font>", path);

    fprintf(logfile, "<font size = \"+1\">in function: %s\n</font>", signature);

    fprintf(logfile, "<font size = \"+1\">line: %d\n</font>", line);

    if (tree->status != TREE_OK) 
        {
        for (size_t j = 0; j < NUM_OF_ERROR; j++)
            {
            if ((tree->status & (1 << j)))
                {
                fprintf(logfile, "<font color = \"red\">ERROR: %s\n</font>", ErrorMas[j + 1].NameError);
                }
            }
        }

    else 
        {
        size_t number_of_node = 0;

        graph_file1 = fopen("dotfile1.dot", "wb");

        static int dump_number = 1;

        print("digraph struct {bgcolor=RosyBrown rankdir = HR\n\n\n");

        print("label = \"tree_dump from function %s, Tree/%s:%d\";\n", signature, path, line);

        NodeDump(node, &number_of_node, L_CHILD, "red");

        print("\n\n}");

        fclose(graph_file1);

        char shell_command[MAX_COMMAND_LENGTH] = "";
            sprintf(shell_command, "dot -Tpng dotfile1.dot -o graph%d.png", dump_number);
            system(shell_command);

        fprintf(logfile, "<img  width=\"1100px\" height=\"600px\" src=\"graph_tree%d.png\">", dump_number);

        fprintf(logfile, ".\n");

        fprintf(logfile, ".\n");

        fprintf(logfile, ".\n");
                
        fclose(logfile);

        dump_number++;
        } 
    }


/*--------------------------------------------------Backend/Reverse_Read----------------------------------------------------*/


#define CREATE_NODE CreateNode(tok, tok.type, nullptr, nullptr);


Type_error TreeRead(Tree* tree, Text* data, LangNameTableArray* table_array) 
    {
    tree->main_func = atoi(data->Buf);

    size_t i = 2;

    tree->root = PrefixReadTree(tree, data, &i, table_array);
    
    return tree->status;
    }


Node_t* PrefixReadTree(Tree* tree, Text* data, size_t* i, LangNameTableArray* table_array)
    {
    Node_t* node = nullptr;
    
    if (data->Buf[*i] == '(') 
        {
        List_type tok = {};

        (*i) += SPACE_MAGNIFICATION;

        tok.type = (Types)(GET_NUMBER(*i)); 

        if (tok.type == CALL_TYPE || tok.type == PARAM_TYPE)
            { 
            node = CREATE_NODE;

            (*i) += SPACE_MAGNIFICATION;
            }

        else
            {
            (*i) += SPACE_MAGNIFICATION;

            double value = 0;

            int skip_symbols  = 0;

            sscanf(data->Buf + *i, "%lf%n", &value, &skip_symbols);

            (*i) += skip_symbols + 1;
            
            if (tok.type == NUM_TYPE) 
                {
                tok.form.num = value;

                node = CREATE_NODE;
                }

            else if (tok.type == KEYW_TYPE) 
                {
                tok.form.key_w = value;

                node = CREATE_NODE;
                }

            else if (tok.type == ID_TYPE || tok.type == VAR_DECL_TYPE || tok.type == FUNC_DEF_TYPE) 
                {
                strcpy(tok.form.id, FindInNameTableByCode(&table_array->Array[GENERAL_TABLE_INDEX], value));

                node = CREATE_NODE;
                }
            }
        } 

        node->left  = MakeNodeFromText(tree, data, i, table_array);

        node->right = MakeNodeFromText(tree, data, i, table_array);

        if (data->Buf[*i] != ')')
            {
            printf("PIZDEZ\n");
            TreeDtor(tree);

            return nullptr;
            }

    return node;
    }


Node_t* MakeNodeFromText(Tree* tree, Text* data, size_t* i, LangNameTableArray* table_array)
    {
    Node_t* node = nullptr;

    if (data->Buf[*i] == '(')
        {
        node = PrefixReadTree(tree, data, i, table_array);

        (*i) += SPACE_MAGNIFICATION;

        if (node == nullptr)
            return nullptr;
        }

    else if (data->Buf[*i] != ')')
        {
        if (data->Buf[*i] == '_')
            {
            (*i) += SPACE_MAGNIFICATION;

            return nullptr;
            }
        }

    return node;
    }


#undef CREATE_NODE


const char* GetKeyword(Node_t* node) 
    {
    assert(node);

    for (size_t i = 0; i < NUMBER_OF_KEYWORD; i++) 
        {
        if (node->kind.form.key_w == keyword_array[i].num)
            return keyword_array[i].name;
        }
    
    return nullptr;
    }

const char* GetKeywordByNumber(int number_keyword) 
    {
    for (size_t i = 0; i < NUMBER_OF_KEYWORD; i++) 
        {
        if (number_keyword == keyword_array[i].num)
            return keyword_array[i].name;
        }

    return nullptr;
    }


void ReverseBackDtor(LangNameTableArray* table_array, Tree* tree, 
                 Text* analyz, Text* data_name_table)
    {
    free(analyz->Buf);

    free(data_name_table->Buf);

    NameTableArrayDtor(table_array);

    TreeDtor(tree);
    }
