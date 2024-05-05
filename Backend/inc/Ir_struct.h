#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>


enum Instruction
    {
    IR_PUSH,
    IR_POP,
    IR_RET,
    IR_CALL,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
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
    NO = -1,
    IR_RAX,
    IR_RBX,
    IR_RCX,
    IR_RDX,
    IR_RSP,
    IR_RBP,
    IR_RSI,
    IR_RDI,
    IR_R8,
    IR_R9,
    IR_R10,
    IR_R11,
    IR_R12,
    IR_R13,
    IR_R14,
    IR_R15
    };


struct RegistersArray
    {
    Registers reg;
    const char* name_reg;
    };


const RegistersArray registers_array[] = 
    {
    {NO,     ""},
    {IR_RAX, "rax"},
    {IR_RBX, "rbx"}, 
    {IR_RCX, "rcx"},
    {IR_RDX, "rdx"},
    {IR_RSP, "rsp"},
    {IR_RBP, "rbp"},
    {IR_RSI, "rsi"},
    {IR_RDI, "rdi"},
    {IR_R8,  "r8"},
    {IR_R9,  "r9"}, 
    {IR_R10, "r10"},
    {IR_R11, "r11"},
    {IR_R12, "r12"},
    {IR_R13, "r13"},
    {IR_R14, "r14"},
    {IR_R15, "r15"},
    };


struct Ir
    {
    Instruction instr;
    Registers reg1;
    Registers reg2;
    double imm;
    char* func_name;
    };