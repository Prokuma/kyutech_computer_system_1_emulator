#include "hardware.h"

unsigned short register_list[4];
unsigned short accumulator;
unsigned short program_counter;
unsigned short instruction_register;
unsigned char memory[65536];

char zero_flag = 1;
char sign_flag = 0;

void set_flags() {
    if (accumulator == 0) {
        zero_flag = 1;
    } else {
        zero_flag = 0;
    }

    if (accumulator & (0x1 << 15) >> 15) {
        sign_flag = 1;
    } else {
        sign_flag = 0;
    }
}

void fetch() {
    instruction_register = memory[program_counter] << 8 | memory[program_counter+1];
}

void movi(unsigned short im) {
    accumulator = im;
    set_flags();
    program_counter += 2;
}

void addi(unsigned short im) {
    accumulator += im;
    set_flags();
    program_counter += 2;
}

void add(unsigned char reg) {
    if (reg >= 0 && reg < 4) {
        accumulator += register_list[reg];
        set_flags();
    } else if (reg == 5) {
        accumulator += accumulator;
        set_flags();
    }
    program_counter += 2;
}

void movfmd(unsigned short addr) {
    accumulator += memory[addr] << 8 | memory[addr+1];
    set_flags();
    program_counter += 2;
}

void movfmi(unsigned short addr) {
    unsigned short dst_addr = memory[addr] << 8 | memory[addr+1];
    accumulator += memory[dst_addr] << 8 | memory[dst_addr+1];
    set_flags();
    program_counter += 2;
}

void movfmr(unsigned char reg, unsigned char offset) {
    unsigned short addr;
    if (reg >= 0 && reg < 4) {
        addr = register_list[reg] + offset;
    } else if (reg == 5) {
        addr = accumulator + offset;
    } else {
        return;
    }

    accumulator += memory[addr] << 8 | memory[addr+1];
    set_flags();
    program_counter += 2;
}

void com(unsigned char reg) {
    if (reg >= 0 && reg < 4) {
        accumulator = ~register_list[reg] + 1;
        set_flags();
    } else if (reg == 5) {
        accumulator = ~accumulator + 1;
        set_flags();
    }
    program_counter += 2;
}

void not_(unsigned char reg) {
    if (reg >= 0 && reg < 4) {
        accumulator = ~register_list[reg];
        set_flags();
    } else if (reg == 5) {
        accumulator = ~accumulator;
        set_flags();
    }
    program_counter += 2;
}

void or_(unsigned char reg) {
    if (reg >= 0 && reg < 4) {
        accumulator = register_list[reg] | accumulator;
        set_flags();
    } else if (reg == 5) {
        accumulator = accumulator | accumulator;
        set_flags();
    }
    program_counter += 2;
}

void and_(unsigned char reg) {
    if (reg >= 0 && reg < 4) {
        accumulator = register_list[reg] & accumulator;
        set_flags();
    } else if (reg == 5) {
        accumulator = accumulator & accumulator;
        set_flags();
    }
    program_counter += 2;
}

void mone(unsigned char reg) {
    if (reg >= 0 && reg < 4) {
        register_list[reg] = ~1 + 1;
    } else if (reg == 5) {
        accumulator = ~1 + 1;
        set_flags();
    }
    program_counter += 2;
}

void movr(unsigned char org, unsigned char dst) {
    unsigned short *org_reg;
    unsigned short *dst_reg;

    if (org >= 0 && org < 4) {
        org_reg = &register_list[org];
    } else if (org == 5) {
        org_reg = &accumulator;
    } else {
        return;
    }

    if (dst >= 0 && dst < 4) {
        dst_reg = &register_list[dst];
    } else if (org == 5) {
        dst_reg = &accumulator;
    } else {
        return;
    }

    *org_reg = *dst_reg;
    set_flags();
    program_counter += 2;
}

void movtm(unsigned short addr) {
    memory[addr] = (unsigned char)((accumulator & 0xFF00) >> 8);
    memory[addr+1] = (unsigned char)(accumulator & 0x00FF);
    program_counter += 2;
}

void jmpz(unsigned short addr) {
    if (zero_flag) {
        program_counter = addr;
    } else {
        program_counter += 2;
    }
}
void jmpnz(unsigned short addr) {
    if (!zero_flag) {
        program_counter = addr;
    } else {
        program_counter += 2;
    }
}

void jmps(unsigned short addr) {
    if (sign_flag) {
        program_counter = addr;
    } else {
        program_counter += 2;
    }
}