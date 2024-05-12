#pragma once


#include "../../ReadingFile/read.h"
#include "../../NameTable/name_table.h"
#include "lexem_data.h"
    

int IsKeyword (char* word, int *keyword_code);

int GetWord(Text* data, char word[WORD_MAX_LEN], size_t* i);

void GetTokensFromOneString(LIST* tokens, Text* data, size_t* i, LangNameTable* general_table, size_t number_string); 

int GetTokens(Text* data, LIST* tokens, LangNameTable* general_table); 