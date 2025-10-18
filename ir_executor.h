#ifndef IR_EXECUTOR_H
#define IR_EXECUTOR_H

#include "ir.h"
#include <unordered_map>
#include <string>
#include <iostream>

class IR_Executor{
public:
    std::unordered_map<std::string,std::string> vars;

    void run(const std::vector<IRInstruction>& instructions);
    int intresolve(const std::string& name);
    float floatresolve(const std::string& name);
    std::string stringresolve(const std::string& name);
    std::string dtyperesolve(const std::string&name);
};

#endif