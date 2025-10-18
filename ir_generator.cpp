#include "ir_generator.h"
#include <iostream>
#include <map>
#include "parser.tab.h"

static std::map<int, std::string> op_name_map_2 = {
    {OR, "OR"}, {AND, "AND"}, {EQ, "EQ"}, {NEQ, "NEQ"},
    {LEQ, "LEQ"}, {GEQ, "GEQ"}, {LT, "LT"}, {GT, "GT"},
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {DIV, "DIV"},
    {NOT, "NOT"}, {INCREMENT, "INCREMENT"}, {DECREMENT, "DECREMENT"}
};

// Function to get operator name
std::string getOpName_2(int op_id) {
    auto it = op_name_map_2.find(op_id);
    if (it != op_name_map_2.end()) {
        return it->second;
    }
    return "UNKNOWN_OP(" + std::to_string(op_id) + ")";
}

std::string IRGenerator::newTemp(){
    return "t" + std::to_string(temp_counter++);
}
std::string IRGenerator::newLabel(){
    return "L" + std::to_string(label_counter++);
}

void IRGenerator::visit(ProgramNode& node){
    if(node.statements) node.statements->accept(*this);
}

void IRGenerator::visit(BlockStatementNode& node){
    for (const auto& stmt : node.statements) stmt->accept(*this);
}

void IRGenerator::visit(IntLiteralNode& node){
    node.tmp_name=std::to_string(node.value);
}

void IRGenerator::visit(FloatLiteralNode& node){
    node.tmp_name=std::to_string(node.value);
}

void IRGenerator::visit(StringLiteralNode& node){
    node.tmp_name=node.value;
}

void IRGenerator::visit(BooleanLiteralNode& node){
    node.tmp_name=node.value?"1":"0";
}

void IRGenerator::visit(IdentifierNode& node){
    node.tmp_name=node.name;
}

void IRGenerator::visit(EmptyStatementNode& node){
    return;
}

void IRGenerator::visit(BinaryExprNode& node){
    node.left->accept(*this);
    node.right->accept(*this);
    std::string t = newTemp();
    std::string op;
    switch(node.op){
        case OR:
        op="OR";
        break;

        case AND:
        op="AND";
        break;

        case EQ:
        op="==";
        break;

        case NEQ:
        op="!=";
        break;

        case LEQ:
        op="<=";
        break;

        case GEQ:
        op=">=";
        break;

        case LT:
        op="<";
        break;

        case GT:
        op=">";
        break;

        case PLUS:
        op="ADD";
        break;

        case MINUS:
        op="SUB";
        break;

        case MULT:
        op="MUL";
        break;

        case DIV:
        op="DIV";
        break;

        default:
        op="??";
    }
    instructions.push_back({op,node.left->tmp_name,node.right->tmp_name,t});
    node.tmp_name=t;
}

void IRGenerator::visit(UnaryExprNode& node){
    node.operand->accept(*this);
    std::string t = newTemp();
    std::string op;
    switch(node.op){
        case NOT:
        op="NOT";
        break;

        default:
        op="NEG";
    }
    instructions.push_back({op,node.operand->tmp_name,"",t});
    node.tmp_name=t;
}

void IRGenerator::visit(AssignmentStatementNode& node){
    node.value->accept(*this);
    instructions.push_back({"ASSIGN",node.value->tmp_name,"",node.id_name});
}

void IRGenerator::visit(IncDecStatementNode& node){
    std::string op;
    if (node.is_increment){
        op="INCREMENT";
    }
    else op="DECREMENT";
    instructions.push_back({op,node.id_name,"",node.id_name});
}

void IRGenerator::visit(DisplayStatementNode& node){
    node.expr->accept(*this);
    instructions.push_back({"DISP",node.expr->tmp_name,"",""});
}

void IRGenerator::visit(ReadStatementNode& node){
    instructions.push_back({"READ",node.id_name,"",""});
}

void IRGenerator::visit(IfStatementNode& node){
    node.condition->accept(*this);
    std::string else_Label=newLabel();
    std::string end_Label=newLabel();
    instructions.push_back({"IF_FALSE_GOTO",node.condition->tmp_name,else_Label,""});
    node.then_branch->accept(*this);
    instructions.push_back({"GOTO",end_Label,"",""});
    instructions.push_back({"LABEL",else_Label,"",""});
    if (node.else_branch) node.else_branch->accept(*this);
    instructions.push_back({"LABEL",end_Label,"",""});
}

void IRGenerator::visit(WhileStatementNode& node){
    std::string start_Label=newLabel();
    std::string end_Label=newLabel();
    instructions.push_back({"LABEL",start_Label,"",""});
    node.condition->accept(*this);
    instructions.push_back({"IF_FALSE_GOTO",node.condition->tmp_name,end_Label,""});
    node.body->accept(*this);
    instructions.push_back({"GOTO",start_Label,"",""});
    instructions.push_back({"LABEL",end_Label,"",""});
}

void IRGenerator::visit(ForStatementNode& node){
    std::string start_Label=newLabel();
    std::string end_Label=newLabel();
    if (node.init){
        node.init->accept(*this);
    }
    instructions.push_back({"LABEL",start_Label,"",""});
    if(node.condition){
        node.condition->accept(*this);
        instructions.push_back({"IF_FALSE_GOTO",node.condition->tmp_name,end_Label,""});
    }
    if (node.body){
        node.body->accept(*this);
    }

    if(node.increment){
        node.increment->accept(*this);
    }

    instructions.push_back({"GOTO",start_Label,"",""});
    instructions.push_back({"LABEL",end_Label,"",""});
}