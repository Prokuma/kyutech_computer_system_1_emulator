#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#define OPCODE 0
#define OPERAND 1
#define COMMA 2

extern char *opcode_list[16];

typedef struct token {
    char token_type;
    char *string;
    int string_len;
    struct token *next;
} Token;

#endif