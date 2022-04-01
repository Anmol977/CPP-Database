#ifndef STATEMENT_H_
#define STATEMENT_H_
#include <cstring>
#include "enums.h"
#include "table.h"
#include "schema.h"
#include "input_buffer.h"

class Statement
{
public:
    StatementType type;
    Row row_to_insert;

    PrepareResult prepare_statement(InputBuffer &input_buffer);
    ExecuteResult execute_insert(Table &table);
    ExecuteResult execute_select(Table &table);
    ExecuteResult execute_statement(Table &table);
};

#endif
