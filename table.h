#ifndef TABLE_H_
#define TABLE_H_
#include <stdint.h>
#include <stdlib.h>
#include "enums.h"
#include "schema.h"

#define TABLE_MAX_PAGES 100

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Table
{
public:
    uint32_t num_rows;
    void *pages[TABLE_MAX_PAGES];
    Table()
    {
        // std::cout<<"table constructor called \n";
        this->num_rows = 0;
        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
        {
            this->pages[i] = nullptr;
        }
    }
    uint32_t get_num_rows()
    {
        return this->num_rows;
    }
    void *row_slot( uint32_t row_num)
    {
        uint32_t page_num = row_num / ROWS_PER_PAGE;
        void *page = this->pages[page_num];
        if (page == nullptr)
        {
            // std::cout<<"allocating page... \n";
            page = this->pages[page_num] = malloc(PAGE_SIZE);
        }
        uint32_t row_offset = row_num % ROWS_PER_PAGE;
        uint32_t byte_offset = row_offset * ROW_SIZE;
        return page + byte_offset;
    }
    void increment_num_rows(){
        this->num_rows++;
    }
    ~Table()
    {
        for (uint32_t i = 0; this->pages[i]; i++)
        {
            free(this->pages[i]);
        }
    }
};

#endif
