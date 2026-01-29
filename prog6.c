/*
* Iterate through an ar (1) archive .
*/

#include <stdio.h>
#include <stdlib.h>

#include <err.h>
#include <sysexits.h>
#include <fcntl.h>
#include <unistd.h>

#include <libelf.h>


int main(int argc, char **argv){

    int fd;
    Elf *e, *ar;
    Elf_Arhdr *arh;

    if(argc != 2)
        errx(EX_USAGE, "usage: %s filename", argv[0]);

    if(elf_version(EV_CURRENT) == EV_NONE)
        errx(EX_SOFTWARE, "ELF library initialization failed : %s", elf_errmsg(-1));

    if((fd = open(argv[1], O_RDONLY, 0)) < 0)
        err(EX_NOINPUT, "open \"%s\" failed", argv[1]);    

    if(( ar = elf_begin(fd,ELF_C_READ, NULL)) == NULL)
        errx(EX_SOFTWARE, "elf_begin() failed %s", elf_errmsg(-1));
    
    if(elf_kind(ar) != ELF_K_AR)
        errx(EX_DATAERR, "\"%s \" is not an AR archive.", argv[1]);


    while((e = elf_begin(fd, ELF_C_READ, ar)) != NULL){

        if((arh = elf_getarhdr(e)) == NULL)
            errx(EX_SOFTWARE, "elf_getarhdr() failed %s", elf_errmsg(-1)); 

        printf("%20s %d\n", arh->ar_name, arh->ar_size);

        elf_next(e);
        elf_end(e);

    }

    close(fd);
    elf_end(e);
    exit(EX_OK);

}