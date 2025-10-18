#include "ast.h"
#include "parser.tab.h" // Include this to get token IDs like PLUS, EQ, NOT
#include <iostream>
#include <string>
#include <algorithm> // For std::remove (if used for string literal quotes)
#include <map>       // For mapping token IDs to strings

// Global root of the AST
std::unique_ptr<ProgramNode> ast_root = nullptr;

// Helper to print indentation
static void indentation(int indent) {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  "; // Two spaces per indent level
    }
}

// Helper map for operator names (define globally or as static member)
static std::map<int, std::string> op_name_map = {
    {OR, "OR"}, {AND, "AND"}, {EQ, "EQ"}, {NEQ, "NEQ"},
    {LEQ, "LEQ"}, {GEQ, "GEQ"}, {LT, "LT"}, {GT, "GT"},
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {DIV, "DIV"},
    {NOT, "NOT"}, {INCREMENT, "INCREMENT"}, {DECREMENT, "DECREMENT"}
};

// Function to get operator name
std::string getOpName(int op_id) {
    auto it = op_name_map.find(op_id);
    if (it != op_name_map.end()) {
        return it->second;
    }
    return "UNKNOWN_OP(" + std::to_string(op_id) + ")";
}

// --- ExprNode Implementations ---
void IntLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IntLiteralNode::dump(int indent) const {
    indentation(indent);
    std::cout << "IntLiteralNode (Line " << lineno << "): " << value << std::endl;
}
std::string IntLiteralNode::getType() const { return "int"; }

void FloatLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void FloatLiteralNode::dump(int indent) const {
    indentation(indent);
    std::cout << "FloatLiteralNode (Line " << lineno << "): " << value << std::endl;
}
std::string FloatLiteralNode::getType() const { return "float"; }

void StringLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StringLiteralNode::dump(int indent) const {
    indentation(indent);
    std::cout << "StringLiteralNode (Line " << lineno << "): \"" << value << "\"" << std::endl;
}
std::string StringLiteralNode::getType() const { return "string"; }

void BooleanLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BooleanLiteralNode::dump(int indent) const {
    indentation(indent);
    std::cout << "BooleanLiteralNode (Line " << lineno << "): " << (value ? "true" : "false") << std::endl;
}
std::string BooleanLiteralNode::getType() const { return "bool"; }

void IdentifierNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IdentifierNode::dump(int indent) const {
    indentation(indent);
    std::cout << "IdentifierNode (Line " << lineno << "): " << name << std::endl;
}
std::string IdentifierNode::getType() const {
     return id_type;
 }

void BinaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BinaryExprNode::dump(int indent) const {
    indentation(indent);
    std::cout << "BinaryExprNode (Line " << lineno << "): Op = " << getOpName(op) << std::endl;
    left->dump(indent + 1);
    right->dump(indent + 1);
}
std::string BinaryExprNode::getType() const{
    if(left->getType()==right->getType()){
        return left->getType();
    }
    //std::cout<<"left type: "<<left->getType()<<" right type: "<<right->getType()<<std::endl;
    return "unknown";
}

void UnaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void UnaryExprNode::dump(int indent) const {
    indentation(indent);
    std::cout << "UnaryExprNode (Line " << lineno << "): Op = " << getOpName(op) << std::endl;
    operand->dump(indent + 1);
}
std::string UnaryExprNode::getType() const {
    return operand->getType(); 
}

// --- StatementNode Implementations ---
void BlockStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BlockStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "BlockStatementNode (Line " << lineno << "):" << std::endl;
    for (const auto& stmt : statements) {
        stmt->dump(indent + 1);
    }
}

void EmptyStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void EmptyStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "EmptyStatementNode (Line " << lineno << ")" << std::endl;
}

void AssignmentStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void AssignmentStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "AssignmentStatementNode (Line " << lineno << "): ID = " << id_name << std::endl;
    value->dump(indent + 1);
}

void IncDecStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IncDecStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "IncDecStatementNode (Line " << lineno << "): ID = " << id_name
              << ", " << (is_prefix ? "Prefix" : "Postfix")
              << ", " << (is_increment ? "Increment" : "Decrement") << std::endl;
}

void DisplayStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void DisplayStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "DisplayStatementNode (Line " << lineno << "):" << std::endl;
    expr->dump(indent + 1);
}

void ReadStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ReadStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "ReadStatementNode (Line " << lineno << "): ID = " << id_name << std::endl;
}

void IfStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IfStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "IfStatementNode (Line " << lineno << "):" << std::endl;
    indentation(indent + 1);
    std::cout << "Condition:" << std::endl;
    condition->dump(indent + 2);
    indentation(indent + 1);
    std::cout << "Then Branch:" << std::endl;
    then_branch->dump(indent + 2);
    if (else_branch) {
        indentation(indent + 1);
        std::cout << "Else Branch:" << std::endl;
        else_branch->dump(indent + 2);
    }
}

void WhileStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void WhileStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "WhileStatementNode (Line " << lineno << "):" << std::endl;
    indentation(indent + 1);
    std::cout << "Condition:" << std::endl;
    condition->dump(indent + 2);
    indentation(indent + 1);
    std::cout << "Body:" << std::endl;
    body->dump(indent + 2);
}

void ForStatementNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ForStatementNode::dump(int indent) const {
    indentation(indent);
    std::cout << "ForStatementNode (Line " << lineno << "):" << std::endl;
    if (init) {
        indentation(indent + 1);
        std::cout << "Init:" << std::endl;
        init->dump(indent + 2);
    }
    indentation(indent + 1);
    std::cout << "Condition:" << std::endl;
    condition->dump(indent + 2);
    if (increment) {
        indentation(indent + 1);
        std::cout << "Increment:" << std::endl;
        increment->dump(indent + 2);
    }
    indentation(indent + 1);
    std::cout << "Body:" << std::endl;
    body->dump(indent + 2);
}

// --- ProgramNode Implementation ---
void ProgramNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ProgramNode::dump(int indent) const {
    indentation(indent);
    std::cout << "ProgramNode (Line " << lineno << "):" << std::endl;
    if (statements) {
        statements->dump(indent + 1);
    }
}



// --- Example ASTVisitor (for demonstration/debugging) ---
// This could be in a separate file (e.g., SemanticAnalyzer.h/cpp or ASTPrinter.h/cpp)
class ASTPrinter : public ASTVisitor {
public:
    void visit(ProgramNode& node) override {
        std::cout << "Visiting ProgramNode" << std::endl;
        if (node.statements) {
            node.statements->accept(*this);
        }
    }
    void visit(BlockStatementNode& node) override {
        std::cout << "Visiting BlockStatementNode" << std::endl;
        for (const auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    }
    void visit(EmptyStatementNode& node) override {
        std::cout << "Visiting EmptyStatementNode" << std::endl;
    }
    void visit(IntLiteralNode& node) override {
        std::cout << "Visiting IntLiteralNode: " << node.value << std::endl;
    }
    void visit(FloatLiteralNode& node) override {
        std::cout << "Visiting FloatLiteralNode: " << node.value << std::endl;
    }
    void visit(StringLiteralNode& node) override {
        std::cout << "Visiting StringLiteralNode: \"" << node.value << "\"" << std::endl;
    }
    void visit(BooleanLiteralNode& node) override {
        std::cout << "Visiting BooleanLiteralNode: " << (node.value ? "true" : "false") << std::endl;
    }
    void visit(IdentifierNode& node) override {
        std::cout << "Visiting IdentifierNode: " << node.name << std::endl;
    }
    void visit(BinaryExprNode& node) override {
        std::cout << "Visiting BinaryExprNode (Op: " << getOpName(node.op) << ")" << std::endl;
        node.left->accept(*this);
        node.right->accept(*this);
    }
    void visit(UnaryExprNode& node) override {
        std::cout << "Visiting UnaryExprNode (Op: " << getOpName(node.op) << ")" << std::endl;
        node.operand->accept(*this);
    }
    void visit(AssignmentStatementNode& node) override {
        std::cout << "Visiting AssignmentStatementNode: " << node.id_name << std::endl;
        node.value->accept(*this);
    }
    void visit(IncDecStatementNode& node) override {
        std::cout << "Visiting IncDecStatementNode: " << node.id_name << std::endl;
    }
    void visit(DisplayStatementNode& node) override {
        std::cout << "Visiting DisplayStatementNode" << std::endl;
        node.expr->accept(*this);
    }
    void visit(ReadStatementNode& node) override {
        std::cout << "Visiting ReadStatementNode: " << node.id_name << std::endl;
    }
    void visit(IfStatementNode& node) override {
        std::cout << "Visiting IfStatementNode" << std::endl;
        node.condition->accept(*this);
        node.then_branch->accept(*this);
        if (node.else_branch) {
            node.else_branch->accept(*this);
        }
    }
    void visit(WhileStatementNode& node) override {
        std::cout << "Visiting WhileStatementNode" << std::endl;
        node.condition->accept(*this);
        node.body->accept(*this);
    }
    void visit(ForStatementNode& node) override {
        std::cout << "Visiting ForStatementNode" << std::endl;
        if (node.init) node.init->accept(*this);
        node.condition->accept(*this);
        if (node.increment) node.increment->accept(*this);
        node.body->accept(*this);
    }
};
