#include <woody.h>

int find_offset(t_wdy *obj)
{
    int i = 0;
    
    Elf64_Ehdr *hdr;
    Elf64_Shdr *shdr;
    Elf64_Shdr *secname_section;
    char *secname;

    hdr = (Elf64_Ehdr*)obj->ptr;
    obj->entry = hdr->e_entry;
    obj->entry_addr = &hdr->e_entry;
    if (!chk_ptr(wdy, obj->ptr, hdr->e_shoff))
    {
        ft_putstr("Pointer error\n");
        return (0);
    }
    shdr = obj->ptr + hdr->e_shoff;
    if (!chk_ptr(wdy, shdr, hdr->e_shstrndx))
    {
        ft_putstr("Pointer error\n");
        return (0);
    }
    secname_section = shdr + hdr->e_shstrndx;
    while (i < hdr->e_shnum)
    {
        if (!chk_ptr(wdy, obj->ptr, secname_section->sh_offset + shdr->sh_name) ||
        !chk_ptr(wdy, shdr, sizeof(*shdr)))
        {
            ft_putstr("Pointer error\n");
            return (0);
        }
        secname = (char*)(obj->ptr + secname_section->sh_offset + shdr->sh_name);
        printf("name:%s\n", secname);
        if (!strcmp(secname, ".plt"))
            return ((int)shdr->sh_offset);
        shdr++;
        i++;
    }
    return 0;
    
}

int check_null_space(t_wdy *obj)
{
    size_t         i;
    size_t         nbyte;
    char       *tmp_ptr;

    i = find_offset(obj);
    if (!i)
    {
        ft_putstr("Can't write in binary's sections\n");
        return (-1);
    }
    nbyte = i;
    tmp_ptr = (char *)obj->ptr;
    while (i < obj->size)
    {
        if (tmp_ptr[i])
            nbyte = i + 1;
        else if (i - nbyte >= SHELLCODE_LEN)
            return (nbyte);
        i++;
    }
    ft_putstr("Not enough NULL bytes to write in this binary\n");
    return (-2);
}

void insert_shellcode(t_wdy *obj, int offset)
{
    *(uint64_t *)obj->entry_addr = offset;
    ft_memcpy(obj->ptr + offset, ELF64_SHELLCODE, SHELLCODE_LEN);
}

int insert_pack(t_wdy *obj)
{
    int offset;
    int fp;

    if ((offset = check_null_space(obj)) == -1)
        return (0);
    insert_shellcode(obj, offset);
    if (!(fp = open("packed", O_CREAT | O_WRONLY, 0777) == -1 ))
    {
        printf("Can't open %s\n", packed);
        return (0);
    }
    write(fp, obj->ptr, obj->size);
    if (close(fp) == -1)
        return (0);
    return (1);
}