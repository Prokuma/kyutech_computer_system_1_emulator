#ifndef _HARDWARE_H_
#define _HARDWARE_H_

extern unsigned short register_list[4];
extern unsigned short accumulator;
extern unsigned short program_counter;
extern unsigned short instruction_register;
extern unsigned char memory[65536];

extern char zero_flag;
extern char sign_flag;

void fetch();
void movi(unsigned short im);
void addi(unsigned short im);
void add(unsigned char reg);
void movfmd(unsigned short addr);
void movfmi(unsigned short addr);
void movfmr(unsigned char reg, unsigned char offset);
void com(unsigned char reg);
void not_(unsigned char reg);
void or_(unsigned char reg);
void and_(unsigned char reg);
void mone(unsigned char reg);
void movr(unsigned char org, unsigned char dst); 
void movtm(unsigned short addr);
void jmpz(unsigned short addr);
void jmpnz(unsigned short addr);
void jmps(unsigned short addr);

#endif