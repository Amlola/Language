#include "tree_backend.h"



enum KeywordCode
    {
    #define DEF_KEYW(DEF, CODE, WORD) \
        KEYW_ ## DEF = CODE,
        
        #include "../keyword"

    #undef DEF_KEYW
    };


enum OperatorCode
    {
    #define DEF_OPER(DEF, CODE, SIGN, LEN) \
        KEYW_ ## DEF = CODE,

    #define DEF_OTHER(DEF, CODE, OTHER)    \
        KEYW_ ## DEF = CODE,

        #include "../operators"

    #undef DEF_OPER
    #undef DEF_OTHER
    };


const int MAX_NAMES = 100;

struct Asm
    {
    char** names_table;
    int names_num;
    int label_num;
    };



void AsmInit(Asm* asmb);

void GetAsmCode(Asm* asmb, FILE* file, Node_t* node);

void CreateAsm(Asm* asmb, Node_t* node, FILE* file);

static void PushParams(FILE* file, Node_t* node, Asm* asmb);

static int GetVarPosition (const char* var, char** names_array, int n_names);

static void PushVar (FILE* file, const char* var, Asm* asmb);





