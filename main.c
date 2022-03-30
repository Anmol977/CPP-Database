#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "enums.h"

#define TRUE 1
#define RESET "\x1B[0m"
#define RED "\x1B[31m"

typedef struct
{
    StatementType type;
} Statement;

typedef struct
{
    char *buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer;

InputBuffer *init_input_buffer()
{
    InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_len = 0;

    input_buffer->input_len = 0;
    return input_buffer;
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer)
{
    if (strcmp(input_buffer->buffer, ".exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED;
    }
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *Statement)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0)
    {
        Statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    else if (strncmp(input_buffer->buffer, "select", 6) == 0)
    {
        Statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement)
{
    switch (statement->type)
    {
    case (STATEMENT_INSERT):
        printf("This is where we would do an insert.\n");
        break;
    case (STATEMENT_SELECT):
        printf("This is where we would do a select.\n");
        break;
    }
}

void read_input(InputBuffer *input_buffer)
{
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_len), stdin);
    if (bytes_read <= 0)
    {
        printf("Error reading input\n");
        exit(0);
    }
    input_buffer->input_len = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

void print_prompt() { printf(RED "db > " RESET); }

int main(int argc, char *argv[])
{
    InputBuffer *input_buffer = init_input_buffer();
    while (TRUE)
    {
        print_prompt();
        read_input(input_buffer);
        if (input_buffer->buffer[0] == '.')
        {
            switch (do_meta_command(input_buffer))
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED):
                printf("unrecognized command '%s'", input_buffer->buffer);
                continue;
            default:
                break;
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("urecognized keyword at start of '%s'. \n", input_buffer->buffer);
            continue;
        default:
            break;
        }
        execute_statement(&statement);
        printf("command executed.\n");
    }
}