#include "../inc/Frontend.h"



int main(int argc, char* argv[]) 
    {    
    Text analyz = {};

    LIST tokens = {};

    if (argc < 2) 
        {
        printf("you must specify the input and output file\n");
        return NOT_ALL_ARGUMENTS_ARE_SPECIFIED;
        }

    FILE* file_input = fopen(argv[1], "r");

    CHECK_FILE(file_input, argv[1]);

    FileInput(file_input, &analyz);

    LangNameTableArray table_array = {};

    NameTableArrayCtor(&table_array);

    NameTableCtor(&table_array.Array[GENERAL_TABLE_INDEX]);

    ListCtor(&tokens);

    GetTokens(&analyz, &tokens, &table_array.Array[GENERAL_TABLE_INDEX]);

    free(analyz.Buf);

    if (analyz.count_n == 0)
        {
        NameTableArrayDtor(&table_array);
        ListDtor(&tokens);
        return SYNTAX_ERROR;
        }

    Tree tree = {};

    TreeCtor(&tree);

    tree.root = GetGlobalNameTable(&tokens, &table_array);

    if (!tree.root)
        {
        printf("ALL IS BAD\n");
        FrontDtor(&tree, &tokens, &table_array);
        return SYNTAX_ERROR;
        }

    TreeDump(&tree, tree.root);

    FILE* file_output = fopen(argv[2], "w");

    CHECK_FILE(file_output, argv[2]);

    TreePrefixPrint(&tree, tree.root, file_output, &table_array);

    fclose(file_output);

    FILE* name_table_file = fopen("../name_table.txt", "w");

    CHECK_FILE(name_table_file, "../name_table.txt");

    WriteNameTableArrayToFile(name_table_file, &table_array);

    FrontDtor(&tree, &tokens, &table_array);

    printf("ALL IS GOOD\n");

    return 0;
    }


void FrontDtor(Tree* tree, LIST* list, LangNameTableArray* table_array) 
    {
    NameTableArrayDtor(table_array);

    TreeDtor(tree);

    ListDtor(list);
    }