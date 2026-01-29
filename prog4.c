/*
* Print the names of ELF sections and shstrtab content.
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

    int fd;
    Elf *e;
    char *name, *p, pc[4*sizeof(char)];
    Elf_Scn *scn;
    Elf_Data *data;
    GElf_Shdr shdr;
    size_t n, shstrndx, sz;

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

    if(elf_getshdrstrndx(e, &shstrndx) != 0)
        errx(EX_SOFTWARE, "elf_getshdrstrndx() failes %s",elf_errmsg(-1));


    scn = NULL;

    while ((scn = elf_nextscn(e, scn)) != NULL){
        
        if(gelf_getshdr(scn ,&shdr) != &shdr)
            errx(EX_SOFTWARE, "getshdr() failed %s", elf_errmsg(-1));
        
        if((name = elf_strptr(e, shstrndx, shdr.sh_name)) == NULL)
            errx(EX_SOFTWARE, "elf_strptr() failed %s", elf_errmsg(-1));
        
        printf("Section %-4.4jd %s\n", (uintmax_t)elf_ndxscn(scn), name);
             
    }

    if((scn = elf_getscn(e, shstrndx)) == NULL)
        errx(EX_SOFTWARE, "getscn() failed %s", elf_errmsg(-1));
    
    if(gelf_getshdr(scn, &shdr) != &shdr)
        errx(EX_SOFTWARE, "getshdr(shstrndx) failed %s", elf_errmsg(-1));
    
    printf(".shstrtab: size=%jd\n", (uintmax_t)shdr.sh_size);

    data = NULL; n = 0;

    while(n < shdr.sh_size && (data = elf_getdata(scn, data)) != NULL){

        p = (char *)data->d_buf;
        while(p < (char *)data->d_buf + data->d_size){
            if(vis(pc, *p, VIS_WHITE, 0))
                printf("%s", pc);
            n++;
            p++;
            putchar((n % 16) ? ' '  : '\n');
        }
    }

    putchar('\n');

    elf_end(e);
    close(fd);
    exit(EX_OK);

}