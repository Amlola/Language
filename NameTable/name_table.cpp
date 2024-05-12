#include "name_table.h"



NameTableError NameTableCtor(LangNameTable* name_table)
    {
    assert(name_table);

    name_table->Table = (LangNameTableUnit*)calloc(NAME_TABLE_CAPACITY, sizeof(LangNameTableUnit));

    if (!name_table->Table)
        return NAME_TABLE_ALLOC_ERROR;

    name_table->ptr = 0;

    name_table->capacity = NAME_TABLE_CAPACITY;

    name_table->Table->init = 0;

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

    for (size_t i = 0; i < name_table->capacity; i++)
        {
        if (number == i)
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

    fprintf(name_table_file, "%zu\n", table_array->Array[GENERAL_TABLE_INDEX].ptr);

    for (size_t j = 0; j < table_array->Array[GENERAL_TABLE_INDEX].ptr; j++)
        fprintf(name_table_file, "%s\n", table_array->Array[GENERAL_TABLE_INDEX].Table[j].name);

    fprintf(name_table_file, "\n");

    fprintf(name_table_file, "%zu\n\n", table_array->ptr);

    fprintf(name_table_file, "%zu %d\n", table_array->Array[GLOBAL_TABLE_INDEX].ptr, 
                                         table_array->Array[GLOBAL_TABLE_INDEX].table_number);

    size_t cur_func = 0;

    for (size_t j = 0; j < table_array->Array[GLOBAL_TABLE_INDEX].ptr; j++)
        {
        fprintf(name_table_file, "%zu %d\n", table_array->Array[GLOBAL_TABLE_INDEX].Table[j].id_index, 
                                             table_array->Array[GLOBAL_TABLE_INDEX].Table[j].type);

        if (table_array->Array[GLOBAL_TABLE_INDEX].Table[j].type != VAR_NAME)
            { 
            table_array->Array[cur_func + 2].table_number = table_array->Array[GLOBAL_TABLE_INDEX].Table[j].id_index;
            cur_func++;
            }
        }

    fprintf(name_table_file, "\n");

    size_t i = 2;

    size_t glob_vars = 0;

    while (i <= table_array->ptr)
        {
        for (size_t cur_var = 0; cur_var < table_array->Array[i].ptr; cur_var++)
            {
            printf("VAR: %s\n", table_array->Array[i].Table[cur_var].name);
            if (FindInNameTable(&table_array->Array[GLOBAL_TABLE_INDEX], table_array->Array[i].Table[cur_var].name) != -1)
                glob_vars++;
            }

        fprintf(name_table_file, "%zu %d\n", table_array->Array[i].ptr - glob_vars, table_array->Array[i].table_number);

        if ((error = WriteNameTableToFile(name_table_file, &(table_array->Array[i]), table_array)))
                return error;

        glob_vars = 0;
        
        fprintf(name_table_file, "\n");
        
        i++;
        }

    fclose(name_table_file);

    return error;
    }


NameTableError WriteNameTableToFile(FILE* name_table_file, LangNameTable* table, LangNameTableArray* table_array)
    {
    assert(name_table_file);
    assert(table);

    for (size_t i = 0; i < table->ptr; i++)
        {
        if (FindInNameTable(&table_array->Array[GLOBAL_TABLE_INDEX], table->Table[i].name) == -1)
            fprintf(name_table_file, "%zu %d\n", table->Table[i].id_index, table->Table[i].type);
        }

    return NO_NAME_TABLE_ERROR;
    }


NameTableError GetGlobalAndLocalsTable(LangNameTableArray* table_array, Text* data, size_t* index)
    {
    assert(table_array);
    assert(data);

    size_t locals_table_size = 0;
    int check = 0;

    sscanf(data->Buf + *index, "%zu%n", &locals_table_size, &check);
    (*index) += check;
    (*index) = SkipEndOfString(data, *index);

    printf("locals_table_size: %zu\n", locals_table_size);
   
    size_t global_table_size = 0;

    sscanf(data->Buf + *index, "%zu%n", &global_table_size, &check);
    (*index) += check + 1;

    LangNameTable* global_table = &table_array->Array[GLOBAL_TABLE_INDEX];
    NameTableCtor(global_table);

    printf("global_table_size: %zu\n", global_table_size);

    global_table->table_number = -1;
    (*index) += 2;
    (*index) = SkipEndOfString(data, *index);

    printf("global_table->table_number: %d\n", global_table->table_number);

    for (size_t i = 0; i < global_table_size; i++)
        {
        size_t id_index = 0;

        sscanf(data->Buf + *index, "%zu%n", &id_index, &check);
        (*index) += check + 1;

        printf("GLOBAL ID_INDEX IN GENERAL: %zu\n", id_index);

        AddToNameTable(global_table, table_array->Array[GENERAL_TABLE_INDEX].Table[id_index].name, 
                        id_index, (UnitNameTableTypes)(GET_NUMBER(*index)));

        (*index)++;
        (*index) = SkipEndOfString(data, *index);
        }

    table_array->ptr++;

    for (size_t table_number = 2; table_number < (locals_table_size + 1); table_number++) 
        {        
        NameTableCtor(&table_array->Array[table_number]);
        CheckTableArraySize(table_array);

        size_t current_table_size = 0;

        sscanf(data->Buf + *index, "%zu%n", &current_table_size, &check);
        (*index) += check + 1;

        printf("current_table_size %zu\n", current_table_size);

        sscanf(data->Buf + *index, "%d%n", &table_array->Array[table_number].table_number, &check);
        (*index) += check;
        (*index) = SkipEndOfString(data, *index);

        printf("table_array->Array[table_number].table_number %d\n", table_array->Array[table_number].table_number);

        size_t id_index = 0;

        for (size_t current_ident = 0; current_ident < current_table_size; current_ident++) 
            { 
            sscanf(data->Buf + *index, "%zu%n", &id_index, &check);
            (*index) += check + 1;

            printf("ID_INDEX IN GENERAL: %zu\n", id_index);

            AddToNameTable(&table_array->Array[table_number], table_array->Array[GENERAL_TABLE_INDEX].Table[id_index].name, 
                           id_index, (UnitNameTableTypes)(GET_NUMBER(*index)));

            (*index)++;
            (*index) = SkipEndOfString(data, *index);
            }
        } 

    return NO_NAME_TABLE_ERROR;
    }


size_t GetGeneralTable(LangNameTableArray* table_array, Text* data)
    {
    assert(table_array);
    assert(data);

    size_t general_table_size = 0;
    size_t i = 0;

    int check = 0;
    sscanf(data->Buf, "%zu%n", &general_table_size, &check);
    i += check;
    i = SkipEndOfString(data, i);

    printf("General: %zu\n", general_table_size);

    LangNameTable* general_table = &table_array->Array[GENERAL_TABLE_INDEX];

    NameTableCtor(general_table);

    for (size_t word_index = 0; word_index < general_table_size; word_index++) 
        {        
        char* string = nullptr;

        GetString(data, &string, &i);

        printf("IDENT: %s\n", string);

        AddToNameTable(general_table, string, GENERAL_TABLE_INDEX, GENERAL_TYPE);
        }

    table_array->ptr = 1;

    return SkipEndOfString(data, i);;
    }


NameTableError GetNameTableArray(LangNameTableArray* table_array, Text* data)
    {
    size_t number_string = 0;

    size_t i = GetGeneralTable(table_array, data);

    GetGlobalAndLocalsTable(table_array, data, &i);

    return NO_NAME_TABLE_ERROR;
    }


NameTableError GetString(Text* data, char** string, size_t* i)
    {
    assert(data);
    assert(string);
    assert(*i);
    
    *string = data->Buf;

    size_t symbol_index =  0;

    while (data->Buf[*i] != '\0')
        {
        (*string)[symbol_index++] = data->Buf[*i];

        (*i)++;
        }

    (*string)[symbol_index] = '\0';

    (*i)++;
    
    return NO_NAME_TABLE_ERROR;
    }


void CheckTableArraySize(LangNameTableArray* table_array) 
    {
    table_array->ptr++;

    if (table_array->ptr >= table_array->capacity)
        NameTableArrayRealloc(table_array);
    }