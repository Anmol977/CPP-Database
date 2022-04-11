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

MetaCommandResult do_meta_command(InputBuffer input_buffer, Table &table)
{
    if (strcmp(input_buffer.buffer, ".exit") == 0)
    {
        table.db_close();
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED;
    }
}

void print_prompt()
{
    // std::cout << RED << "db > " << RESET;
    std::cout << "db > ";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_prompt();
        std::cout << "Must provide a database filename. \n";
        exit(EXIT_FAILURE);
    }
    char *filename = argv[1];
    InputBuffer input_buffer;
    Table table(filename);
    while (true)
    {
        print_prompt();
        input_buffer.read_input();
        if (input_buffer.buffer[0] == '.')
        {
            switch (do_meta_command(input_buffer, table))
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
        case (PREPARE_STRING_TOO_LONG):
            std::cout << "String is too long.\n";
            continue;
        case (PREPARE_NEGATIVE_ID):
            std::cout << "ID must be positive.\n";
            continue;
        case (PREPARE_SYNTAX_ERROR):
            std::cout << "Syntax error. Could not parse statement. \n";
            continue;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            std::cout << "urecognized keyword at start of '" << input_buffer.buffer << "'. \n";
            continue;
        }
        switch (statement.execute_statement(table))
        {
        case (EXECUTE_SUCCESS):
            std::cout << "Executed.\n";
            break;
        case (EXECUTE_TABLE_FULL):
            std::cout << "Error: Table full.\n";
            break;
        }
    }
}