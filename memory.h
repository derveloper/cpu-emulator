#ifndef MEMORY_H
#define MEMORY_H

struct Memory : public std::map<const value, word> {
    byte memSize;
    byte registerOffset;
    byte programOffset;
    byte dataOffset;

    Memory() :
        memSize(0x3f), registerOffset(0x00),
        programOffset(0x20), dataOffset(0x30)
    {
        reset();
    }

    void reset() {
        clear();
        for (value i = 0; i < memSize.to_ulong(); i++) {
            this->insert(std::pair<const value, word>(i, 0));
        }
    }
};

#endif // MEMORY_H
