#include "statement.h"

PrepareResult Statement::prepare_statement(InputBuffer &input_buffer)
{
    if (strncmp(input_buffer.buffer, "insert", 6) == 0)
    {
        this->type = STATEMENT_INSERT;
        char *keyword = strtok(input_buffer.buffer, " ");
        char *id_str = strtok(NULL, " ");
        char *username = strtok(NULL, " ");
        char *email = strtok(NULL, " ");
        if (id_str == NULL || username == NULL || email == NULL)
        {
            // std::cout<<"smth is null \n";
            return PREPARE_SYNTAX_ERROR;
        }
        int id = atoi(id_str);
        if (id < 0)
        {
            return PREPARE_NEGATIVE_ID;
        }
        if (strlen(username) > COLUMN_USERNAME_SIZE || strlen(email) > COLUMN_EMAIL_SIZE)
        {
            return PREPARE_STRING_TOO_LONG;
        }
        this->row_to_insert.id = id;
        strcpy(this->row_to_insert.username, username);
        strcpy(this->row_to_insert.email, email);
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
    // std::cout<<"current rows in table : "<<table.num_rows<<"\n";
    if (table.num_rows >= TABLE_MAX_ROWS)
    {
        return EXECUTE_TABLE_FULL;
    }
    Row row_to_insert = this->row_to_insert;
    void *dest = table.row_slot(table.num_rows);
    // std::cout<<"serializing the row... \n";
    row_to_insert.serialize_row(dest);
    // std::cout<<"serializing done \n";
    table.increment_num_rows();
    return EXECUTE_SUCCESS;
}

ExecuteResult Statement::execute_select(Table &table)
{
    Row row;
    // std::cout<<"id \tusername \temail\n";
    for (uint32_t i = 0; i < table.num_rows; i++)
    {
        void *dest = table.row_slot(i);
        row.deserialize_row(dest);
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