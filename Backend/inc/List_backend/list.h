#pragma once


#include "../Ir_struct.h"
#include "../../../Front/inc/Token_struct.h"


#define ListGraphDump(list_ptr) ListGraphDumpFunction(list_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__);
static FILE* graph_file_list = NULL;

#define PRINT_TO_GRAPH_FILE(...) fprintf(graph_file_list, __VA_ARGS__);

const int MAX_COMMAND_LENGTH = 125;

#define POISON_VALUE_FOR_ADRESS (List_type*)0xDED

#define POISON_NUMBER_FOR_VALUE 238832


const int size_list       =  0;
const int capacity_list   = 10;
const int size_extend     =  2;
const int ZERO            =  0;
const int FREE_INDEX      = -1;


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

void ListGraphDumpFunction(LIST* list, const char* path, const char* signature, unsigned line);