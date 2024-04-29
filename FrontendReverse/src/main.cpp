#include "../inc/reverse_front.h"


int main(int argc, char* argv[]) 
    {
    Text analyz = {};

     if (argc < 2) 
        {
        fprintf(stderr, "you must specify the input, name_table and output file\n");
        return NOT_ALL_ARGUMENTS_ARE_SPECIFIED;
        }

    FILE* file_input = fopen(argv[1], "r");

    CHECK_FILE(file_input, argv[1]);

    FileInput(file_input, &analyz);

    FILE* name_table_file = fopen(argv[2], "r");

    CHECK_FILE(name_table_file, argv[2]);

    Text data_name_table = {};

    FileInput(name_table_file, &data_name_table);

    LangNameTableArray table_array = {};

    NameTableArrayCtor(&table_array);

    GetNameTableArray(&table_array, &data_name_table);

    Tree tree = {};

    TreeCtor(&tree);

    TreeRead(&tree, &analyz, &table_array);

    TreeDump(&tree, tree.root);

    free(analyz.Buf);

    NameTableArrayDtor(&table_array);

    printf("ALL IS GOOD\n");

    return 0;
    }