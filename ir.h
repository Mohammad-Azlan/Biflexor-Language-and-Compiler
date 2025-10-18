#ifndef IR_H
#define IR_H

#include <string>
#include <vector>

struct IRInstruction{
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string res;
};

#endif