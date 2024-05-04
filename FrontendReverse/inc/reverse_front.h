#pragma once


#include "../../Tree/tree.h"


enum ReverseErrors 
    {
    NOT_FIND_REVERSE_ERRORS    = 0,
    UNKNOWN_NODE_TYPE          = 1 << 1,
    UNKNOWN_KEYWORD            = 1 << 2,
    UNEXPECTED_TYPE            = 1 << 3,
    UNEXPECTED_KEYWORD         = 1 << 4
    };


ReverseErrors GetMySyntax(Node_t* node, FILE* file);

ReverseErrors PrintDecl(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintParameters(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintVarDecl(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintTypeNode(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintFunc(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintArgumentSequence(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintBodyFunc(Node_t* node, FILE* file, size_t* indent, bool* flag);

ReverseErrors PrintKeyword(Node_t* node, FILE* file, size_t* indent, bool* flag);

void ReverseDtor(LangNameTableArray* table_array, Tree* tree, 
                 Text* analyz, Text* data_name_table);

void PrintIndent(size_t* indent, FILE* file);