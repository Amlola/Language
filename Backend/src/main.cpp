#include "../inc/ir.h"



int main(int argc, char* argv[]) 
    {    
     if (argc < 2) 
        {
        printf("you must specify the input, name_table\n");
        return NOT_ALL_ARGUMENTS_ARE_SPECIFIED;
        }

    Text tree_analyz = {};

    FILE* file_input = fopen(argv[1], "r");

    CHECK_FILE(file_input, argv[1]);

    FileInput(file_input, &tree_analyz);

/*--------------------------------------------------------------*/

    FILE* name_table_file = fopen(argv[2], "r");

    CHECK_FILE(name_table_file, argv[2]);

    Text data_name_table = {};

    FileInput(name_table_file, &data_name_table);

    LangNameTableArray table_array = {};

    NameTableArrayCtor(&table_array);

    GetNameTableArray(&table_array, &data_name_table);

    printf("\n\n");

/*--------------------------------------------------------------*/

    Tree tree = {};

    TreeCtor(&tree);

    TreeRead(&tree, &tree_analyz, &table_array);

    TreeDump(&tree, tree.root);

/*--------------------------------------------------------------*/

    LIST instruction_list = {};

    ListCtor(&instruction_list);

    //GetIr(&instruction list, tree.root, &table_array);

    ReverseBackDtor(&table_array, &tree, &tree_analyz, &data_name_table);

    ListDtor(&instruction_list);

    printf("ALL IS GOOD\n");

    return 0;
    }