#pragma once


#include <elf.h>
#include "ir.h"


#define CUR_ADDRESS &elf->buf[elf->buf_size]

const size_t ELF_START_ADDRESS  = 0x400000;
const size_t ELF_TEXT_ADDRESS   = 0x400000 * 40;
const size_t ELF_ALIGNMENT      = 0x1000;

const size_t BUFFER_CAP         = 1000;

const size_t LIB_FILE_SIZE      = 306;
const size_t ELF_PRINT_SIZE     = 156;

const size_t ELF_PRINT_ADDRESS  = ELF_TEXT_ADDRESS;
const size_t ELF_INPUT_ADDRESS  = ELF_PRINT_ADDRESS + ELF_PRINT_SIZE;
const size_t ELF_MAIN_ADDRESS   = ELF_TEXT_ADDRESS;

const size_t VAR_BUF_SIZE       = 256;


struct ElfHeaders
    {
    Elf64_Ehdr elf_header;
    Elf64_Phdr text_header;
    };


struct JMP
    {
    unsigned char* address;
    const char* name;
    };


enum GO_ELF 
    {
    FISRT_TIME,
    SECOND_TIME
    };


struct BinaryTranslate
    {
    unsigned char* buf;
    size_t buf_size;
    GO_ELF status;
    JMP* label;
    JMP* call_func;
    };


enum ALL_CMDS
    {
    ELF_RET,
    ELF_CALL,
    ELF_MOV_XMM0_RSP_PL_8,
    ELF_MOV_XMM1_RSP,
    ELF_ADD_XMM0_XMM1,
    ELF_SUB_XMM0_XMM1,
    ELF_MUL_XMM0_XMM1,
    ELF_DIV_XMM0_XMM1,
    ELF_MOV_RSP_XMM0,
    ELF_JE,
    ELF_JNE,
    ELF_JB,
    ELF_JBE,
    ELF_JA,
    ELF_JAE,
    ELF_JMP,
    ELF_SQRT,
    ELF_PUSH_REG,
    ELF_POP_REG,
    ELF_CMP_R10_R11,
    ELF_CMP_R12_1,
    ELF_PUSH_R14_NUM_MEM,
    ELF_POP_R14_NUM_MEM,
    ELF_SUB_R14_NUM,
    ELF_ADD_R14_NUM,
    ELF_MOV_R12_1,
    ELF_MOV_R12_0,
    ELF_PUSH_R15,
    ELF_POP_R15,
    ELF_MOV_RCX_NUM
    };


struct Opcode
    {
    ALL_CMDS cmd;
    unsigned char opcode[16];
    size_t len;
    };


 const Opcode opcodes_array[] =
    {   
    {ELF_RET,               {0xC3},                                             1},   // ret
    {ELF_CALL,              {0xE8},                                             1},   // call
    {ELF_MOV_XMM0_RSP_PL_8, {0xF2, 0x0F, 0x10, 0x44, 0x24, 0x08},               6},   // mov xmm0, qword [rsp + 8]
    {ELF_MOV_XMM1_RSP,      {0xF2, 0x0F, 0x10, 0x0C, 0x24},                     5},   // mov xmm1, [rsp]
    {ELF_ADD_XMM0_XMM1,     {0xF2, 0x0F, 0x58, 0xC1},                           4},   // addsd xmm0, xmm1
    {ELF_SUB_XMM0_XMM1,     {0xF2, 0x0F, 0x5C, 0xC1},                           4},   // subsd xmm0, xmm1
    {ELF_MUL_XMM0_XMM1,     {0xF2, 0x0F, 0x59, 0xC1},                           4},   // mulsd xmm0, xmm1
    {ELF_DIV_XMM0_XMM1,     {0xF2, 0x0F, 0x5E, 0xC1},                           4},   // divsd xmm0, xmm1
    {ELF_MOV_RSP_XMM0,      {0xF2, 0x0F, 0x11, 0x04, 0x24},                     5},   // movsd [rsp], xmm0
    {ELF_JE,                {0x0F, 0x84, 0x00, 0x00, 0x00, 0x00},               6},   // je
    {ELF_JNE,               {0x0F, 0x85, 0x00, 0x00, 0x00, 0x00},               6},   // jne
    {ELF_JB,                {0x0F, 0x8C, 0x00, 0x00, 0x00, 0x00},               6},   // jb 
    {ELF_JBE,               {0x0F, 0x8E, 0x00, 0x00, 0x00, 0x00},               6},   // jbe
    {ELF_JA,                {0x0F, 0x8F, 0x00, 0x00, 0x00, 0x00},               6},   // ja
    {ELF_JAE,               {0x0F, 0x8D, 0x00, 0x00, 0x00, 0x00},               6},   // jae
    {ELF_JMP,               {0xE9, 0x00, 0x00, 0x00, 0x00},                     5},   // jmp
    {ELF_SQRT,              {0xF2, 0x0F, 0x51, 0xC1},                           4},   // sqrtsd
    {ELF_PUSH_REG,          {0x50},                                             1},   // push reg
    {ELF_POP_REG,           {0x58},                                             1},   // pop reg
    {ELF_CMP_R10_R11,       {0x4D, 0x39, 0xDA},                                 3},   // cmp r10, r11
    {ELF_CMP_R12_1,         {0x49, 0x83, 0xFC, 0x01},                           4},   // cmp r12, 1
    {ELF_PUSH_R14_NUM_MEM,  {0x41, 0xFF, 0x76},                                 3},   // push [r14 + num]
    {ELF_POP_R14_NUM_MEM,   {0x41, 0x8F, 0x86},                                 3},   // pop  [r14 + num]
    {ELF_SUB_R14_NUM,       {0x49, 0x81, 0xEE},                                 3},   // sub r14, num
    {ELF_ADD_R14_NUM,       {0x49, 0x81, 0xC6},                                 3},   // add r14, num
    {ELF_MOV_R12_1,         {0x49, 0xC7, 0xC4, 0x01, 0x00, 0x00, 0x00},         7},   // mov r12, 1
    {ELF_MOV_R12_0,         {0x49, 0xC7, 0xC4, 0x00, 0x00, 0x00, 0x00},         7},   // mov r12, 0
    {ELF_PUSH_R15,          {0x41, 0x57},                                       2},   // push r15
    {ELF_POP_R15,           {0x41, 0x5F},                                       2},   // pop r15
    {ELF_MOV_RCX_NUM,       {0x48, 0xB9},                                       2}    // mov rcx, num
    };


IrError GetElfHeader(Elf64_Ehdr* elf_header);

IrError GetProgHeaderTable(Elf64_Phdr* text_header, size_t code_size);

void PrintHeaders(FILE* file, size_t code_size);

IrError ElfCtor(BinaryTranslate* elf, Ir* ir);

IrError ElfDtor(BinaryTranslate* elf);

IrError GetBinaryFile(FILE* file, BinaryTranslate* elf, Ir* ir, LangNameTableArray* table_array);

IrError BinaryTranslation(BinaryTranslate* elf, Ir* ir);

IrError PrintBinaryMathOperation(BinaryTranslate* elf, List_type command);

IrError PrintBinaryPush(BinaryTranslate* elf, Ir_command* instr);

IrError PrintBinaryPop(BinaryTranslate* elf, Ir_command* instr);

IrError PrintBinaryOut(BinaryTranslate* elf, Ir* ir);

IrError GetAdressFunc(BinaryTranslate* elf, Ir_funcs* func);

IrError PrintBinaryCall(BinaryTranslate* elf, List_type command, Ir* ir);

void FillBinaryStart(BinaryTranslate* elf);

void GetBinaryLib(FILE* file);

void GetData(FILE* file, LangNameTableArray* table_array);

void FillBuffer(BinaryTranslate* elf, ALL_CMDS cmd);

void GetHexInt(BinaryTranslate* elf, int num);

void GetHexLong(BinaryTranslate* elf, long long num);
