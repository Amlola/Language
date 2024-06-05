#include "../inc/BinaryTransl.h"


IrError GetElfHeader(Elf64_Ehdr* elf_header)
    {
/*--------------------------------------ELF file identification---------------------------------------------------*/

    elf_header->e_ident[EI_MAG0]        = ELFMAG0;               // 0x7f                          
    elf_header->e_ident[EI_MAG1]        = ELFMAG1;               // 'E'              
    elf_header->e_ident[EI_MAG2]        = ELFMAG2;               // 'L'
    elf_header->e_ident[EI_MAG3]        = ELFMAG3;               // 'F'
    elf_header->e_ident[EI_CLASS]       = ELFCLASS64;            // bit depth 64
    elf_header->e_ident[EI_DATA]        = ELFDATA2LSB;           // little endian
    elf_header->e_ident[EI_VERSION]     = EV_CURRENT;            // version of the ELF format specification

    elf_header->e_ident[EI_OSABI]       = ELFOSABI_NONE;         // define the binary interface version and operating system
    elf_header->e_ident[EI_ABIVERSION]  = 0x00;                  // for Linux 0x00 0x00

    elf_header->e_ident[EI_PAD]         = ET_NONE;               // now filled with zero values

/*----------------------------------------------------------------------------------------------------------------*/ 

    elf_header->e_type       = ET_EXEC;                          // executable file(2), unknown(0), object(1), core(4), library(3)
    elf_header->e_machine    = EM_X86_64;                        // arch
    elf_header->e_version    = EV_CURRENT;                       // duplicates from e_ident
    elf_header->e_entry      = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + ELF_TEXT_ADDRESS;                     // ELF file entry point where program execution begins

    elf_header->e_phoff      = sizeof(Elf64_Ehdr);               // offset from file to program header table   |
                                                                 // offset from file to section header table   | relative to the beginning of the file!!!

                                                                 // for x86: 0
    elf_header->e_ehsize     = sizeof(Elf64_Ehdr);               // ELF header size
    elf_header->e_phentsize  = sizeof(Elf64_Phdr);               // size of one entry in the program headers section
    elf_header->e_phnum      = 1;                                // number of headers

    elf_header->e_shentsize  = sizeof(Elf64_Ehdr);
    elf_header->e_shnum      = 0;                                // for section
    elf_header->e_shstrndx   = 0;

    return NO_IR_ERROR;
    }


IrError GetProgHeaderTable(Elf64_Phdr* text_header, size_t code_size)
    {
/*-----------------------------------------------------------------------------------------------------*

/*-------------------------------------------------------------------------------------------------------*/           

    text_header->p_type      = PT_LOAD;                           // Program segment loaded into memory
    text_header->p_offset    = 0;                                 // offset from file start
    text_header->p_vaddr     = ELF_TEXT_ADDRESS;                  // Virtual address
    text_header->p_paddr     = ELF_TEXT_ADDRESS;                  // Physical address
    text_header->p_filesz    = code_size + sizeof(Elf64_Ehdr);    // segment size in bytes in the ELF file
    text_header->p_memsz     = code_size + sizeof(Elf64_Ehdr);    // segment size in bytes in the memory
    text_header->p_flags     = PF_R | PF_X;                       // PF_R = READ - 0x04, PF_X = EXEC - 0x01
    text_header->p_align     = ELF_ALIGNMENT;                     // alignment  


    /*data_header->p_type      = PT_LOAD;
    data_header->p_offset    = code_size + sizeof(Elf64_Ehdr);
    data_header->p_vaddr     = ELF_TEXT_ADDRESS + code_size;      // Virtual address
    data_header->p_paddr     = ELF_TEXT_ADDRESS + code_size;
    data_header->p_filesz    = VAR_BUF_SIZE + sizeof(Elf64_Ehdr); // segment size in bytes in the ELF file
    data_header->p_memsz     = VAR_BUF_SIZE + sizeof(Elf64_Ehdr); 
    data_header->p_flags     = PF_R | PF_W;                       // PF_R = READ - 0x04, PF_X = EXEC - 0x01
    data_header->p_align     = ELF_ALIGNMENT;*/



    /*
    1) Virtual address  - the address to which this segment should be mapped at runtime.
    2) Physical address - the address of the segment after loading into memory
    */
    
    return NO_IR_ERROR;
    }


void PrintHeaders(FILE* file, size_t code_size)
    {
    assert(file);

    ElfHeaders headers = {};

    GetElfHeader(&headers.elf_header);
    GetProgHeaderTable(&headers.text_header, code_size);

    char* segments = (char*)calloc(64 + 56, sizeof(char));
    memcpy(segments, &headers, sizeof(ElfHeaders));

    fwrite(segments, 64 + 56, 1, file);

    free(segments);
    }