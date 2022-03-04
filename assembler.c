#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *opcode_list[16] = {"movi", "addi", "add", "movfmd",
                         "movfmi", "movfmr", "com", "not",
                         "or", "and", "mone", "movr", 
                         "movtm", "jmpz", "jmpnz", "jmps"};

int is_same(char *dest, char *org, int dest_size, int org_size) {
    int i;

    if (dest_size != org_size) {
        return 0;
    }

    for (i = 0; i < dest_size; i++) {
        if (dest[i] != org[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    Token *top = NULL;
    Token *p = NULL;
    char assembly[100000];
    char binary[65536];
    int i = 0, j, k;

    int tmp_slen = 0;
    char *tmp_sp = assembly;
    char tmp_str[100];
    char tmp_type;
    unsigned char tmp_op1, tmp_op2;
    unsigned short tmp_op;
    unsigned short tmp_command;
    char c, is_end = 0;

    if (argc < 3) {
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        return 1;
    }

    memset(assembly, '\0', sizeof(assembly));   
    memset(tmp_str, '\0', sizeof(tmp_str));   

    while ((c = fgetc(fp)) != EOF) {
        assembly[i] = c;
        i++;
    }

    fclose(fp);

    // 字句解析・構文解析
    for (i = 0; assembly[i] != '\0'; i++) {
        if (!((assembly[i] >= '0' && assembly[i] <= '9') ||
            (assembly[i] >= 'a' && assembly[i] <= 'z') || assembly[i] == ',')) {
            continue; 
        }

        if (is_end) {
            tmp_sp = &assembly[i];
            is_end = 0;
            tmp_slen = 0;
        }

        if (p == NULL || p->token_type == OPERAND) {
            if (*tmp_sp == ',') {
                is_end = 1;
                tmp_type = COMMA;
                tmp_slen++;
            } else {
                for (j = 0; j < 16; j++) {
                    if (is_same(opcode_list[j], tmp_sp, strlen(opcode_list[j]), tmp_slen + 1)) {
                        is_end = 1;
                        tmp_type = OPCODE;
                    }
                }
                tmp_slen++;
            }
        } else if (p->token_type == COMMA || p->token_type == OPCODE) {
            if (!((assembly[i+1] >= '0' && assembly[i+1] <= '9') ||
                (assembly[i+1] >= 'a' && assembly[i+1] <= 'z'))) {
                is_end = 1;
                tmp_type = OPERAND;
            }
            tmp_slen++;
        } else {
            fprintf(stderr, "Syntax Error!\n");
            for (j = i; j < 10; j++) {
                fprintf(stderr, "%c", assembly[j]);
            }
            fprintf(stderr, "\n");
            return 1;
        }
        
        if (is_end) {
            if (top == NULL) {
                top = (Token*) malloc(sizeof(Token));
                p = top;
            } else {
                p->next = (Token*) malloc(sizeof(Token)); 
                p = p->next;
            }
            p->string = tmp_sp;
            p->string_len = tmp_slen;
            p->token_type = tmp_type;
            p->next = NULL;
        }
    }

    // バイナリ生成
    p = top;
    i = 0;
    tmp_command = 0;
    while (p != NULL) {
        if (p->token_type == OPCODE) {
            for (j = 0; j < 16; j++) {
                if (is_same(opcode_list[j], p->string, strlen(opcode_list[j]), p->string_len)) {
                    tmp_command = j << 12;
                    p = p->next;
                    break;
                }
            }
            if (j == 16) {
                fprintf(stderr, "Syntax Error!\n");
                return 1;
            }
        } else if (p->next != NULL  && (p->token_type == OPERAND && p->next->next->token_type == OPERAND)) {
            memcpy(tmp_str, p->string, p->string_len);
            tmp_op1 = atoi(tmp_str);
            memset(tmp_str, '\0', 100);
            memcpy(tmp_str, p->next->next->string, p->next->next->string_len);
            tmp_op2 = atoi(tmp_str);
            memset(tmp_str, '\0', 100);

            tmp_command |= tmp_op1 & 0x000F << 8;
            tmp_command |= tmp_op2 & 0x00FF;
            p = p->next->next->next;

            binary[i] = tmp_command & 0xFF00 >> 8;
            binary[i+1] = tmp_command & 0x00FF;
            i += 2;
            tmp_command = 0;
        } else {
            memcpy(tmp_str, p->string, p->string_len);
            tmp_op = atoi(tmp_str);
            memset(tmp_str, '\0', 100);

            tmp_command |= tmp_op & 0x0FFF;
            p = p->next;
            binary[i] = tmp_command & 0xFF00 >> 8;
            binary[i+1] = tmp_command & 0x00FF;
            i += 2;
            tmp_command = 0;
        }

    }

    if ((fp = fopen(argv[2], "w")) == NULL) {
        return 1;
    }

    fwrite(binary, sizeof(char), i, fp);
    fclose(fp);

    return 0;
}