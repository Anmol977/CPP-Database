#ifndef TABLE_H_
#define TABLE_H_
#include <cstdint>
#include <cstdlib>
#include "pager.h"
#include "schema.h"

// const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Table
{
public:
    uint32_t num_rows;
    Pager pager;
    Table();
    Table(const char *filename);
    uint32_t get_num_rows();
    void *row_slot(uint32_t row_num);
    void increment_num_rows();
    void db_close();
    ~Table();
};

#endif
