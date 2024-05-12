#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>


const size_t MAX_LEN_COMMENT        = 100;
const size_t MAX_COMMANDS_IN_FUNC   = 1000;
const size_t MAX_COMMANDS           = 10000;
const size_t MAX_LABELS_NUM         = 100;
const size_t MAX_GLOBALS            = 100;
const size_t MAX_NAME_GLOB_VARS_LEN = 50;


#define INDEX_LIST

#ifdef INDEX_LIST
    typedef int iterator_t;
#endif


enum Instruction
    {
    IR_PUSH,
    IR_POP,
    IR_RET,
    IR_CALL,
    IR_ADD,
    IR_SUB,
    IR_MULT,
    IR_DIV,
    IR_LABEL,
    IR_FUNC,
    IR_MOV,
    IR_IN,
    IR_OUT,
    IR_CMP,
    IR_JMP,
    IR_JNE,
    IR_JE,
    IR_JAE,
    IR_JBE,
    IR_JA,
    IR_JB,
    IR_SQRT,
    };


enum Registers
    {
    IR_NO_REG = -1,
    IR_RAX    = 0,
    IR_RBX    = 1,
    IR_RCX    = 2,
    IR_RDX    = 3,
    IR_RSP    = 4,
    IR_RBP    = 5,
    IR_RSI    = 6,
    IR_RDI    = 7,
    IR_R8     = 8,
    IR_R9     = 9,
    IR_R10    = 10,
    IR_R11    = 11,
    IR_R12    = 12,
    IR_R13    = 13,
    IR_R14    = 14,
    IR_R15    = 15
    };


const char* const register_array[] = {"rax", "rbx", "rcx", "rdx", "rsp", "rbp", "rsi", "rdi", 
                                      "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};


enum ArgumentType 
    {
    IR_NO_ARG,             
    IR_REG,
    IR_MEM,                
    IR_MEM_NUM,                   
    IR_MEM_REG,                   
    IR_MEM_REG_NUM,             
    IR_NUM,                    
    IR_FUNC_TYPE, 
    };


struct Ir_command 
    {
    Instruction instr;
    ArgumentType arg1_type;
    ArgumentType arg2_type;
    Registers reg1;
    Registers reg2;
    double num;
    char comment[MAX_LEN_COMMENT];
    const char* call_func;
    size_t offset_var;
    const char* name_label;
    };


typedef Ir_command List_type;


struct Node
    {
    List_type value;
    iterator_t prev;
    iterator_t next;
    };
    

struct LIST
    {
    Node* data;
    size_t capacity;
    size_t size;

    iterator_t free;
    iterator_t front;
    iterator_t back;

    int status;
    };


struct Ir_Cond
    {
    size_t equal      = 0;
    size_t less       = 0;
    size_t more       = 0;
    size_t less_equal = 0;
    size_t more_equal = 0;
    size_t not_equal  = 0;

    size_t and_op     = 0;
    size_t or_op      = 0; 
    };



struct Ir_funcs
    {
    LIST commands;
    int num_func;
    const char* name_func;
    size_t func_num_vars;
    size_t cur_command;
    };


struct Ir
    {
    Ir_funcs* funcs;
    size_t num_main_func;
    const char* name_main_func;
    size_t cur_num_func;

    size_t if_label;
    size_t while_label;
    size_t logical_label;
    Ir_Cond cond;

    char glob_vars[MAX_GLOBALS][MAX_NAME_GLOB_VARS_LEN];
    size_t glob_vars_size;
    };