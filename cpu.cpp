#include <iostream>
#include <fstream>
#include <map>

#include "basic.h"
#include "instruction.h"
#include "program.h"

struct CPU {
    Memory mem;
    bool programLoaded;
    value progSize;
    value dataSize;
    Disassambler disassembler;
    Assembler assembler;

    CPU() : programLoaded(false) { }

    void readBinary(std::string filename) {
        Program prog;
        std::ifstream bin(filename.c_str(), std::ios::binary );
        if (!bin)
        {
            std::cout << "I could not open \"" << filename << "\".\n";
            return;
        }

        char byte;
        bool isModeData = false;
        value size = 0;
        value pdc = 0;
        while (bin.get( byte ))
        {
            word inst;
            opcode o;
            value a;
            value b;
            if(size == 0) {
                size = byte;
                if(!bin.get(byte)) return;
            }
            if(!isModeData) {
                o = (opcode) byte;

                if(!bin.get(byte)) return;
                else {
                    a = (value) byte;
                    if(!bin.get(byte)) return;
                    else {
                        b = (value) byte;
                        inst = assembler.encode(o, a, b);
                        prog.push_back(inst);
                    }
                }

                pdc++;

                isModeData = pdc >= size;
            }
            else {
                inst = (value)byte;
                prog.data.push_back(inst);
            }
        }

        loadProgram(prog);
    }

private:
    void loadProgram(Program program) {
        progSize = program.size();
        dataSize = program.data.size();
        programLoaded = progSize > 0;
        value pc = mem.programOffset.to_ulong();
        value dc = mem.dataOffset.to_ulong();
        for(word inst : program) {
            mem[pc++] = inst;
        }
        for(word data : program.data) {
            mem[dc++] = data;
        }

        //std::cout << "loaded prog:\n" << program.toString(true) << std::endl;
    }

public:
    byte run() {
        if(programLoaded) {
            Instruction decoded;
            word inst;
            value pc = mem.programOffset.to_ulong();
            value v;
            for(;;) {
                inst = mem[pc++];
                decoded = disassembler.decode(inst);

                switch(decoded.opCode) {
                case HLT:
                    return mem[decoded.a].to_ulong();
                case SET:
                    if(decoded.a == IP) pc = decoded.b+mem.programOffset.to_ulong();
                    else mem[decoded.a] = decoded.b;
                    break;
                case OUT:
                    v = (unsigned char)mem[mem[decoded.a].to_ulong()].to_ulong();
                    printf("%c", v);
                    break;
                case INC:
                    mem[decoded.a] = mem[decoded.a].to_ulong() + 1;
                    break;
                case IFE:
                    if(mem[mem[decoded.a].to_ulong()] != mem[mem[decoded.b].to_ulong()]) pc++;
                    break;
                default:
                    return 1;
                }
            }
        }
        else {
            std::cout << "no program loaded" << std::endl;
        }

        return 0;
    }
};

int main(int argc, char **argv)
{
    if(argc < 2) {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    CPU cpu;

    cpu.readBinary(std::string(argv[1]));

    cpu.run();

    return 0;
}

