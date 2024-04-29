#include "read.h"


long long GetFileSize(FILE* file)
    {
    assert(file);

    long long start = ftell(file);
    fseek(file, start, SEEK_END);

    long long bufsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    return bufsize;
    }


void FileInput(FILE* file, Text* data)
    {
    assert(file);
    assert(data);
    
    data->BufSize = GetFileSize(file);

    data->Buf = (char*)calloc(data->BufSize + 1, sizeof(char));

    fread(data->Buf, sizeof(char), data->BufSize, file);

    fclose(file);

    data->count_n = NumberLines(data->Buf, data->BufSize);

    ReplaceSlash_n(data->Buf, data->BufSize);
    }


void ReplaceSlash_n(char* Buf, size_t BufSize)
    {
    assert(Buf);

    for (size_t i = 0; i < BufSize; i++)
        {
        if (*(Buf + i) == '\n')
            {
            *(Buf + i) = '\0';
            }
        }
    }


int NumberLines(char* Buf, size_t BufSize)
    {
    assert(Buf);

    int count_n = 0;

    for (size_t i = 0; i < BufSize; i++)
        {
        if (*(Buf + i) == '\n')
            {
            count_n++;
            }
        }

    return count_n + 1;
    }


int SkipSpaces(Text* data, size_t i) 
    {
    assert(data);

    while (isspace(data->Buf[i])) 
        i++;
    
    return i;
    }