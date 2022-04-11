#include "pager.h"

Pager::Pager()
{
}

Pager::Pager(const char *filename)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (fd == -1)
    {
        std::cout << "unable to open file \n";
        exit(EXIT_FAILURE);
    }
    this->file_len = lseek(fd, 0, SEEK_END);
    this->file_descriptor = fd;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        this->pages[i] = nullptr;
    }
}

void *Pager::get_page(uint32_t page_num)
{
    if (page_num > TABLE_MAX_PAGES)
    {
        std::cout << "Page number out of bounds \n";
        exit(EXIT_FAILURE); // a horrible way to deal with this exception
    }
    if (this->pages[page_num] == nullptr)
    {
        void *page = malloc(PAGE_SIZE);
        uint32_t num_pages = this->file_len / PAGE_SIZE;
        if (this->file_len % PAGE_SIZE)
            num_pages++;
        if (page_num <= num_pages)
        {
            // lseek to set file offset to page we want to read then copy it to page pointer
            lseek(this->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(this->file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1)
            {
                std::cout << "error reading file " << errno << std::endl;
                exit(EXIT_FAILURE); // once again, a horrible way..
            }
        }
        this->pages[page_num] = page;
    }
    return this->pages[page_num];
}

void Pager::page_flush(uint32_t page_num, uint32_t size)
{
    if (this->pages[page_num] == nullptr)
    {
        std::cout << "Could not flush a null page. \n";
        exit(EXIT_FAILURE); // just stop the friggin program on any exception :)
    }
    off_t offset = lseek(this->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
    if (offset == -1)
    {
        std::cout << "An error occured while seeking \n";
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_written = write(this->file_descriptor, this->pages[page_num], size);
    if (bytes_written == -1)
    {
        std::cout << "An error occured while writing to the database. \n";
        exit(EXIT_FAILURE);
    }
}