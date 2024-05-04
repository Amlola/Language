#pragma once


#include "../../Tree/tree.h"


#define ListGraphDump(list_ptr) ListGraphDumpFunction(list_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__);
static FILE* graph_file_list = NULL;

#define PRINT_TO_GRAPH_FILE(...) fprintf(graph_file_list, __VA_ARGS__);

enum Instruction
    {
    PUSH,
    POP,
    RET,
    CALL,
    ADD,
    SUB,
    MUL,
    DIV,
    LABEL,
    FUNC,
    MOV,
    IN,
    OUT,
    CMP,
    JUMP,
    JNE,
    JE,
    JAE,
    JBE,
    JA,
    JB,
    SQRT,
    };


enum Registers
    {
    NO = -1,
    RAX,
    RCX,
    RDX,
    RBX,
    RSP,
    RBP,
    RSI,
    RDI
    };


struct RegistersArray
    {
    Registers reg;
    const char* name_reg;
    };


const RegistersArray registers_array[] = 
    {
    {NO,    ""},
    {RAX, "rax"}, 
    {RCX, "rcx"},
    {RDX, "rdx"},
    {RBX, "rbx"},
    {RSP, "rsp"},
    {RBP, "rbp"},
    {RSI, "rsi"},
    {RDI, "rdi"},
    };


void ListGraphDumpFunction(LIST* list, const char* path, const char* signature, unsigned line);