#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "ast.h"
#include "ir.h"
#include <vector>

class IRGenerator : public ASTVisitor {
public:
    std::vector<IRInstruction> instructions;
    int temp_counter = 0;
    int label_counter = 0;

    std::string newTemp();
    std::string newLabel();

    void visit(ProgramNode& node) override; //done
    void visit(BlockStatementNode& node) override; //done
    void visit(IntLiteralNode& node) override; //done
    void visit(FloatLiteralNode& node) override; //done
    void visit(StringLiteralNode& node) override; //done
    void visit(BooleanLiteralNode& node) override; //done
    void visit(IdentifierNode& node) override; //done
    void visit(BinaryExprNode& node) override; //done
    void visit(UnaryExprNode& node) override; //done
    void visit(EmptyStatementNode& node) override;
    void visit(AssignmentStatementNode& node) override; //done
    void visit(IncDecStatementNode& node) override; //done but check
    void visit(DisplayStatementNode& node) override; //done
    void visit(ReadStatementNode& node) override; //done but check
    void visit(IfStatementNode& node) override; //done
    void visit(WhileStatementNode& node) override; //done
    void visit(ForStatementNode& node) override; //done
};

#endif