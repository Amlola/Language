#pragma once


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