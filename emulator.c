#include "hardware.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    int i = 0, steps;
    char c;

    char opcode;
    short operand;

    if (argc < 3) {
        fprintf(stderr, "Usage: ./emulator file steps\n");
        return 1;
    }

    //INITIALIZE
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Opening the file was failed!\n");
        return 1;
    }
    steps = atoi(argv[2]);
    memset(memory, 0, 65536);
    memset(register_list, 0, 8);
    program_counter = 0;
    instruction_register = 0;
    accumulator = 0;

    //LOAD PROGRAM
    while ((c= fgetc(fp)) != EOF) {
        memory[i] = c;
        i++;
    }

    fclose(fp);

    for (i = 0; i < steps; i++) {
        printf("STEP %d:\n", i);

        printf("PC:0x%x,IR:0x%x,ACC:0x%x\n", 
                program_counter, instruction_register,
                accumulator);
        printf("REG0:0x%x,REG1:0x%x,REG2:%x,REG3:0x%x\n",
                register_list[0], register_list[1],
                register_list[3], register_list[4]);
        printf("ZF:%d,SF:%d\n", zero_flag, sign_flag);

        fetch();

        //DECODER+EXECUTE
        opcode = (instruction_register & 0xF000) >> 12;
        operand = instruction_register & 0x0FFF;

        switch (opcode) {
            case 0x0:
                movi(operand);
                break;
            case 0x1:
                addi(operand);
                break;
            case 0x2:
                add(operand);
                break;
            case 0x3:
                movfmd(operand);
                break;
            case 0x4:
                movfmi(operand);
                break;
            case 0x5:
                movfmr((operand & 0xFF00) >> 8, operand & 0x00FF);
                break;
            case 0x6:
                com(operand);
                break;
            case 0x7:
                not_(operand);
                break;
            case 0x8:
                or_(operand);
                break;
            case 0x9:
                and_(operand);
                break;
            case 0xA:
                mone(operand);
                break;
            case 0xB:
                movr((operand & 0xFF00) >> 8, operand & 0x00FF);
                break;
            case 0xC:
                movtm(operand);
                break;
            case 0xD:
                jmpz(operand);
                break;
            case 0xE:
                jmpnz(operand);
                break;
            case 0xF:
                jmps(operand);
                break;
        }
    }

    return 0;
}