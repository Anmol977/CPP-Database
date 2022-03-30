#ifndef ENUMS_H_
#define ENUMS_H_

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum { 
    STATEMENT_INSERT,
     STATEMENT_SELECT 
} StatementType;

typedef enum { 
    PREPARE_SUCCESS, 
    PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;

#endif