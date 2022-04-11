#include "table.h"

Table::Table()
{
    this->num_rows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        this->pager.pages[i] = nullptr;
    }
}
Table::Table(const char *filename)
{
    std::cout << "table constructor \n";
    Pager pager(filename);
    this->num_rows = pager.file_len / ROW_SIZE;
    std::cout<<"num of rows already present : "<<this->num_rows<<std::endl;
    this->pager = pager;
}
uint32_t Table::get_num_rows()
{
    return this->num_rows;
}
void *Table::row_slot(uint32_t row_num)
{
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = this->pager.get_page(page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}
void Table::increment_num_rows()
{
    this->num_rows++;
}
void Table::db_close()
{
    uint32_t num_full_pages = this->num_rows / ROWS_PER_PAGE;
    for (uint32_t i = 0; i < num_full_pages; i++)
    {
        if (this->pager.pages[i] == nullptr)
            continue;
        this->pager.page_flush(i, PAGE_SIZE);
        free(this->pager.pages[i]);
        this->pager.pages[i] = nullptr;
    }
    uint32_t additional_rows = this->num_rows % ROWS_PER_PAGE;
    if (additional_rows > 0)
    {
        uint32_t page_num = num_full_pages;
        if (this->pager.pages[page_num] != nullptr)
        {
            this->pager.page_flush(page_num, additional_rows * ROW_SIZE);
            free(pager.pages[page_num]);
            this->pager.pages[page_num] = nullptr;
        }
    }
    int res = close(this->pager.file_descriptor);
    if (res == -1)
    {
        std::cout << "error closing database file. \n";
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        void *page = this->pager.pages[i];
        if (page)
        {
            free(page);
            this->pager.pages[i] = nullptr;
        }
    }
}
Table::~Table()
{
    for (uint32_t i = 0; this->pager.pages[i]; i++)
    {
        free(this->pager.pages[i]);
    }
}