#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>


const int ASCII_ZERO = 48;
const size_t WORD_INDEX_BEGIN = 2;
const size_t SPACE_MAGNIFICATION = 2;

#define GET_NUMBER(index) data->Buf[index] - ASCII_ZERO


#define CHECK_FILE(file, name_file)                                 \
    if (!file)                                                      \
        {                                                           \
        fprintf(stderr, "failed to open file %s\n", name_file);     \
        return FAILED_TO_OPEN_FILE;                                 \
        }                                                           \


enum OPEN_ERROR 
    {
    FAILED_TO_OPEN_FILE = 1 << 1,
    NOT_ALL_ARGUMENTS_ARE_SPECIFIED = 1 << 2
    };


struct Text
    {
    char* Buf;
    long long BufSize;
    size_t count_n;
    };


void ReplaceSlash_n(char* Buf, size_t BufSize);

int NumberLines(char* Buf, size_t BufSize);

long long GetFileSize(FILE* file);

void FileInput(FILE* file, Text* data);

int SkipSpaces(Text* data, size_t i);

size_t SkipEndOfString(Text* data, size_t i); 
