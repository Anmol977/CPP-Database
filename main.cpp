#include <cstdint>
#include <iostream>
#include <cstring>
#include "enums.h"
#include "schema.h"
#include "table.h"
#include "statement.h"
#include "input_buffer.h"

#define RED "\033[31m"
#define RESET "\033[0m"

MetaCommandResult do_meta_command(InputBuffer input_buffer)
{
    if (strcmp(input_buffer.buffer,".exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED;
    }
}

void print_prompt()
{
    std::cout << RED << "db > " << RESET;
}

int main()
{
    InputBuffer input_buffer;
    Table table;
    while (true)
    {
        print_prompt();
        input_buffer.read_input();
        if (input_buffer.buffer[0] == '.')
        {
            switch (do_meta_command(input_buffer))
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED):
                std::cout << "unrecognized command '" << input_buffer.buffer << "'" << std::endl;
                continue;
            default:
                break;
            }
        }
        Statement statement;
        switch (statement.prepare_statement(input_buffer))
        {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_SYNTAX_ERROR):
            std::cout << "Syntax error. Could not parse statement. \n";
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            std::cout << "urecognized keyword at start of '" << input_buffer.buffer << "'. \n";
            continue;
        }
        switch (statement.execute_statement(table))
        {
        case (EXECUTE_SUCCESS):
            std::cout << "command executed.\n";
            break;
            case(EXECUTE_TABLE_FULL):
                std::cout<<"Error: Table full. \n";
                break;
        }
    }
}