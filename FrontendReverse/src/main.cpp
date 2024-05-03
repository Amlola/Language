#include "../inc/reverse_front.h"


int main(int argc, char* argv[]) 
    {
     if (argc < 2) 
        {
        fprintf(stderr, "you must specify the input, name_table and output file\n");
        return NOT_ALL_ARGUMENTS_ARE_SPECIFIED;
        }

    Text analyz = {};

    FILE* file_input = fopen(argv[1], "r");

    CHECK_FILE(file_input, argv[1]);

    FileInput(file_input, &analyz);

/*--------------------------------------------------------------*/

    FILE* name_table_file = fopen(argv[2], "r");

    CHECK_FILE(name_table_file, argv[2]);

    Text data_name_table = {};

    FileInput(name_table_file, &data_name_table);

    LangNameTableArray table_array = {};

    NameTableArrayCtor(&table_array);

    GetNameTableArray(&table_array, &data_name_table);

/*--------------------------------------------------------------*/

    /*Tree tree = {};

    TreeCtor(&tree);

    TreeRead(&tree, &analyz, &table_array);

    FILE* lang_file = fopen("../lang_file.txt", "w");

    CHECK_FILE(lang_file, "../lang_file.txt");

    GetMySyntax(tree.root, lang_file);

    fclose(lang_file);

    TreeDump(&tree, tree.root);*/

/*---------------------------------------------------------------*/

    free(analyz.Buf);

    free(data_name_table.Buf);

    NameTableArrayDtor(&table_array);

    //TreeDtor(&tree);

    printf("ALL IS GOOD\n");

    return 0;
    }