#ifndef semantic_analyzer_h
#define semantic_analyzer_h

#include <unordered_map>
#include <string>
#include "ast.h"

class SemanticAnalyzer : public ASTVisitor {
private:
    std::unordered_map<std::string, std::string> symbol_table; // var: type mapping

public:
    SemanticAnalyzer();

    void visit(ProgramNode& node) override;
    void visit(BlockStatementNode& node) override;
    void visit(EmptyStatementNode& node) override;
    void visit(IntLiteralNode& node) override;
    void visit(FloatLiteralNode& node) override;
    void visit(StringLiteralNode& node) override;
    void visit(BooleanLiteralNode& node) override;
    void visit(IdentifierNode& node) override;
    void visit(BinaryExprNode& node) override;
    void visit(UnaryExprNode& node) override;
    void visit(AssignmentStatementNode& node) override;
    void visit(IncDecStatementNode& node) override;
    void visit(DisplayStatementNode& node) override;
    void visit(ReadStatementNode& node) override;
    void visit(IfStatementNode& node) override;
    void visit(WhileStatementNode& node) override;
    void visit(ForStatementNode& node) override;
};

#endif