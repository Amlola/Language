#pragma once


#include "../../Tree/tree.h"


const int GLOBAL_TABLE_ID = -1;


Node_t* GetGlobalNameTable(LIST* tokens, LangNameTableArray* table_array); 

Node_t* GetGrammar(LIST* tokens, LangNameTableArray* table_array);

Node_t* GetDefFunc(LIST* tokens, LangNameTableArray* table_array);

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

void CheckValidFunc(LIST* tokens);

void CheckOpenRoundBracket(LIST* tokens);

void CheckIdName(LIST* tokens);

void CheckCloseRoundBracket(LIST* tokens);

void CheckOpenFigBracket(LIST* tokens);

void CheckCloseFigBracket(LIST* tokens);

List_type GetFictToken();

void CheckTableArraySize(LangNameTableArray* table_array);