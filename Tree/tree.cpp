#include "tree.h"


void TreeCtor(Tree* tree)
    {   
    tree->status = TREE_OK;

    tree->root = (Node_t*)calloc(1, sizeof(Node_t));

    tree->root->left = tree->root->right = tree->root->parent = nullptr;

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
        TreePrefixPrint(tree, node->left, file, table_array);
        }
    
    else
        fprintf(file, "_ ");

    if (node->right)
        {
        TreePrefixPrint(tree, node->right, file, table_array);
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
        printf("%d\n", node->kind.form.key_w);

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
        printf("NUM\n");
        print("node%zu[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %lf | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %s}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.num, node->parent, node->left, node->right, str_types[node->kind.type - 1]);  
        }

    else if (node->kind.type == ID_TYPE || node->kind.type == VAR_DECL_TYPE || node->kind.type == FUNC_DEF_TYPE) 
        {
        printf("%s\n", node->kind.form.id);
        print("node%zu[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %s | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %s}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.id, node->parent, node->left, node->right, str_types[node->kind.type - 1]);  
        }
    
    else 
        {
        printf("OTHER\n");
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


#define CREATE_NUM_NODE CreateNode(tok, NUM_TYPE, nullptr, nullptr)
#define CREATE_KEYW_NODE CreateNode(tok, KEYW_TYPE, nullptr, nullptr)


Type_error TreeRead(Tree* tree, Text* data, LangNameTableArray* table_array) 
    {
    size_t i = 0;

    tree->root = PrefixReadTree(tree, data, &i, table_array);
    
    return tree->status;
    }


Node_t* PrefixReadTree(Tree* tree, Text* data, size_t* i, LangNameTableArray* table_array)      // need to be fix
    {
    Node_t* node = nullptr;
    
    if (data->Buf[*i] == '(') 
        {
        List_type tok = {};

        (*i) += SPACE_MAGNIFICATION;

        tok.type = (Types)(GET_NUMBER(*i));      

        if (tok.type == NUM_TYPE) 
            {
            (*i) += SPACE_MAGNIFICATION;

            double value = 0;

            int skip_symbols  = 0;

            sscanf(data->Buf + *i, "%lf%n", &value, &skip_symbols);

            tok.form.num = value;

            node = CREATE_NUM_NODE;

            (*i) += skip_symbols + 1;
            }

        else if (tok.type == KEYW_TYPE) 
            {
            (*i) += SPACE_MAGNIFICATION;

            int value = 0;

            int skip_symbols = 0;

            sscanf(data->Buf + *i, "%d%n", &value, &skip_symbols);

            tok.form.key_w = value;

            node = CREATE_KEYW_NODE;

            (*i) += skip_symbols + 1;
            }

        else if (tok.type == ID_TYPE || tok.type == VAR_DECL_TYPE || tok.type == FUNC_DEF_TYPE) 
            {
            (*i) += SPACE_MAGNIFICATION;

            int value = 0;

            int skip_symbols = 0;

            sscanf(data->Buf + *i, "%d%n", &value, &skip_symbols);

            printf("VAL: %d\n", value);    
        
            strcpy(tok.form.id, FindInNameTableByCode(&table_array->Array[GENERAL_TABLE_INDEX], value));

            (*i) += skip_symbols + 1;

            node = CreateNode(tok, tok.type, nullptr, nullptr);
            }

        else 
            { 
            node = CreateNode(tok, tok.type, nullptr, nullptr);

            (*i) += SPACE_MAGNIFICATION;
            }
        } 

        node->left  = CreateNodeFromBrackets(tree, data, i, table_array);

        node->right = CreateNodeFromBrackets(tree, data, i, table_array);

        if (data->Buf[*i] != ')')
            {
            TreeDtor(tree);

            return nullptr;
            }

    return node;
    }


Node_t* CreateNodeFromBrackets(Tree* tree, Text* data, size_t* i, LangNameTableArray* table_array)
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


#undef CREATE_NUM_NODE
#undef CREATE_KEYW_NODE