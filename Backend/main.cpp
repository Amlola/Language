#include "Asm.h"



int main() 
    {    
    Text data = {};

    Tree tree = {};

    TreeCtor(&tree);

    FILE* file_input = fopen("../Front/output.txt", "r");

    FileInput(file_input, &data);

    printf("ZERO_ELEM: %c\n", data.Buf[0]);

    TreeRead(&tree, &data);

    printf("YES\n");

    free(data.Buf);

    TreeDump(&tree, tree.root);

    Asm asmb = {};

    AsmInit(&asmb);

    FILE* asm_input = fopen("asm.txt", "w");

    GetAsmCode(&asmb, asm_input, tree.root);

    printf("ALL IS GOOD\n");

    return 0;
    }