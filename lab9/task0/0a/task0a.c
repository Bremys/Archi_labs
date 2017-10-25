#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>

int main(int argc, char** argv){
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0){
        perror("Error while opening file");
        return -1;
    }
    
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    
    Elf32_Ehdr* map_start =(Elf32_Ehdr*) mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (map_start==MAP_FAILED){
        fprintf(stderr, "%s\n", "Error: mapping failed");
        if (map_start != NULL){
            munmap(map_start, size);
            map_start = NULL;
        }
        close(fd);
        return -1;
    }
    
    unsigned char* magic_number = map_start->e_ident;
    
    if(strncmp("ELF", magic_number+1, 3) != 0){
        printf("%s\n\n", "Not an ELF file, cannot continue");
        if (map_start != NULL){
            munmap(map_start, size);
            map_start = NULL;
        }
        close(fd);
        return -1;
    }
    
    else{
        Elf32_Phdr* p_header = (Elf32_Phdr*) ((char*)map_start + map_start->e_phoff);
        int i;
        printf("%s  %s      %s     %s   %s    %s     %s   %s\n", "Type", "Offset", "FileSiz", "VirtAddr", "PhysAddr", "MemSiz", "Flg", "Align");
        for(i=0; i < map_start->e_phnum; ++i){
             printf("%d    0x%06x    %p    %p    0x%05x    0x%05x    %d  0x%x\n", p_header->p_type, p_header->p_offset, (void*)p_header->p_vaddr, (void*)p_header->p_paddr, p_header->p_filesz, p_header->p_memsz, p_header->p_flags, p_header->p_align);
             p_header++;
        }
    }
    
    return 0;
}
