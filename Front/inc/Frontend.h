#pragma once


#include "../../Tree/tree.h"


const int GLOBAL_TABLE_ID = -1;

const int SYNTAX_ERROR = -1;

#define NO_ERROR_PTR (Node_t*)(123456);


Node_t* GetGlobalNameTable(LIST* tokens, LangNameTableArray* table_array); 

Node_t* GetGrammar(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetDefFunc(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetGlobalVariable(LIST* tokens, LangNameTableArray* table_array, List_type var_name, List_type var_type);

Node_t* GetOperatorsSequence(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetOperator(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetInit(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetNotInitToken(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetFunc(LIST* tokens, List_type func, LangNameTableArray* table_array);

Node_t* GetOr(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetAnd(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetLogicalExpression(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetComparison(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetMultDiv(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetPower(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetAddSub(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetBaseFunc(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetExpression(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetAssign(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetWhile(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetIf(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetElse(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetParamSequence(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetParam(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetReturn(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetVar(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetNumber(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetArgumentSequence(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetCyclesOperators(LIST* tokens, List_type token);

Node_t* CheckInitialization(List_type token);

Node_t* CheckOpenRoundBracket(LIST* tokens);

Node_t* CheckIdName(List_type token);

Node_t* CheckCloseRoundBracket(LIST* tokens);

Node_t* CheckOpenFigBracket(LIST* tokens);

Node_t* CheckCloseFigBracket(LIST* tokens);

Node_t* CheckSequentialOperator(List_type oper);

List_type GetFictToken(OperatorCode oper);

void CheckTableArraySize(LangNameTableArray* table_array);

bool CheckOperators(List_type oper);

void FrontDtor(Tree* tree, LIST* list, LangNameTableArray* table_array);

void BeginningOfInitVar(LangNameTable* table, int init_array[]);

void GetInitArray(int* init_array, LangNameTableArray* table_array);

Node_t* GetInitVarInNameTable(LangNameTableArray* table_array, List_type token);

Node_t* CheckAssign(LIST* tokens, LangNameTableArray* table_array, 
                    List_type var_name, List_type var_type, Node_t* rValue, Node_t* type);
