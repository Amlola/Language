#pragma once


#include <elf.h>
#include "ir.h"


struct ELF_cmds
    {
    unsigned char* cmds;
    int num_cmds;
    int cur_cmd;
    };