#pragma once


#include "../../ReadingFile/read.h"
#include "../../NameTable/name_table.h"


enum KeywordCode
    {
    #define DEF_KEYW(DEF, CODE, WORD) \
        KEYW_ ## DEF = CODE,
        
        #include "../../keyword"

    #undef DEF_KEYW
    };


enum OperatorCode
    {
    #define DEF_OPER(DEF, CODE, SIGN, LEN) \
        KEYW_ ## DEF = CODE,

    #define DEF_OTHER(DEF, CODE, OTHER)    \
        KEYW_ ## DEF = CODE,

        #include "../../operators"

    #undef DEF_OPER
    #undef DEF_OTHER
    };
    

int IsKeyword (char* word, int *keyword_code);

int GetWord(Text* data, char word[WORD_MAX_LEN], size_t* i);

void GetTokensFromOneString(LIST* tokens, Text* data, size_t* i, LangNameTable* general_table, size_t number_string); 

int GetTokens(Text* data, LIST* tokens, LangNameTable* general_table); 