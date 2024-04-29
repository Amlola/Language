#include "../inc/Frontend.h"



int main() 
    {    
    Text analyz = {};

    LIST tokens = {};

    FILE* file_input = fopen("../input.txt", "r");

    FileInput(file_input, &analyz);

    LangNameTableArray table_array = {};

    NameTableArrayCtor(&table_array);

    NameTableCtor(&table_array.Array[GENERAL_TABLE_INDEX]);

    ListCtor(&tokens);

    GetTokens(&analyz, &tokens, &table_array.Array[GENERAL_TABLE_INDEX]);

    printf("\n");

    free(analyz.Buf);

    Tree tree = {};

    TreeCtor(&tree);

    tree.root = GetGlobalNameTable(&tokens, &table_array);

    TreeDump(&tree, tree.root);

    FILE* file_output = fopen("../output.txt", "w");

    TreePrefixPrint(&tree, tree.root, file_output, &table_array);

    fclose(file_output);

    FILE* name_table_file = fopen("../name_table.txt", "w");

    WriteNameTableArrayToFile(name_table_file, &table_array);

    NameTableArrayDtor(&table_array);

    TreeDtor(&tree);

    ListDtor(&tokens);

    printf("ALL IS GOOD\n");

    return 0;
    }