#include "ir_executor.h"

void IR_Executor::run(const std::vector<IRInstruction>& instructions){
    size_t linemarker=0;
    std::unordered_map<std::string,size_t> labels;

    for(size_t i=0;i<instructions.size();i++){
        if(instructions[i].op=="LABEL"){
            labels[instructions[i].arg1]=i;
        }
    }

    while(linemarker<instructions.size()){
        const auto& instruction = instructions[linemarker];

        // if(instruction.op=="IF_FALSE_GOTO"){
        //     if (IR_Executor::intresolve(instruction.arg1)==false){
        //         linemarker=labels[instruction.arg2];
        //         continue;
        //     }
        // }
        if (instruction.op == "IF_FALSE_GOTO") {
            // This is a special case that requires dynamic type handling
            std::string condition_val_str = stringresolve(instruction.arg1);
            bool condition_is_false = false;
            if (condition_val_str == "false" || condition_val_str == "0") {
                condition_is_false = true;
            } else {
                try {
                    float val = std::stof(condition_val_str);
                    if (val == 0.0f) {
                        condition_is_false = true;
                    }
                } catch (...) {
                    // Non-numeric strings are generally considered "true" in this context
                }
            }
            
            if (condition_is_false) {
                linemarker = labels.at(instruction.arg2);
                continue;
            }
            
        }
        else if(instruction.op=="GOTO"){
            linemarker=labels[instruction.arg1];
            continue;
        }

        else if(IR_Executor::dtyperesolve(instruction.arg1)=="INT"){
            int a1=IR_Executor::intresolve(instruction.arg1);
            int a2;
            if(instruction.arg2!="") a2=IR_Executor::intresolve(instruction.arg2);
            if (instruction.op=="OR"){
            vars[instruction.res]=std::to_string(a1 || a2);
        }
        else if(instruction.op=="AND"){
            vars[instruction.res]=std::to_string(a1 && a2);
        }
        else if(instruction.op=="=="){
            vars[instruction.res]=std::to_string(a1==a2);
        }
        else if(instruction.op=="!="){
            vars[instruction.res]=std::to_string(a1!=a2);
        }
        else if(instruction.op=="<="){
            vars[instruction.res]=std::to_string(a1<=a2);
        }
        else if(instruction.op==">="){
            vars[instruction.res]=std::to_string(a1>=a2);
        }
        else if(instruction.op=="<"){
            vars[instruction.res]=std::to_string(a1<a2);
        }
        else if(instruction.op==">"){
            vars[instruction.res]=std::to_string(a1>a2);
        }
        else if(instruction.op=="ADD"){
            vars[instruction.res]=std::to_string(a1+a2);
        }
        else if(instruction.op=="SUB"){
            vars[instruction.res]=std::to_string(a1-a2);
        }
        else if(instruction.op=="MUL"){
            vars[instruction.res]=std::to_string(a1*a2);
        }
        else if(instruction.op=="DIV"){
            vars[instruction.res]=std::to_string(a1/a2);
        }
        else if(instruction.op=="NOT"){
            vars[instruction.res]=std::to_string( !a1);
        }
        else if(instruction.op=="NEG"){
            vars[instruction.res]=std::to_string(-1*a1);
        }
        else if(instruction.op=="ASSIGN"){
            vars[instruction.res]=std::to_string(a1);
        }
        else if(instruction.op=="INCREMENT"){
            vars[instruction.res]= (std::stof(vars[instruction.res])-std::stoi(vars[instruction.res]))==0 ? std::to_string(std::stoi(vars[instruction.res])+1) : std::to_string(std::stof(vars[instruction.res])+1.0f);
        }
        else if(instruction.op=="DECREMENT"){
            vars[instruction.res]=(std::stof(vars[instruction.res])-std::stoi(vars[instruction.res]))==0 ? std::to_string(std::stoi(vars[instruction.res])-1) : std::to_string(std::stof(vars[instruction.res])-1.0f);
        }
        else if(instruction.op=="DISP"){
            std::cout<<a1<<std::endl;
        }
        else if(instruction.op=="READ"){
            std::string val;
            std::cin>>val;
            vars[instruction.arg1]=val;
        }
        }

        else if(IR_Executor::dtyperesolve(instruction.arg1)=="FLOAT"){
            float a1=IR_Executor::floatresolve(instruction.arg1);
            float a2;
            if(instruction.arg2!="") a2=IR_Executor::floatresolve(instruction.arg2);
            if (instruction.op=="OR"){
            vars[instruction.res]=std::to_string(a1 || a2);
        }
        else if(instruction.op=="AND"){
            vars[instruction.res]=std::to_string(a1 && a2);
        }
        else if(instruction.op=="=="){
            vars[instruction.res]=std::to_string(a1==a2);
        }
        else if(instruction.op=="!="){
            vars[instruction.res]=std::to_string(a1!=a2);
        }
        else if(instruction.op=="<="){
            vars[instruction.res]=std::to_string(a1<=a2);
        }
        else if(instruction.op==">="){
            vars[instruction.res]=std::to_string(a1>=a2);
        }
        else if(instruction.op=="<"){
            vars[instruction.res]=std::to_string(a1<a2);
        }
        else if(instruction.op==">"){
            vars[instruction.res]=std::to_string(a1>a2);
        }
        else if(instruction.op=="ADD"){
            vars[instruction.res]=std::to_string(a1+a2);
        }
        else if(instruction.op=="SUB"){
            vars[instruction.res]=std::to_string(a1-a2);
        }
        else if(instruction.op=="MUL"){
            vars[instruction.res]=std::to_string(a1*a2);
        }
        else if(instruction.op=="DIV"){
            vars[instruction.res]=std::to_string(a1/a2);
        }
        else if(instruction.op=="NOT"){
            vars[instruction.res]=std::to_string( !a1);
        }
        else if(instruction.op=="NEG"){
            vars[instruction.res]=std::to_string(-1*a1);
        }
        else if(instruction.op=="ASSIGN"){
            vars[instruction.res]=std::to_string(a1);
        }
        else if(instruction.op=="INCREMENT"){
            vars[instruction.res]=(std::stof(vars[instruction.res])-std::stoi(vars[instruction.res]))==0 ? std::to_string(std::stoi(vars[instruction.res])+1) : std::to_string(std::stof(vars[instruction.res])+1.0f);
        }
        else if(instruction.op=="DECREMENT"){
            vars[instruction.res]=(std::stof(vars[instruction.res])-std::stoi(vars[instruction.res]))==0 ? std::to_string(std::stoi(vars[instruction.res])-1) : std::to_string(std::stof(vars[instruction.res])+1.0f);
        }
        else if(instruction.op=="DISP"){
            std::cout<<a1<<std::endl;
        }
        else if(instruction.op=="READ"){
            std::string val;
            std::cin>>val;
            vars[instruction.arg1]=val;
        }
        }
        else{
            std::string a1=IR_Executor::stringresolve(instruction.arg1);
            std::string a2;
            if(instruction.arg2!="") a2=IR_Executor::stringresolve(instruction.arg2);
            if(a1=="true" || a1=="false"){
                if (instruction.op=="OR"){
                    vars[instruction.res]=std::to_string(a1=="true"?1:0 || a2=="true"?1:0);
                }
                else if(instruction.op=="AND"){
                    vars[instruction.res]=std::to_string(a1=="true"?1:0 && a2=="true"?1:0);
                }
                else if(instruction.op=="NOT"){
                    vars[instruction.res]=std::to_string(a1=="true"?0:1);
        }
        }
            
        // else if(instruction.op=="=="){
        //     vars[instruction.res]=std::to_string(a1==a2);
        // }
        // else if(instruction.op=="!="){
        //     vars[instruction.res]=std::to_string(a1!=a2);
        // }
        // else if(instruction.op=="<="){
        //     vars[instruction.res]=std::to_string(a1<=a2);
        // }
        // else if(instruction.op==">="){
        //     vars[instruction.res]=std::to_string(a1>=a2);
        // }
        // else if(instruction.op=="<"){
        //     vars[instruction.res]=std::to_string(a1<a2);
        // }
        // else if(instruction.op==">"){
        //     vars[instruction.res]=std::to_string(a1>a2);
        // }
        else if(instruction.op=="ADD"){
            vars[instruction.res]=a1+a2;
        }
        // else if(instruction.op=="SUB"){
        //     vars[instruction.res]=std::to_string(a1-a2);
        // }
        // else if(instruction.op=="MUL"){
        //     vars[instruction.res]=std::to_string(a1*a2);
        // }
        // else if(instruction.op=="DIV"){
        //     vars[instruction.res]=std::to_string(a1/a2);
        // }
        
        // else if(instruction.op=="NEG"){
        //     vars[instruction.res]=std::to_string(-1*a1);
        // }
        else if(instruction.op=="ASSIGN"){
            vars[instruction.res]=a1;
        }
        // else if(instruction.op=="INCREMENT"){
        //     vars[instruction.res]=(std::stof(vars[instruction.res]))%10==0 ? std::to_string(std::stoi(vars[instruction.res])+1) : std::to_string(std::stof(vars[instruction.res])+1.0f);
        // }
        // else if(instruction.op=="DECREMENT"){
        //     vars[instruction.res]=(std::stof(vars[instruction.res]))%10==0 ? std::to_string(std::stoi(vars[instruction.res])-1) : std::to_string(std::stof(vars[instruction.res])+1.0f);
        // }
        else if(instruction.op=="DISP"){
            std::cout<<a1<<std::endl;
        }
        else if(instruction.op=="READ"){
            std::string val;
            std::cin>>val;
            vars[instruction.arg1]=val;
        }
        
        }

        // if (instruction.op=="OR"){
        //     vars[instruction.res]=resolve(instruction.arg1) || resolve(instruction.arg2);
        // }
        // else if(instruction.op=="AND"){
        //     vars[instruction.res]=resolve(instruction.arg1) && resolve(instruction.arg2);
        // }
        // else if(instruction.op=="=="){
        //     vars[instruction.res]=resolve(instruction.arg1) == resolve(instruction.arg2)?1:0;
        // }
        // else if(instruction.op=="!="){
        //     vars[instruction.res]=resolve(instruction.arg1) != resolve(instruction.arg2)?1:0;
        // }
        // else if(instruction.op=="<="){
        //     vars[instruction.res]=resolve(instruction.arg1) <= resolve(instruction.arg2)?1:0;
        // }
        // else if(instruction.op==">="){
        //     vars[instruction.res]=resolve(instruction.arg1) >= resolve(instruction.arg2)?1:0;
        // }
        // else if(instruction.op=="<"){
        //     vars[instruction.res]=resolve(instruction.arg1) < resolve(instruction.arg2)?1:0;
        // }
        // else if(instruction.op==">"){
        //     vars[instruction.res]=resolve(instruction.arg1) > resolve(instruction.arg2)?1:0;
        // }
        // else if(instruction.op=="ADD"){
        //     vars[instruction.res]=resolve(instruction.arg1) + resolve(instruction.arg2);
        // }
        // else if(instruction.op=="SUB"){
        //     vars[instruction.res]=resolve(instruction.arg1) - resolve(instruction.arg2);
        // }
        // else if(instruction.op=="MUL"){
        //     vars[instruction.res]=resolve(instruction.arg1) * resolve(instruction.arg2);
        // }
        // else if(instruction.op=="DIV"){
        //     vars[instruction.res]=resolve(instruction.arg1) / resolve(instruction.arg2);
        // }
        // else if(instruction.op=="NOT"){
        //     vars[instruction.res]= !resolve(instruction.arg1);
        // }
        // else if(instruction.op=="NEG"){
        //     vars[instruction.res]=-1*resolve(instruction.arg1);
        // }
        // else if(instruction.op=="ASSIGN"){
        //     vars[instruction.res]=resolve(instruction.arg1);
        // }
        // else if(instruction.op=="INCREMENT"){
        //     vars[instruction.res]=vars[instruction.res]+1;
        // }
        // else if(instruction.op=="DECREMENT"){
        //     vars[instruction.res]=vars[instruction.res]-1;
        // }
        // else if(instruction.op=="DISP"){
        //     std::cout<<resolve(instruction.arg1)<<std::endl;
        // }
        // else if(instruction.op=="READ"){//need fixing for datatype
        //     int val;
        //     std::cin>>val;
        //     vars[instruction.arg1]=val;
        // }
        // else if(instruction.op=="IF_FALSE_GOTO"){
        //     if (resolve(instruction.arg1)==false){
        //         linemarker=labels[instruction.arg2];
        //         continue;
        //     }
        // }
        // else if(instruction.op=="GOTO"){
        //     linemarker=labels[instruction.arg1];
        //     continue;
        // }
        linemarker++;

    }
}

// int IR_Executor::resolve(const std::string& name){
//     if (isdigit(name[0])|| (name[0]=='-' && name.size()>1)){
//         return std::stoi(name);
//     }
//     return vars[name];
// }

int IR_Executor::intresolve(const std::string& name){
    if (isdigit(name[0])|| (name[0]=='-' && name.size()>1)){
        return std::stoi(name);
    }
    return std::stoi(vars[name]);
}

float IR_Executor::floatresolve(const std::string& name){
    if (isdigit(name[0])|| (name[0]=='-' && name.size()>1)){
        return std::stof(name);
    }
    return std::stof(vars[name]);
}

std::string IR_Executor::stringresolve(const std::string& name){
    if (vars.find(name)!=vars.end()){
        return vars[name];
    }
    else{
        return name;
    }
}

std::string IR_Executor::dtyperesolve(const std::string&name){
    std::string nname;
    if (vars.find(name)!=vars.end()){
        nname=vars[name];
    }
    else{
        nname=name;
    }
    if (isdigit(nname[0]) || (nname[0]=='-' && nname.size()>1)){
            if ((std::stof(nname)-std::stoi(nname))==0) return "INT";
            else return "FLOAT";
        }
        else return "STRING";
}

