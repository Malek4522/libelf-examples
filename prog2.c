/*
* Print the ELF Executable Header from an ELF object .
*/

#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>

#include <err.h>
#include <sysexits.h>
#include <fcntl.h>
#include <unistd.h>
#include <bsd/vis.h>

//#include <libelf.h>
#include <gelf.h>

int main(int argc, char **argv){

    int i,fd;
    Elf *e;
    char *id, bytes[5];
    size_t n;

    GElf_Ehdr ehdr;

    if(argc != 2)
        errx(EX_USAGE, "usage: %s filename", argv[0]);
    
    if(elf_version(EV_CURRENT) == EV_NONE)
        errx(EX_SOFTWARE, "ELF library initialization failed : %s", elf_errmsg(-1));
        
    if((fd = open(argv[1], O_RDONLY, 0)) < 0)
        err(EX_NOINPUT, "open \"%s\" failed", argv[1]);    

    if(( e = elf_begin(fd,ELF_C_READ, NULL)) == NULL)
        errx(EX_SOFTWARE, "elf_begin() failed %s", elf_errmsg(-1));
    
    if(elf_kind(e) != ELF_K_ELF)
        errx(EX_DATAERR, "\"%s \" is not an ELF object.", argv[1]);
    
    if(gelf_getehdr(e, &ehdr) == NULL)
        errx(EX_SOFTWARE, "getehdr() failed %s.", elf_errmsg(-1));
    
    if((i = gelf_getclass(e)) == ELFCLASSNONE)
        errx(EX_SOFTWARE, "getclass() failed %s", elf_errmsg(-1));
        

    printf("%s : %d-bit ELF object \n", argv[1], i == ELFCLASS32? 32 : 64);
    
    
    if((id = elf_getident(e, NULL)) == NULL)
        errx(EX_SOFTWARE, "getident() failed %s", elf_errmsg(-1));


    printf("%3s e_ident[0..%1d] %7s", " ",EI_ABIVERSION, "");

    for(i = 0; i <= EI_ABIVERSION; i++){

        vis(bytes, id[i], VIS_WHITE, 0);
        printf(" [%s %X]", bytes, id[i]);
    }

    printf("\n");

    #define PRINT_FMT "    %-20s 0x%jx \n"
    #define PRINT_FIELD(N)do{ \
        ( void ) printf (PRINT_FMT ,#N , (uintmax_t)ehdr.N); \
    } while(0)
    
    PRINT_FIELD(e_type);
    PRINT_FIELD(e_machine);
    PRINT_FIELD(e_version);
    PRINT_FIELD(e_entry);
    PRINT_FIELD(e_phoff);
    PRINT_FIELD(e_shoff);
    PRINT_FIELD(e_flags);
    PRINT_FIELD(e_ehsize);
    PRINT_FIELD(e_phentsize);
    PRINT_FIELD(e_shentsize);


    if(elf_getshdrnum(e, &n) != 0)
        errx(EX_SOFTWARE, "getshdrnum() failed %s", elf_errmsg(-1));

    printf(PRINT_FMT, "(shnum)", (uintmax_t)n);

    if(elf_getshdrstrndx(e, &n) != 0)
        errx(EX_SOFTWARE, "getshdstrndx() failes %s", elf_errmsg(-1));

    printf(PRINT_FMT, "(shstrndx)", (uintmax_t)n);

    if(elf_getphdrnum(e, &n) != 0)
        errx(EX_SOFTWARE, "getphdnum() failes %s", elf_errmsg(-1));

    printf(PRINT_FMT, "(phnum)",(uintmax_t)n);
    



    elf_end(e);
    close(fd);
    exit(EX_OK);
    
}