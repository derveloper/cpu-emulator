#include "assembler.h"
#include "program.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <boost/regex.hpp>


int main(int argc, char **argv) {

    if(argc < 3) {
        std::cout << "usage: " << argv[0] << " <infile> <outfilename>" << std::endl;
        return 1;
    }

    Program prog;

    std::ifstream infile(argv[1]);

    std::string line;

    while (std::getline(infile, line))
    {
        boost::regex e("^(HLT|INC|SET|OUT|IFE)\\s?([a-zA-Z][a-zA-Z0-9]+)?[,]?\\s?(.*)?$");
        boost::smatch match;

        if(boost::regex_match(line, match, e)) {
            if(match.size() == 2) {
                prog.addInstruction(match[1]);
            }
            else if(match.size() > 2) {
                if(match.size() == 3) {
                    prog.addInstruction(match[1],match[2]);
                }else if(match.size() == 4) {
                    prog.addInstruction(match[1],match[2],match[3]);
                }
            }
        }
    }

    /*prog.addInstruction("SET", "R1", "$\"foo\\n\"");
    prog.addInstruction("SET", "R2", "$'0'");
    prog.addInstruction("IFE", "R1", "R2");
    prog.addInstruction("HLT");
    prog.addInstruction("OUT", "R1");
    prog.addInstruction("INC", "R1");
    prog.addInstruction("SET", "IP", "$2");*/

    prog.writeBinary(std::string(argv[2]));
}
