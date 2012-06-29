#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "basic.h"
#include "util.h"

struct Instruction {
    opcode opCode;
    std::string opName;

    value a;
    std::string aName;
    bool aPtrVal;

    value b;
    std::string bName;
    bool bPtrVal;

    Instruction() : aPtrVal(false), bPtrVal(false) { }

    std::string getRegisterName(word addr, bool sourceOnly = false) {
        std::string opCodeName;
        const char _addr = addr.to_ulong();
        switch (_addr) {
        case IP:
            opCodeName = "IP";
            break;
        case R0:
            opCodeName = "R0";
            break;
        case R1:
            opCodeName = "R1";
            break;
        case R2:
            opCodeName = "R2";
            break;
        case R3:
            opCodeName = "R3";
            break;
        case R4:
            opCodeName = "R4";
            break;
        case R5:
            opCodeName = "R5";
            break;
        default:
            opCodeName = int_to_hex(_addr);
            break;
        }

        const char * addrStr = int_to_bin(_addr,6);
        return opCodeName + (!sourceOnly ? "(0b" + std::string(addrStr) + ")" : "");
    }

    std::string toString(bool sourceOnly = false) {
        switch (opCode) {
        case HLT:
            opName = "HLT";
            break;
        case INC:
            opName = "INC";
            break;
        case MOV:
            opName = "MOV";
            break;
        case OUT:
            opName = "OUT";
            break;
        case SET:
            opName = "SET";
            break;
        case IFE:
            opName = "IFE";
            break;
        default:
            opName = int_to_hex(opCode);
            break;
        }

        aName = getRegisterName(a, sourceOnly);
        bName = getRegisterName(b, sourceOnly);

        const char * opStr = int_to_bin(opCode, 6);

        opName = opName + (!sourceOnly ? "(0b" + std::string(opStr) + ")" : "");

        return opName + " " + aName + ", " + bName;
    }
};

#endif // INSTRUCTION_H
