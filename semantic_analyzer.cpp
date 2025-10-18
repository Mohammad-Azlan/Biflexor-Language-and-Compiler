#include "semantic_analyzer.h"
#include <iostream>
#include <memory>
#include <map>
#include "parser.tab.h"

SemanticAnalyzer::SemanticAnalyzer(){}

static std::map<int, std::string> op_name_map_1 = {
    {OR, "OR"}, {AND, "AND"}, {EQ, "EQ"}, {NEQ, "NEQ"},
    {LEQ, "LEQ"}, {GEQ, "GEQ"}, {LT, "LT"}, {GT, "GT"},
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {DIV, "DIV"},
    {NOT, "NOT"}, {INCREMENT, "INCREMENT"}, {DECREMENT, "DECREMENT"}
};

// Function to get operator name
std::string getOpName_1(int op_id) {
    auto it = op_name_map_1.find(op_id);
    if (it != op_name_map_1.end()) {
        return it->second;
    }
    return "UNKNOWN_OP(" + std::to_string(op_id) + ")";
}

void SemanticAnalyzer::visit(ProgramNode& node) {
    if (node.statements) {
        node.statements->accept(*this);
    }
}

void SemanticAnalyzer::visit(BlockStatementNode& node) {
    for (const auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(EmptyStatementNode& node) {}

void SemanticAnalyzer::visit(IntLiteralNode& node) {}
void SemanticAnalyzer::visit(FloatLiteralNode& node) {}
void SemanticAnalyzer::visit(StringLiteralNode& node) {}
void SemanticAnalyzer::visit(BooleanLiteralNode& node) {}

void SemanticAnalyzer::visit(IdentifierNode& node) {
    if (symbol_table.find(node.name) == symbol_table.end()) {
        std::cerr << "Semantic error: Undeclared variable: '" << node.name
                  << "' used at line " << node.lineno << std::endl;
        node.id_type = "unknown";
    }
    else{
        node.id_type=symbol_table[node.name];
    }
}


void SemanticAnalyzer::visit(BinaryExprNode& node) {
    node.left->accept(*this);
    node.right->accept(*this);
    if(node.left->getType()!=node.right->getType()){
        std::cerr << "Semantic error: Left and right type not same in binary expression at line "
                  << node.lineno << std::endl;
        std::cout << "Left type: " << node.left->getType() << ", Right type: " << node.right->getType() << std::endl;
    }
}

void SemanticAnalyzer::visit(UnaryExprNode& node) {
    node.operand->accept(*this);
    if(getOpName_1(node.op)=="NOT" && node.operand->getType() != "bool") {
        std::cerr << "Semantic error: NOT operator applied to non-boolean type at line "
                  << node.lineno << std::endl;
    } else if (getOpName_1(node.op)=="MINUS"  && node.operand->getType() != "int" && node.operand->getType() != "float") {
        std::cerr << "Semantic error: Unary minus applied to non-numeric type at line "
                  << node.lineno << std::endl;
    }
}
void SemanticAnalyzer::visit(AssignmentStatementNode& node) {
    node.value->accept(*this);
    symbol_table[node.id_name] = node.value->getType();
    //std::cout<<"Type from map= "<<symbol_table[node.id_name]<<std::endl;
    std::cout << "Assigning type '" << node.value->getType() << "' to variable '" 
              << node.id_name << "' at line " << node.lineno << std::endl;
}

void SemanticAnalyzer::visit(IncDecStatementNode& node) {
    if (symbol_table.find(node.id_name) == symbol_table.end()) {
        std::cerr << "Semantic error: Variable '" << node.id_name
                  << "' used without declaration at line " << node.lineno << std::endl;
    }
    if(symbol_table[node.id_name] != "int" && symbol_table[node.id_name] != "float") {
        std::cerr << "Semantic error: Increment/Decrement operator applied to non-numeric type at line "
                  << node.lineno << std::endl;
    }
}
void SemanticAnalyzer::visit(DisplayStatementNode& node) {
    node.expr->accept(*this);
}

void SemanticAnalyzer::visit(ReadStatementNode& node) {
    symbol_table[node.id_name] = "int";
}

void SemanticAnalyzer::visit(IfStatementNode& node) {
    node.condition->accept(*this);
    node.then_branch->accept(*this);
    if (node.else_branch) {
        node.else_branch->accept(*this);
    }
}

void SemanticAnalyzer::visit(WhileStatementNode& node) {
    node.condition->accept(*this);
    node.body->accept(*this);
}

void SemanticAnalyzer::visit(ForStatementNode& node) {
    if (node.init) node.init->accept(*this);
    if (node.condition) node.condition->accept(*this);
    if (node.increment) node.increment->accept(*this);
    node.body->accept(*this);
}