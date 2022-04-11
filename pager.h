#ifndef PAGER_H_
#define PAGER_H_
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "values.h"

class Pager
{
public:
    int file_descriptor;
    uint32_t file_len;
    void *pages[TABLE_MAX_PAGES];
    Pager();
    Pager(const char *filename);
    void *get_page(uint32_t page_num);
    void page_flush(uint32_t page_num, uint32_t size);
};

#endif