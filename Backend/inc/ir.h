#pragma once


#include "Tree_backend/tree.h"
#include "../../Front/inc/lexem_data.h"
#include "back_dsl.h"


enum IrError
    {
    IR_BAD              = -1,
    NO_IR_ERROR,
    IR_UNEXPECTED_TYPE  = 1 << 1,
    IR_UNKNOWN_KEYWORD  = 1 << 2
    };


const size_t IR_POIZON_VALUE = 2019283;


void IrCtor(Ir* ir, Tree* tree, LangNameTableArray* table_array);

void IrDtor(Ir* ir);

IrError GetIr(Ir* ir, Node_t* node, LangNameTableArray* table_array); // maybe done

IrError GetIrDecl(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrParameters(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrTypeNode(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrNum(Ir* ir, Node_t* node, LangNameTableArray* table_array);   // done

IrError GetIrId(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrVarDecl(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrCall(Ir* ir, Node_t* node, LangNameTableArray* table_array); //done

IrError GetIrArgumentSequence(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrBodyFunc(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrMathOperation(Ir* ir, Node_t* node, LangNameTableArray* table_array, Instruction instr);

IrError GetIrKeyword(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrComparison(Ir* ir, Node_t* node, LangNameTableArray* table_array, Instruction instr, size_t* cond, const char* name); // done

IrError GetIrAssign(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrPrintf(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrInput(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetLogicalAnd(Ir* ir, Node_t* node, LangNameTableArray* table_array, size_t* cond, const char* name);

IrError GetLogicalOr(Ir* ir, Node_t* node, LangNameTableArray* table_array, size_t* cond, const char* name);

IrError GetIrWhile(Ir* ir, Node_t* node, LangNameTableArray* table_array); // done

IrError GetIrIf(Ir* ir, Node_t* node, LangNameTableArray* table_array);

void AddCommand(Ir_funcs* func, Instruction instr, ArgumentType type_arg1, 
                                ArgumentType type_arg2, Registers arg1,
                                Registers arg2, double num, const char* comment, 
                                size_t offset_var, const char* call_func, const char* name_label);


size_t GetBufferSize(LangNameTableArray* table_array);

int GetPos(Ir* ir, Node_t* node, LangNameTableArray* table_array);

size_t GetGlobalStart(LangNameTableArray* table_array);

IrError GetGlobalVars(Ir* ir, Node_t* node, LangNameTableArray* table_array);

IrError GetIrGlobalId(Ir* ir, Node_t* node, LangNameTableArray* table_array);



