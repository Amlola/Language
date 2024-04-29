#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>


#define INDEX_LIST

#ifdef INDEX_LIST
    typedef int iterator_t;
#endif


const int MAX_COMMAND_LENGTH = 125;

#define POISON_VALUE_FOR_ADRESS (List_type*)0xDED

#define POISON_NUMBER_FOR_VALUE 238832


const int size_list       =  0;
const int capacity_list   = 10;
const int size_extend     =  2;
const int ZERO            =  0;
const int FREE_INDEX      = -1;
const size_t WORD_MAX_LEN = 50;


enum Types
    {
    NUM_TYPE       = 1,
    ID_TYPE        = 2,
    KEYW_TYPE      = 3,
    FUNC_DEF_TYPE  = 4,
    PARAM_TYPE     = 5,
    VAR_DECL_TYPE  = 6,
    CALL_TYPE      = 7
    };


struct Token
    {
    Types type;

    union NodeType
        {
        int key_w;
        double num;
        char id[WORD_MAX_LEN];
        }form;
    };


const char* const str_types[] =
    {
    "constant", "id_type", "keyw_type", 
    "func_def_type", "param_type", 
    "var_decl_type", "call_type"
    };


typedef Token List_type;


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


typedef enum
    {
    LIST_OK                    = 0,
	CAPACITY_LESS_THAN_ZERO    = 1 << 1,
	SIZE_LESS_ZERO             = 1 << 2,
    LIST_IS_EMPTY              = 1 << 3,
    WRONG_NEW_CAPACITY         = 1 << 4,
    NULL_LIST                  = 1 << 5,
	NULL_DATA_BASE             = 1 << 6,
    ALLOC_FAIL                 = 1 << 7,
    CAPACITY_LESS_THAN_SIZE    = 1 << 8,
    INDEX_BIGGER_THAN_CAPACITY = 1 << 9,
    NUMBER_OF_ERROR            = 10
    }ListError;


typedef struct 
    {
    ListError CodeError;
    const char* NameError;
    }LIST_STATUS;

const LIST_STATUS ErrorArray[] = 
    {
    {LIST_OK,                    "NO ERROR"},
    {CAPACITY_LESS_THAN_ZERO,    "CAPACITY LESS THAN ZERO"},
    {SIZE_LESS_ZERO,             "SIZE LESS ZERO"},
    {LIST_IS_EMPTY,              "LIST IS EMPTY"},
    {WRONG_NEW_CAPACITY,         "WRONG NEW CAPACITY"},
    {NULL_LIST,                  "NULL LIST"},
    {NULL_DATA_BASE,             "NULL DATA"},
    {ALLOC_FAIL,                 "REALLOC FAIL"},
    {CAPACITY_LESS_THAN_SIZE,    "CAPACITY LESS THAN SIZE"},
    {INDEX_BIGGER_THAN_CAPACITY, "INDEX BIGGER THAN CAPACITY"},
    };


int ListCtor(LIST* list);

static void SetList(LIST* list, size_t size);

int ListVerify(LIST* list);

void ListDtor(LIST* list);

int ListInsert(LIST* list, List_type value, iterator_t index);

int PushBack(LIST* list, List_type value);

int PushFront(LIST* list, List_type value);

int PopBack(LIST* list);

int PopFront(LIST* list);

int ListDelete(LIST* list, iterator_t index);

static int ListResize(LIST* list, int new_capacity_list);

iterator_t FindByIndex(LIST* list, size_t index);

iterator_t NextCurIndex(LIST* list, size_t index);

iterator_t PrevCurIndex(LIST* list, size_t index);

iterator_t Begin(LIST* list);

iterator_t End(LIST* list);

void UpdateParams(LIST* list); 