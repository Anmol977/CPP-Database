#ifndef INPUT_BUFFER_H_
#define INPUT_BUFFER_H_
#include <string>

class InputBuffer
{
public:
    char *buffer;
    size_t buffer_len;
    ssize_t input_len;
    InputBuffer()
    {
        // InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
        this->buffer = NULL;
        this->buffer_len = 0;
        this->input_len = 0;
    }
    void read_input()
    {
        ssize_t bytes_read = getline(&(this->buffer), &(this->buffer_len), stdin);
        if (bytes_read <= 0)
        {
            printf("Error reading input\n");
            exit(0);
        }
        this->input_len = bytes_read - 1;
        this->buffer[bytes_read - 1] = 0;
    }
    char *get_buffer()
    {
        return this->buffer;
    }
    ~InputBuffer()
    {
        free(this->buffer);
    }
};

#endif