#include "name_table.h"



NameTableError NameTableCtor(LangNameTable* name_table)
    {
    assert(name_table);

    name_table->Table = (LangNameTableUnit*)calloc(NAME_TABLE_CAPACITY, sizeof(LangNameTableUnit));

    if (!name_table->Table)
        return NAME_TABLE_ALLOC_ERROR;

    name_table->ptr = 0;

    name_table->capacity = NAME_TABLE_CAPACITY;

    return NO_NAME_TABLE_ERROR;
    }


NameTableError NameTableDtor(LangNameTable* name_table)
    {
    assert(name_table);

    if (!name_table->Table)
        return NAME_TABLE_NULLPTR;

    free(name_table->Table);

    name_table->ptr = 0;

    name_table->capacity = 0;

    return NO_NAME_TABLE_ERROR;
    }


NameTableError NameTableArrayCtor(LangNameTableArray* table_array)
    {
    assert(table_array);
    
    table_array->Array = (LangNameTable*)calloc(NAME_TABLE_ARRAY_CAPACITY, sizeof(LangNameTable));

    if (!table_array->Array)
        return NAME_TABLE_ARRAY_ALLOC_ERROR;

    table_array->ptr = 0;

    table_array->capacity = NAME_TABLE_CAPACITY;

    return NO_NAME_TABLE_ERROR;
    } 


NameTableError NameTableArrayDtor(LangNameTableArray* table_array)
    {
    assert(table_array);

    NameTableError error = NO_NAME_TABLE_ERROR;

    if (!table_array->Array)
        return NAME_TABLE_ARRAY_NULLPTR;

    for (size_t i = 0; i <= table_array->ptr; i++)
        NameTableDtor(&(table_array->Array[i]));

    free(table_array->Array);

    table_array->ptr = 0;

    table_array->capacity = 0;

    return error;
    }


NameTableError NameTableRealloc(LangNameTable* name_table)
    {
    assert(name_table);

    name_table->Table = (LangNameTableUnit*)realloc(name_table->Table, EXPANSION_CONSTANT * name_table->capacity * sizeof(LangNameTableUnit));

    if (!name_table->Table)
        return NAME_TABLE_ALLOC_ERROR;

    name_table->capacity *= EXPANSION_CONSTANT;

    return NO_NAME_TABLE_ERROR;
    }


NameTableError NameTableArrayRealloc(LangNameTableArray* table_array) 
    {
    assert(table_array);

    table_array->Array = (LangNameTable*)realloc(table_array->Array, EXPANSION_CONSTANT * table_array->capacity * sizeof(LangNameTable));

    if (!table_array->Array)
        return NAME_TABLE_ARRAY_ALLOC_ERROR;

    table_array->capacity *= EXPANSION_CONSTANT;

    return NO_NAME_TABLE_ERROR;
    }


NameTableError AddToNameTable(LangNameTable* name_table, char* name, size_t number, UnitNameTableTypes type)
    {
    assert(name_table);
    assert(name);

    NameTableError error = NO_NAME_TABLE_ERROR;

    if (name_table->ptr >= name_table->capacity)
        {
        if ((error = NameTableRealloc(name_table)))
            return error;
        }
    
    strncpy(name_table->Table[name_table->ptr].name, name, MAX_NAME_LEN);

    name_table->Table[name_table->ptr].id_index = number;

    name_table->Table[name_table->ptr].type = type;

    (name_table->ptr)++;

    return error;
    }


void AddToNameTableIfNotFind(LangNameTable* name_table, char* name, size_t number, UnitNameTableTypes type) 
    {
    assert(name_table);
    assert(name);

    if (FindInNameTable(name_table, name) == -1) 
        AddToNameTable(name_table, name, number, type);
    }


int FindInNameTable(LangNameTable* name_table, char* name)
    {
    assert(name_table);
    assert(name);

    int position = -1;

    for (int i = 0; i <= name_table->ptr; i++)
        {
        if (strcmp(name, name_table->Table[i].name) == 0)
            position = i;
        }

    return position;
    }


const char* FindInNameTableByCode(LangNameTable* name_table, size_t number)
    {
    assert(name_table);

    for (size_t i = 0; i <= name_table->ptr; i++)
        {
        if (number == name_table->Table[i].id_index)
            return name_table->Table[i].name;
        }

    return nullptr;
    }


LangNameTable* FindNameTable(LangNameTableArray* table_array, size_t number)
    {
    assert(table_array);

    for (size_t i = 0; i < table_array->ptr; i++)
        {
        if (table_array->Array[i].table_number == number)
            return &(table_array->Array[i]);
        }

    return nullptr;
    }


NameTableError WriteNameTableArrayToFile(FILE* name_table_file, LangNameTableArray* table_array)
    {
    assert(name_table_file);
    assert(table_array);

    NameTableError error = NO_NAME_TABLE_ERROR;

    fprintf(name_table_file, "%zu\n", table_array->Array[0].ptr);
    for (size_t j = 0; j < table_array->Array[0].ptr; j++)
        fprintf(name_table_file, "%s\n", table_array->Array[0].Table[j].name);

    fprintf(name_table_file, "\n");

    fprintf(name_table_file, "%zu\n\n", table_array->ptr);

    fprintf(name_table_file, "%zu %d\n", table_array->Array[1].ptr, table_array->Array[1].table_number);
    for (size_t j = 0; j < table_array->Array[1].ptr; j++)
        {
        fprintf(name_table_file, "%zu %d\n", table_array->Array[1].Table[j].id_index, table_array->Array[1].Table[j].type);
        table_array->Array[j + 2].table_number = table_array->Array[1].Table[j].id_index;
        }

    fprintf(name_table_file, "\n");

    for (size_t i = 2; i <= table_array->ptr; i++)
        {
        fprintf(name_table_file, "%zu %d\n", table_array->Array[i].ptr, table_array->Array[i].table_number);

        if ((error = WriteNameTableToFile(name_table_file, &(table_array->Array[i]))))
            return error;

        fprintf(name_table_file, "\n");
        }

    fclose(name_table_file);

    return error;
    }


NameTableError WriteNameTableToFile(FILE* name_table_file, LangNameTable* table)
    {
    assert(name_table_file);
    assert(table);

    for (size_t i = 0; i < table->ptr; i++)
        fprintf(name_table_file, "%zu %d\n", table->Table[i].id_index, table->Table[i].type);

    return NO_NAME_TABLE_ERROR;
    }