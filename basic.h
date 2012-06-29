#ifndef BASIC_H
#define BASIC_H

#include <bitset>

//#define opcode unsigned char
//#define value unsigned char

typedef unsigned char opcode;
typedef unsigned char value;

typedef std::bitset<16> word;
typedef std::bitset<8> byte;

const opcode HLT = 0x00;
const opcode MOV = 0x01;
const opcode INC = 0x02;
const opcode OUT = 0x03;
const opcode SET = 0x04;
const opcode IFE = 0x05;

const value R0 = 0x00;
const value R1 = 0x01;
const value R2 = 0x02;
const value R3 = 0x03;
const value R4 = 0x04;
const value R5 = 0x05;
const value IP = 0x06;
const value SP = 0x07;

#endif // BASIC_H
