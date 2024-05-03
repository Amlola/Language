#include "../inc/Lexem.h"


int IsKeyword (char* word, int *keyword_code)
    {
#define DEF_KEYW(DEF, CODE, WORD)              \
    if (strcmp(word, #WORD) == 0)              \
        {                                      \
        *keyword_code = CODE;                  \
        }                                      \
    else

    #include "../../keyword"

        {
        *keyword_code = -1;
        return 0;
        }

#undef DEF_KEYW

    return 0;
    }


int GetWord(Text* data, char word[WORD_MAX_LEN], size_t* i)
    {
    size_t size = 0;

    while ((data->Buf[*i] >= 'a' && data->Buf[*i] <= 'z'  ||
            data->Buf[*i] >= 'A' && data->Buf[*i] <= 'Z'  || 
            data->Buf[*i] >= '0' && data->Buf[*i] <= '9'  ||
            data->Buf[*i] == '_') && data->Buf[*i] != '\0') 

            {
            word[size++] = data->Buf[(*i)++];
            }
    
    return 0;
    }


void GetTokensFromOneString(LIST* tokens, Text* data, size_t* i, LangNameTable* general_table, size_t number_string) 
    { 
    *i = SkipSpaces(data, *i);

    while (data->Buf[*i] != '\0') 
        {
        List_type token = {};

        token.line = number_string + 1;

        *i = SkipSpaces(data, *i);

        if (data->Buf[*i] == '\0') 
            break;

        if (isalpha(data->Buf[*i]))
            {
            char word[WORD_MAX_LEN] = "";

            GetWord(data, word, i);

            int keyword_code = 0;

            IsKeyword(word, &keyword_code);

            if (keyword_code >= 0)
                {
                token.type = KEYW_TYPE;

                token.form.key_w = keyword_code;
                }

            else
                {
                AddToNameTableIfNotFind(general_table, word, GENERAL_TABLE_INDEX, GENERAL_TYPE);

                token.type = ID_TYPE;

                memcpy(token.form.id, word, WORD_MAX_LEN * sizeof(char));
                }
            }

        else if (isdigit(data->Buf[*i])) 
            {
            double val = 0;

	        int check  = 0;

            sscanf(data->Buf + *i, "%lf%n", &val, &check);

            token.form.num = val;

            token.type = NUM_TYPE;

            (*i) += check;

            if (isalpha(data->Buf[*i]))
                {
                char word[WORD_MAX_LEN] = "";

                GetWord(data, word, i);

                fprintf(stderr, "error: identifier '%s' must start with a letter\n", word);

                data->count_n = 0;
                return;
                }
            }

        else 
            {

            token.type = KEYW_TYPE;


        #define DEF_OPER(DEF, CODE, SIGN, LEN)              \
            if (strncmp(data->Buf + *i, #SIGN, LEN) == 0)   \
                {                                           \
                sign = CODE;                                \
                (*i) += LEN;                                \
                }                                           \
            else

        #define DEF_OTHER(DEF, CODE, OTHER)                 \
            if (data->Buf[*i] == OTHER)                     \
                {                                           \
                sign = CODE;                                \
                (*i) += 1;                                  \
                }                                           \
            else

            int sign = 0;

            #include "../../operators"

                {
                sign = -1;
                }
            
            if (sign == -1)
                {
                fprintf(stderr, "error: unexpected lexeme %c\n", data->Buf[*i]);

                return;
                }
            
        #undef DEF_OPER
        #undef DEF_OTHER

            token.form.key_w = sign;
            }

        PushBack(tokens, token);
        }
    }


int GetTokens(Text* data, LIST* tokens, LangNameTable* general_table) 
    {
    size_t i = 0;

    size_t number_string = 0;

    for (number_string = 0; number_string < data->count_n; number_string++) 
        { 
        GetTokensFromOneString(tokens, data, &i, general_table, number_string);

        i++;
        }

    List_type token = {};

    token.type = KEYW_TYPE;

    token.line = number_string;

    token.form.key_w = KEYW_ENDOF;

    PushBack(tokens, token);

    return 0;
    }