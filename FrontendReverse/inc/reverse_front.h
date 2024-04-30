#pragma once


#include "../../Tree/tree.h"


enum ReverseErrors 
    {
    NOT_FIND_REVERSE_ERRORS = 0 << 1,
    UNKNOWN_NODE_TYPE       = 1 << 1,
    UNKNOWN_KEYWORD         = 1 << 2
    };


const char* GetKeyword(Node_t* node);

ReverseErrors GetMySyntax(Node_t* node, FILE* file);

ReverseErrors PrintFuncDecl(Node_t* node, FILE* file);

ReverseErrors PrintParameters(Node_t* node, FILE* file);

ReverseErrors PrintVarDecl(Node_t* node, FILE* file);

ReverseErrors PrintNode(Node_t* node, FILE* file);

ReverseErrors PrintReturnsFunc(Node_t* node, FILE* file); 



