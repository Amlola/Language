#include "../inc/BinaryTransl.h"


/*IrError elf_add_header (ELF_cmds* elf, int main_offset)
    {
    Elf64_Ehdr elf_header = {};
    elf_header.e_ident[EI_MAG0]        = ELFMAG0;
    elf_header.e_ident[EI_MAG1]        = ELFMAG1;              
    elf_header.e_ident[EI_MAG2]        = ELFMAG2;
    elf_header.e_ident[EI_MAG3]        = ELFMAG3;
    elf_header.e_ident[EI_CLASS]       = ELFCLASS64;
    elf_header.e_ident[EI_DATA]        = ELFDATA2LSB;
    elf_header.e_ident[EI_VERSION]     = EV_CURRENT;
    elf_header.e_ident[EI_OSABI]       = ELFOSABI_NONE;  
    elf_header.e_ident[EI_ABIVERSION]  = 0x00;  
    elf_header.e_ident[EI_PAD]         = ET_NONE;
    
    elf_header.e_type       = ET_EXEC;
    elf_header.e_machine    = EM_X86_64;
    elf_header.e_version    = EI_VERSION;                                   // дублирует из e_ident
    elf_header.e_entry      = ELF_START_ADR + ELF_ALIGNMENT + main_offset;  // абсолютный виртуальный адрес начала выполнения программы
    elf_header.e_phoff      = sizeof(Elf64_Ehdr);                           // смещение от файла к program header table
    elf_header.e_shoff      = 0;                                            // смещение от начала файла к section header table
    elf_header.e_flags      = 0;
    elf_header.e_ehsize     = sizeof(Elf64_Ehdr);                           // размер заголовка ELF
    elf_header.e_phentsize  = sizeof(Elf64_Phdr);
    elf_header.e_phnum      = 2;                                            // количество заголовков программы .text и .data
    elf_header.e_shentsize  = 0;
    elf_header.e_shnum      = 0;
    elf_header.e_shstrndx   = 0;

    memcpy (elf->cmds, &elf_header, sizeof(elf_header));

    return NO_IR_ERROR;
    }*/


/*IrError GetProgHeader(ELF_cmds* elf)
    {
    Elf64_Phdr text_prog_header  = {};

    text_prog_header.p_type      = PT_LOAD;
    text_prog_header.p_offset    = ELF_ALIGNMENT;                   // смещение от начала файла
    text_prog_header.p_vaddr     = ELF_START_ADR + ELF_ALIGNMENT;   // виртуальный адрес
    text_prog_header.p_paddr     = 0;                               // физический адрес
    text_prog_header.p_filesz    = ELF_ALIGNMENT;                   // количество байтов в образе файла сегмента
    text_prog_header.p_memsz     = ELF_ALIGNMENT;                   // количество байтов в памяти образа сегмента
    text_prog_header.p_flags     = PF_R | PF_X;                     // PF_R = READ - 0x04, PF_W = WRITE - 0x02, PF_X = EXEC - 0x01
    text_prog_header.p_align     = ELF_ALIGNMENT;                   // выравнивание

    memcpy(elf->cmds + sizeof (Elf64_Ehdr), &text_prog_header, sizeof(text_prog_header));

    Elf64_Phdr data_prog_header  = {};

    data_prog_header.p_type      = PT_LOAD;
    data_prog_header.p_offset    = 2 * ELF_ALIGNMENT;                   // смещение от начала файла
    data_prog_header.p_vaddr     = ELF_START_ADR + 2 * ELF_ALIGNMENT;   // виртуальный адрес
    data_prog_header.p_paddr     = 0;                                   // физический адрес
    data_prog_header.p_filesz    = 5 * ELF_ALIGNMENT;                   // количество байтов в образе файла сегмента
    data_prog_header.p_memsz     = 5 * ELF_ALIGNMENT;                   // количество байтов в памяти образа сегмента
    data_prog_header.p_flags     = PF_R | PF_X | PF_W;                  // PF_R = READ - 0x04, PF_W = WRITE - 0x02, PF_X = EXEC - 0x01
    data_prog_header.p_align     = ELF_ALIGNMENT;                       // выравнивание

    memcpy(elf->cmds + sizeof (Elf64_Ehdr) + sizeof (Elf64_Phdr), &data_prog_header, sizeof(data_prog_header));
    
    return NO_IR_ERROR;
    }*/