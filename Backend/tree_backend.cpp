#include "tree_backend.h"






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



Type_error GetString(Tree* tree, Text* data, char** string, size_t* i)
    {
    *string = data->Buf;

    size_t number =  0;

    while (data->Buf[*i] != ')' && data->Buf[*i] != '(' && data->Buf[*i] != '_')
        {
        (*string)[number++] = data->Buf[*i];

        (*i)++;
        }

    (*string)[number - 1] = '\0';
    
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

    tree->size = 0;

    return tree->status;
    }



Node_t* PrefixReadTree(Tree* tree, Text* data, size_t* i)
    {
    Node_t* node = nullptr;
    
    if (data->Buf[*i] == '(') 
        {
        Token tok = {};

        (*i) += 2;

        tok.type = data->Buf[*i] - 48;

        printf("%d\n", tok.type);

        if (tok.type == NUM_TYPE) 
            {
            (*i) += 2;

            double val = 0;

            int check  = 0;

            sscanf(data->Buf + *i, "%lf%n", &val, &check);

            tok.form.num = val;

            node = CreateNode(tok, NUM_TYPE, nullptr, nullptr);

            printf("NUM: %lg\n", val);

            (*i) += check + 1;
            }

        else if (tok.type == KEYW_TYPE) 
            {
            (*i) += 2;

            int val = 0;

            int check  = 0;

            sscanf(data->Buf + *i, "%d%n", &val, &check);

            tok.form.key_w = val;

            node = CreateNode(tok, KEYW_TYPE, nullptr, nullptr);

            printf("Keyw: %d\n", val);

            (*i) += check + 1;
            }

        else if (tok.type == ID_TYPE || tok.type == VAR_DECL_TYPE || tok.type == FUNC_DEF_TYPE) 
            {
            (*i) += 2;

            char* string = nullptr;

            GetString(tree, data, &string, i);

            printf("%s\n", string);
        
            strcpy(tok.form.id, string);

            node = CreateNode(tok, Types(tok.type), nullptr, nullptr);
            }

        else 
            { 
            node = CreateNode(tok, (Types)tok.type, nullptr, nullptr);

            (*i) += 2;

            printf("PARAM\n");
            }
        } 

        printf("DA: %d\n", *i);

        node->left  = CreateNodeFromBrackets(tree, data, i);

        node->right = CreateNodeFromBrackets(tree, data, i);

        if (data->Buf[*i] != ')')
            {
            TreeDtor(tree);

            return nullptr;
            }

    return node;
    }



Node_t* CreateNodeFromBrackets(Tree* tree, Text* data, size_t* i)
    {
    Node_t* node = nullptr;

    if (data->Buf[*i] == '(')
        {
        printf("TU\n");
        node = PrefixReadTree(tree, data, i);

        (*i) += 2;

        if (node == nullptr)
            {
            return nullptr;
            }
        }

    else if (data->Buf[*i] != ')')
        {
        if (data->Buf[*i] == '_')
            {
            (*i) += 2;

            return nullptr;
            }
        }

    return node;
    }



Type_error TreeRead(Tree* tree, Text* data)
    {
    size_t i = 0;

    tree->root = PrefixReadTree(tree, data, &i);
    
    return tree->status;
    }


int SkipSpaces(Text* data, size_t i) 
    {
    while (isspace(data->Buf[i])) 
        {
        i++;
        }
    
    return i;
    }


long long GetFileSize(FILE* file)
    {
    assert(file);

    long long start = ftell(file);
    fseek(file, start, SEEK_END);

    long long bufsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    return bufsize;
    }



void FileInput(FILE* file, Text* data)
    {
    data->BufSize = GetFileSize(file);

    data->Buf = (char*)calloc(data->BufSize + 1, sizeof(char));

    fread(data->Buf, sizeof(char), data->BufSize, file);

    data->Buf[data->BufSize] = '\0';

    fclose(file);
    }



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
    print("node%d->node%d [color = \"%s\"];\n", from, to, color);
    }


void PrintGraphNode(Node_t* node, size_t* number_of_node, Child child, const char* color) 
    {
    if (node->kind.type == KEYW_TYPE)
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %d | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %d}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.key_w, node->parent, node->left, node->right, KEYW_TYPE);

    else if (node->kind.type == NUM_TYPE) 
        {
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %lf | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %d}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.num, node->parent, node->left, node->right, NUM_TYPE);  
        }

    else if (node->kind.type == ID_TYPE || node->kind.type == VAR_DECL_TYPE || node->kind.type == FUNC_DEF_TYPE) 
        {
        printf("DATA: %s\n", node->kind.form.id);
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | DATA: %s | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %d}\"];\n", 
                                              *number_of_node, color, node, node->kind.form.id, node->parent, node->left, node->right, node->kind.type);  
        }
    
    else 
        {
        print("node%d[shape=record, style=filled, fillcolor=\"%s\", label=\" {ADDRESS: %p | PARENT: %p | LEFT: %p | RIGHT: %p | TYPE: %d}\"];\n", 
                                              *number_of_node, color, node, node->parent, node->left, node->right, node->kind.type);
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