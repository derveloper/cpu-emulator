#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "basic.h"
#include "instruction.h"


class Disassambler {
public:
    Instruction decode(word inst) {
        opcode opCode = (inst >> 12).to_ulong();
        value a = ((inst & word((~0xf03f))) >> 6).to_ulong();
        value b = ((inst & word((~0xffc0)))).to_ulong();

        Instruction instruction;
        instruction.opCode = opCode;
        instruction.a = a;
        instruction.b = b;

        return instruction;
    }
};

class Assembler {
public:
    Assembler()
    {
    }

    word encode(byte opCode, word a, word b) {
        return encode((opcode) opCode.to_ulong(), (value) a.to_ulong(), (value) b.to_ulong());
    }

    word encode(byte opCode, byte a, word b) {
        return encode((opcode) opCode.to_ulong(), (value) a.to_ulong(), (value) b.to_ulong());
    }

    word encode(opcode opCode, value a, value b) {
        word instruction = opCode;

        instruction <<= 12;

        instruction = instruction ^ word((word(a) << 6));

        instruction = instruction ^ word(b);

        return instruction;
    }

    word encode(Instruction inst) {
        return encode(inst.opCode, inst.a, inst.b);
    }
};

#endif // ASSEMBLER_H
