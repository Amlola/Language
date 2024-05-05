#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>


const size_t WORD_MAX_LEN = 50;


enum Types
    {
    NUM_TYPE       = 1,
    ID_TYPE        = 2,
    KEYW_TYPE      = 3,
    FUNC_DEF_TYPE  = 4,
    PARAM_TYPE     = 5,
    VAR_DECL_TYPE  = 6,
    CALL_TYPE      = 7
    };


struct Token
    {
    Types type;

    union NodeType
        {
        int key_w;
        double num;
        char id[WORD_MAX_LEN];
        }form;

    size_t line;
    };


const char* const str_types[] =
    {
    "constant", "id_type", "keyw_type", 
    "func_def_type", "param_type", 
    "var_decl_type", "call_type"
    };
