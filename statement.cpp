#include "statement.h"

PrepareResult Statement::prepare_statement(InputBuffer &input_buffer)
{
    if (strncmp(input_buffer.buffer, "insert", 6) == 0)
    {
        this->type = STATEMENT_INSERT;
        int args_assigned = sscanf(input_buffer.buffer, "insert %d %s %s", &(this->row_to_insert.id), this->row_to_insert.username, this->row_to_insert.email);
        if (args_assigned < 3)
        {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    else if (strncmp(input_buffer.buffer, "select", 6) == 0)
    {
        this->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult Statement::execute_insert(Table &table)
{
    std::cout<<"current rows in table : "<<table.num_rows<<"\n";
    if (table.num_rows >= TABLE_MAX_ROWS)
    {
        return EXECUTE_TABLE_FULL;
    }
    Row row_to_insert = this->row_to_insert;
    void *dest = table.row_slot(table.num_rows);
    row_to_insert.serialize_row(dest);
    table.increment_num_rows();
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute_select(Table &table)
{
    Row row;
    std::cout<<"id \tusername \temail\n";
    for (uint32_t i = 0; i < table.num_rows; i++)
    {
        row.deserialize_row(table.row_slot(i));
        row.print();
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute_statement(Table &table)
{
    switch (this->type)
    {
    case (STATEMENT_INSERT):
        return execute_insert(table);
        break;
    case (STATEMENT_SELECT):
        return execute_select(table);
        break;
    }
}