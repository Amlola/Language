#pragma once


#include "ir.h"


void FillNasmStart(FILE* file, Ir* ir);

void FillNasmData(FILE* file, LangNameTableArray* table_array);

IrError NasmTranslation(Ir* ir, FILE* file); 

void PrintPushToNasm(FILE* file, Ir_command* instr);

void PrintPopToNasm(FILE* file, Ir_command* instr); 

void PrintMathOperationToNasm(FILE* file, List_type command); 

void PrintSqrtToNasm(FILE* file);

void PrintConditionalToNasm(FILE* file, List_type com_instr, Ir* ir, size_t cur_func, size_t command);
