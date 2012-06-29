#ifndef PROGRAM_H
#define PROGRAM_H

#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <regex.h>

#include "basic.h"
#include "memory.h"
#include "instruction.h"
#include "assembler.h"


struct Program : public std::vector<word> {
    std::vector<word> data;
    Memory mem;
    std::map<std::string, opcode> OpCodes;
    std::map<std::string, value> Registers;
    Disassambler disassambler;
    Assembler assembler;

    Program() {
        OpCodes.insert(std::pair<std::string, opcode>("HLT", HLT));
        OpCodes.insert(std::pair<std::string, opcode>("INC", INC));
        OpCodes.insert(std::pair<std::string, opcode>("SET", SET));
        OpCodes.insert(std::pair<std::string, opcode>("IFE", IFE));
        OpCodes.insert(std::pair<std::string, opcode>("OUT", OUT));

        Registers.insert(std::pair<std::string, value>("0", 0));
        Registers.insert(std::pair<std::string, value>("R0", R0));
        Registers.insert(std::pair<std::string, value>("R1", R1));
        Registers.insert(std::pair<std::string, value>("R2", R2));
        Registers.insert(std::pair<std::string, value>("R3", R3));
        Registers.insert(std::pair<std::string, value>("R4", R4));
        Registers.insert(std::pair<std::string, value>("R5", R5));
        Registers.insert(std::pair<std::string, value>("IP", IP));
        Registers.insert(std::pair<std::string, value>("SP", SP));
    }


    void writeBinary(std::string filename) {
        std::ofstream outbin(filename.c_str(), std::ios::binary);
        Instruction i;
        opcode opCode;
        value a;
        value b;

        outbin << (value)this->size();

        for(word inst : *this) {
            i = disassambler.decode(inst);
            opCode = i.opCode;
            a = i.a;
            b = i.b;
            outbin << opCode << a << b;
        }

        for(word d : data) {
            value ii = d.to_ulong();
            outbin << ii;
        }
        outbin.close();
    }

    void addInstruction(Instruction inst) {
        this->push_back(assembler.encode(inst));
    }

    void addInstruction(opcode opCode, value a, value b) {
        this->push_back(assembler.encode(opCode, a, b));
    }

    void addInstruction(value opCode, value a) {
        addInstruction(opCode, a, (value)0);
    }

    void addInstruction(value opCode) {
        addInstruction(opCode, (value)0, (value)0);
    }

    void addInstruction(opcode opCode, value a, word b) {
        data.push_back(b);
        value dp = mem.dataOffset.to_ulong()+(data.size()-1);

        addInstruction(opCode, a, dp);
    }

    void addInstruction(opcode opCode, value a, std::string b) {
        for(word c : b) {
            data.push_back(c);
        }
        value dp = mem.dataOffset.to_ulong()+(data.size()-b.length());

        addInstruction(opCode, a, dp);
    }

    void addInstruction(opcode opCode, value a, char b) {
        data.push_back(b);
        value dp = mem.dataOffset.to_ulong()+(data.size()-1);

        addInstruction(opCode, a, dp);
    }

    void addInstruction(std::string opCode, std::string a = "0", std::string b = "0") {
        static const boost::regex e("^\\$\"(.*)\"$");
        static const boost::regex e2("^\\$(\\d+|(\\n|\\t|\\0|\\a))$");
        static const boost::regex e3("^\\$'[\\\\]*([a-zA-Z0-9]?)'$");
        boost::smatch match;
        boost::smatch match2;
        boost::smatch match3;
        if(boost::regex_match(b, match, e) && match.size() > 1) {
            std::string _b = match[1];
            boost::algorithm::replace_all(_b, "\\n", "\n");
            boost::algorithm::replace_all(_b, "\\0", "\0");
            boost::algorithm::replace_all(_b, "\\t", "\t");
            boost::algorithm::replace_all(_b, "\\a", "\a");
            addInstruction(OpCodes[opCode], Registers[a], _b);

            return;
        }
        else if(boost::regex_match(b, match2, e2) && match2.size() > 1) {
            unsigned short _b = boost::lexical_cast<unsigned short>(match2[1]);
            addInstruction(OpCodes[opCode], Registers[a], (value)_b);

            return;
        }
        else if(boost::regex_match(b, match3, e3) && match3.size() > 1) {
            std::string c = match3[1];
            boost::algorithm::replace_all(c, "\\n", "\n");
            boost::algorithm::replace_all(c, "\\0", "\0");
            boost::algorithm::replace_all(c, "\\t", "\t");
            boost::algorithm::replace_all(c, "\\a", "\a");

            unsigned short _b = boost::lexical_cast<unsigned short>(c);

            addInstruction(OpCodes[opCode], Registers[a], (char)_b);
            return;
        }

        addInstruction(OpCodes[opCode], Registers[a], Registers[b]);
    }

    std::string toString(bool sourceOnly = false) {
        std::string source = "";

        source += ".db\n";

        for(word d : data) {
            source += int_to_hex(d.to_ulong()) + " ";
        }

        source += "\n\n.text\n";

        for(word inst : *this) {
            source += disassambler.decode(inst).toString(sourceOnly) + "\n";
        }

        source += "\n";

        return source;
    }
};

#endif // PROGRAM_H
