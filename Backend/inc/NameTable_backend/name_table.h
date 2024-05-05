#pragma once


#include "../List_backend/list.h"
#include "../ReadingFile_backend/read.h"


const size_t MAX_NAME_LEN                = 50;
const size_t NAME_TABLE_CAPACITY         = 20;
const size_t NAME_TABLE_ARRAY_CAPACITY   = 10;
const size_t EXPANSION_CONSTANT          = 2;

const size_t GENERAL_TABLE_INDEX         = 0;
const size_t GLOBAL_TABLE_INDEX          = 1;


enum UnitNameTableTypes 
    {
    GENERAL_TYPE   = 0,
    VAR_NAME       = 1,
    FUNC_NAME      = 2,
    DATA_TYPE_NAME = 4
    };


enum NameTableError
    {
    NO_NAME_TABLE_ERROR               = 0,
    NAME_TABLE_ALLOC_ERROR            = 1 << 1,
    NAME_TABLE_NULLPTR                = 1 << 2,
    NAME_TABLE_ARRAY_ALLOC_ERROR      = 1 << 3,
    NAME_TABLE_ARRAY_NULLPTR          = 1 << 4
    };


struct LangNameTableUnit 
    {
    char name[MAX_NAME_LEN];
    size_t id_index;
    UnitNameTableTypes type;
    int init;
    };


struct LangNameTable 
    {
    LangNameTableUnit* Table;
    int table_number;
    size_t ptr;
    size_t capacity;
    };


struct LangNameTableArray 
    {
    LangNameTable* Array;
    size_t ptr;
    size_t capacity;
    };


NameTableError NameTableCtor(LangNameTable* name_table);

NameTableError NameTableDtor(LangNameTable* name_table);

NameTableError NameTableArrayCtor(LangNameTableArray* table_array);

NameTableError NameTableArrayDtor(LangNameTableArray* table_array);

NameTableError NameTableRealloc(LangNameTable* name_table);

NameTableError NameTableArrayRealloc(LangNameTableArray* table_array); 

NameTableError AddToNameTable(LangNameTable* name_table, char* name, size_t number, UnitNameTableTypes type);

int FindInNameTable(LangNameTable* name_table, char* name);

const char* FindInNameTableByCode(LangNameTable* name_table, size_t number);

LangNameTable* FindNameTable(LangNameTableArray* table_array, size_t number);

NameTableError WriteNameTableArrayToFile(FILE* name_table_file, LangNameTableArray* table_array);

NameTableError WriteNameTableToFile(FILE* name_table_file, LangNameTable* table);

void AddToNameTableIfNotFind(LangNameTable* name_table, char* name, size_t number, UnitNameTableTypes type); 

NameTableError GetNameTableArray(LangNameTableArray* table_array, Text* data);

NameTableError GetGlobalAndLocalsTable(LangNameTableArray* table_array, Text* data, size_t* index);

size_t GetGeneralTable(LangNameTableArray* table_array, Text* data);

NameTableError GetString(Text* data, char** string, size_t* i);

void CheckTableArraySize(LangNameTableArray* table_array);