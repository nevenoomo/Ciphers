#pragma once
#include "basic_block_cypher.h"

class Mode {
   protected:
    istream& input;
    ostream& output;
    BasicBlockCypher b;

   public:
    virtual void run() = 0;
    Mode(BasicBlockCypher& b, istream& in, ostream& out)
        : b(b), input(in), output(out) {}
};