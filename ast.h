#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>   // For std::unique_ptr
#include <iostream> // For basic output (e.g., for debugging print)
// REMOVED: using namespace std; // Avoid using namespace std in headers

class ASTVisitor;

// Base class for all AST nodes
class ASTNode {
public:
    int lineno; // Line number from source code for error reporting

    // Constructor
    ASTNode(int line) : lineno(line) {}

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~ASTNode() = default;

    // Pure virtual method for the Visitor pattern
    virtual void accept(ASTVisitor& visitor) = 0;

    // Optional: A virtual method for debugging/printing the AST structure
    virtual void dump(int indent = 0) const = 0;
};

// --- Expression Nodes ---
class ExprNode : public ASTNode {
public:
    std::string tmp_name;
    ExprNode(int line) : ASTNode(line) {}
    virtual std::string getType() const=0;
};

class IntLiteralNode : public ExprNode {
public:
    int value;
    IntLiteralNode(int val, int line) : ExprNode(line), value(val) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override;
};

class FloatLiteralNode : public ExprNode {
public:
    float value; // Changed to float as per your parser.y
    FloatLiteralNode(float val, int line) : ExprNode(line), value(val) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override; 
};

class StringLiteralNode : public ExprNode {
public:
    std::string value;
    StringLiteralNode(const std::string& val, int line) : ExprNode(line), value(val) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override; 
};

class BooleanLiteralNode : public ExprNode {
public:
    bool value;
    BooleanLiteralNode(bool val, int line) : ExprNode(line), value(val) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override; 
};

class IdentifierNode : public ExprNode {
public:
    std::string name;
    std::string id_type;
    IdentifierNode(const std::string& id_name, int line) : ExprNode(line), name(id_name) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override;
};

class BinaryExprNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    int op; // Store the token type (PLUS, MINUS, EQ, etc.)
    // REMOVED: vector<string> op_arr; // Moved to ast.cpp
    BinaryExprNode(std::unique_ptr<ExprNode> l, std::unique_ptr<ExprNode> r, int operator_token, int line)
        : ExprNode(line), left(std::move(l)), right(std::move(r)), op(operator_token) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override;
};

class UnaryExprNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> operand;
    int op; // Store the token type (NOT, MINUS for unary negation)
    // REMOVED: vector<string> uop_arr; // Moved to ast.cpp
    UnaryExprNode(std::unique_ptr<ExprNode> opd, int operator_token, int line)
        : ExprNode(line), operand(std::move(opd)), op(operator_token) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
    std::string getType() const override;
};

// --- Statement Nodes ---
class StatementNode : public ASTNode {
public:
    StatementNode(int line) : ASTNode(line) {}
};

class BlockStatementNode : public StatementNode {
public:
    std::vector<std::unique_ptr<StatementNode>> statements;
    BlockStatementNode(int line) : StatementNode(line) {}
    void addStatement(std::unique_ptr<StatementNode> stmt) { // Corrected addstatement to addStatement
        statements.push_back(std::move(stmt));
    }
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class EmptyStatementNode : public StatementNode {
public:
    EmptyStatementNode(int line) : StatementNode(line) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class AssignmentStatementNode : public StatementNode {
public:
    std::string id_name; // Renamed from 'id' to 'id_name' for consistency
    std::unique_ptr<ExprNode> value;
    AssignmentStatementNode(const std::string& name, std::unique_ptr<ExprNode> val, int line)
        : StatementNode(line), id_name(name), value(std::move(val)) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class IncDecStatementNode : public StatementNode {
public:
    std::string id_name; // Renamed from 'id' to 'id_name' for consistency
    bool is_prefix;    // true for ++x, --x; false for x++, x--
    bool is_increment; // true for ++, false for --
    IncDecStatementNode(const std::string& name, bool prefix, bool increment, int line)
        : StatementNode(line), id_name(name), is_prefix(prefix), is_increment(increment) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class DisplayStatementNode : public StatementNode {
public:
    std::unique_ptr<ExprNode> expr;
    DisplayStatementNode(std::unique_ptr<ExprNode> e, int line)
        : StatementNode(line), expr(std::move(e)) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class ReadStatementNode : public StatementNode {
public:
    std::string id_name; // Renamed from 'id' to 'id_name' for consistency
    ReadStatementNode(const std::string& name, int line) // Changed type to const std::string&
        : StatementNode(line), id_name(name) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class IfStatementNode : public StatementNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StatementNode> then_branch;
    std::unique_ptr<StatementNode> else_branch; // Can be nullptr
    IfStatementNode(std::unique_ptr<ExprNode> cond, std::unique_ptr<StatementNode> then_b,
                    std::unique_ptr<StatementNode> else_b, int line)
        : StatementNode(line), condition(std::move(cond)), then_branch(std::move(then_b)),
          else_branch(std::move(else_b)) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class WhileStatementNode : public StatementNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StatementNode> body;
    WhileStatementNode(std::unique_ptr<ExprNode> cond, std::unique_ptr<StatementNode> b, int line)
        : StatementNode(line), condition(std::move(cond)), body(std::move(b)) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

class ForStatementNode : public StatementNode {
public:
    std::unique_ptr<StatementNode> init;
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StatementNode> increment; // CRITICAL: Changed from IncDecStatementNode to StatementNode
    std::unique_ptr<StatementNode> body;
    ForStatementNode(std::unique_ptr<StatementNode> i, std::unique_ptr<ExprNode> cond,
                     std::unique_ptr<StatementNode> inc, std::unique_ptr<StatementNode> b, int line)
        : StatementNode(line), init(std::move(i)), condition(std::move(cond)),
          increment(std::move(inc)), body(std::move(b)) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

// Program Node (Root of the AST)
class ProgramNode : public ASTNode {
public:
    std::unique_ptr<BlockStatementNode> statements; // The main block of statements
    explicit ProgramNode(int line)
        : ASTNode(line), statements(nullptr) {}
    
    ProgramNode(std::unique_ptr<BlockStatementNode> stmts, int line)
        : ASTNode(line), statements(std::move(stmts)) {}
    void accept(ASTVisitor& visitor) override;
    void dump(int indent = 0) const override;
};

// --- AST Visitor Base Class (for traversing the AST) ---
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    // Visit methods for each concrete AST node type
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(BlockStatementNode& node) = 0;
    virtual void visit(EmptyStatementNode& node) = 0;
    virtual void visit(IntLiteralNode& node) = 0;
    virtual void visit(FloatLiteralNode& node) = 0;
    virtual void visit(StringLiteralNode& node) = 0;
    virtual void visit(BooleanLiteralNode& node) = 0;
    virtual void visit(IdentifierNode& node) = 0;
    virtual void visit(BinaryExprNode& node) = 0;
    virtual void visit(UnaryExprNode& node) = 0;
    virtual void visit(AssignmentStatementNode& node) = 0; // Corrected typo: removed extra 'void'
    virtual void visit(IncDecStatementNode& node) = 0;
    virtual void visit(DisplayStatementNode& node) = 0;
    virtual void visit(ReadStatementNode& node) = 0;
    virtual void visit(IfStatementNode& node) = 0;
    virtual void visit(WhileStatementNode& node) = 0;
    virtual void visit(ForStatementNode& node) = 0;
};


// Global root of the AST (will be populated by the parser)
extern std::unique_ptr<ProgramNode> ast_root;

#endif // AST_H
