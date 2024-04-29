#include "Asm.h"



void AsmInit(Asm* asmb) 
    {
    asmb->names_table = (char**)calloc(MAX_NAMES, sizeof(char*));

    asmb->names_num = 0;

    asmb->label_num = 0;
    }



void GetAsmCode(Asm* asmb, FILE* file, Node_t* node)
    {
    fprintf(file, "call main \nhlt \n\n"); 

    CreateAsm(asmb, node, file);

    fclose(file);

    free(asmb->names_table);
    }


void CreateAsm(Asm* asmb, Node_t* node, FILE* file) 
    {
    if (!node)
        return;

    if (node->kind.type == FUNC_DEF_TYPE) 
        {
        free(asmb->names_table);

        asmb->names_table = (char**)calloc(MAX_NAMES, sizeof(char*));

        asmb->names_num = 0;

        printf("%s\n", node->kind.form.id);

        fprintf(file, "%s:\n", node->kind.form.id);

        CreateAsm(asmb, node->right, file);
        }


    else if (node->kind.type == PARAM_TYPE) 
        {
        CreateAsm(asmb, node->left, file);

        CreateAsm(asmb, node->right, file);
        }


    else if (node->kind.type == VAR_DECL_TYPE) 
        {
        printf("TUT\n");
        asmb->names_table[asmb->names_num] = node->kind.form.id;

        asmb->names_num++;

        CreateAsm(asmb, node->right, file);
        }

    
    else if (node->kind.type == ID_TYPE) 
        {
        PushVar(file, node->kind.form.id, asmb);
        }

    
    else if (node->kind.type == CALL_TYPE) 
        {
        fprintf(file, "push rcx\n");

        PushParams(file, node->left, asmb);

        fprintf(file, "push rax\n");

        fprintf(file, "push %d\n", asmb->names_num);

        fprintf(file, "add\n");

        fprintf(file, "pop rax\n");

        fprintf(file, "call %s\n", node->right->kind.form.id);

        fprintf(file, "pop rcx\n");

        fprintf(file, "push rax\n");

        fprintf(file, "push %d\n", asmb->names_num);

        fprintf(file, "sub\n");

        fprintf(file, "pop rax\n");

        }


    else if (node->kind.type == NUM_TYPE)
        fprintf(file, "push %lf\n", node->kind.form.num);


    else if (node->kind.type == KEYW_TYPE) 
        {
        switch (node->kind.form.key_w)
            {
            case KEYW_END:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                break;

            
            case KEYW_COMMA:

                CreateAsm(asmb, node->left, file); 

                break;

            
            case KEYW_ADD:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "add\n");

                break;


            case KEYW_SUB:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "sub\n");

                break;


            case KEYW_MULT:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "mult\n");

                break;


            case KEYW_DIV:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "div\n");

                break;


            case KEYW_SIN:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "sin\n");

                break;


            case KEYW_COS:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "cos\n");

                break;


            case KEYW_SQRT:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "sqrt\n");

                break;

            
            case KEYW_ASSIGN:

                printf("ASSIGN\n");
                CreateAsm(asmb, node->right, file);

                if (node->right->kind.type == CALL_TYPE)
                    fprintf (file, "push rbx\n");

                if (node->left->kind.type == ID_TYPE) 
                    {
                    fprintf(file, "push %d\n", GetVarPosition(node->left->kind.form.id, asmb->names_table, asmb->names_num));

                    fprintf(file, "pop rdx\n");

                    fprintf(file, "pop [rdx]\n");
                    }

                break;


            case KEYW_JE:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "je label%d\n", asmb->label_num);

                break;


            case KEYW_JNE:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "jne label%d\n", asmb->label_num);

                break;


            case KEYW_JBE:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "jbe label%d\n", asmb->label_num);

                break;


            case KEYW_JB:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "jb label%d\n", asmb->label_num);

                break;


            case KEYW_JA:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "ja label%d\n", asmb->label_num);

                break;


            case KEYW_JAE:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "jae label%d\n", asmb->label_num);

                break;

            
            case KEYW_IF:

                CreateAsm(asmb, node->left, file); 

                CreateAsm(asmb, node->right, file);

                fprintf(file, "jmp label%d\n" "label%d:\n", asmb->label_num, asmb->label_num);

                asmb->label_num++;

                break;


            case KEYW_RETURN:

                CreateAsm(asmb, node->right, file);

                fprintf (file, "pop rcx\n" "ret\n");

                break;

            
            case KEYW_PRINTF:

                if (node->right->kind.type == NUM_TYPE)
                    {
                    fprintf(file, "push %lf\nout\n", node->right->kind.form.num);
                            break;
                    }
                
                PushVar(file, node->right->kind.form.id, asmb);

                fprintf(file, "out\n");
                
                break;

                

            
            case KEYW_WHILE:

                CreateAsm(asmb, node->left, file); 

                break;



            case KEYW_ELSE:
                {
                CreateAsm(asmb, node->left, file);

                int elslbl = 0;

                elslbl = asmb->label_num;

                asmb->label_num += 1;

                CreateAsm(asmb, node->right->left, file);

                fprintf(file, "jmp label%d \n", asmb->label_num);

                int aftlbl = asmb->label_num;

                asmb->label_num++;

                fprintf(file, "label%d: \n", elslbl);

                CreateAsm(asmb, node->right->right, file);

                fprintf(file, "jmp label%d \n" "label%d: \n", aftlbl, aftlbl);

                break;
                }

            
            default:
                break;
            }
        }

    
    }



static void PushParams(FILE* file, Node_t* node, Asm* asmb)
    {
    if (node->left)
         PushParams(file, node->left, asmb);

    CreateAsm(asmb, node->right, file);
    }


static int GetVarPosition(const char* var, char** names_array, int n_names)
    {
    for (int i = 0; i < n_names; i++)
        {
        if (strcmp(var, names_array[i]) == 0) 
            return i;
        }

    return -1;
    }


static void PushVar (FILE* file, const char* var, Asm* asmb)
    {
    fprintf(file, "push %d\n", GetVarPosition(var, asmb->names_table, asmb->names_num));

    fprintf(file, "pop rdx\n");

    fprintf(file, "push [rdx]\n");
    }