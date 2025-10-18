%{
#include <iostream>
#include <string>
#include <memory>
#include "ast.h"
#include "parser.h"

// Forward declarations
extern int yylineno;
extern char* yytext;
int yylex();
void yyerror(const char* s);

// Error handling function
void yyerror(const char* s) {
    std::cerr << "Parse error at line " << yylineno << ": " << s << std::endl;
}

// AST root node
extern std::unique_ptr<ProgramNode> ast_root;
%}

%code requires {
    #include "ast.h"
}

%union {
    int ival;
    float fval;
    bool bval;
    char *sval;
    ExprNode* exprval;
    StatementNode* stmtval;
    BlockStatementNode* blockval;
    ProgramNode* programval;
}

// Token types
%token <sval> ID
%token <sval> STRING_LITERAL
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <bval> TRUE FALSE

// Keywords and operators
%token IF ELSE FOR WHILE DISPLAY READ
%token EQ NEQ LEQ GEQ LT GT AND OR NOT
%token PLUS MINUS MULT DIV ASSIGN
%token INCREMENT DECREMENT
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON

// Non-terminal types
%type <programval> program
%type <blockval> block_statement statements
%type <stmtval> statement matched_statement unmatched_statement assignment_statement input_output_statement iterative_statement
%type <stmtval> inc_dec_statement
%type <exprval> expr

// Operator precedence
%right ASSIGN
%nonassoc IF ELSE
%left OR
%left AND
%right NOT
%nonassoc EQ NEQ LEQ GEQ LT GT
%left PLUS MINUS
%left MULT DIV
%right INCREMENT DECREMENT

%%

program:
    statements {
        ProgramNode* prog = new ProgramNode(yylineno);
        prog->statements.reset($1);
        ast_root.reset(prog);
        $$ = prog;
    }
;

statements:
    /* empty */ { 
        $$ = new BlockStatementNode(yylineno); 
    }
|   statements statement {
        $1->addStatement(std::unique_ptr<StatementNode>($2));
        $$ = $1;
    }
;

statement:
    matched_statement   { $$ = $1; }
|   unmatched_statement { $$ = $1; }
;

matched_statement:
    block_statement { $$ = $1; }
|   iterative_statement { $$ = $1; }
|   input_output_statement { $$ = $1; }
|   assignment_statement { $$ = $1; }
|   inc_dec_statement { $$ = $1; }
|   IF LPAREN expr RPAREN matched_statement ELSE matched_statement {
        $$ = new IfStatementNode(
            std::unique_ptr<ExprNode>($3),
            std::unique_ptr<StatementNode>($5),
            std::unique_ptr<StatementNode>($7),
            yylineno
        );
    }
;

block_statement:
    LBRACE statements RBRACE { $$ = $2; }
;



iterative_statement:
    /*FOR LPAREN for_init_stmt SEMICOLON expr SEMICOLON for_increment_stmt RPAREN matched_statement {
        $$ = new ForStatementNode(
            std::unique_ptr<StatementNode>($3),
            std::unique_ptr<ExprNode>($5),
            std::unique_ptr<StatementNode>($7),
            std::unique_ptr<StatementNode>($9),
            yylineno
        );
    }*/
    FOR LPAREN assignment_statement expr SEMICOLON matched_statement RPAREN matched_statement {
        $$ = new ForStatementNode(
            std::unique_ptr<StatementNode>($3),
            std::unique_ptr<ExprNode>($4),
            std::unique_ptr<StatementNode>($6),
            std::unique_ptr<StatementNode>($8),
            yylineno
        );
    }
|   WHILE LPAREN expr RPAREN matched_statement {
        $$ = new WhileStatementNode(
            std::unique_ptr<ExprNode>($3),
            std::unique_ptr<StatementNode>($5),
            yylineno
        );
    }
;

input_output_statement:
    DISPLAY LPAREN expr RPAREN SEMICOLON {
        $$ = new DisplayStatementNode(
            std::unique_ptr<ExprNode>($3),
            yylineno
        );
    }
|   READ LPAREN ID RPAREN SEMICOLON {
        $$ = new ReadStatementNode($3, yylineno);
        free($3);
    }
;

assignment_statement:
    ID ASSIGN expr SEMICOLON {
        $$ = new AssignmentStatementNode($1, std::unique_ptr<ExprNode>($3), yylineno);
        free($1);
    }
;

inc_dec_statement:
    ID INCREMENT SEMICOLON { 
        $$ = new IncDecStatementNode($1, false, true, yylineno);
        free($1);
    }
|   ID DECREMENT SEMICOLON { 
        $$ = new IncDecStatementNode($1, false, false, yylineno);
        free($1);
    }
|   INCREMENT ID SEMICOLON { 
        $$ = new IncDecStatementNode($2, true, true, yylineno);
        free($2);
    }
|   DECREMENT ID SEMICOLON { 
        $$ = new IncDecStatementNode($2, true, false, yylineno);
        free($2);
    }
;

unmatched_statement:
    IF LPAREN expr RPAREN statement {
        $$ = new IfStatementNode(
            std::unique_ptr<ExprNode>($3),
            std::unique_ptr<StatementNode>($5),
            nullptr,
            yylineno
        );
    }
|   IF LPAREN expr RPAREN matched_statement ELSE unmatched_statement {
        $$ = new IfStatementNode(
            std::unique_ptr<ExprNode>($3),
            std::unique_ptr<StatementNode>($5),
            std::unique_ptr<StatementNode>($7),
            yylineno
        );
    }
;

expr:
    expr OR expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            OR,
            yylineno
        );
    }
|   expr AND expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            AND,
            yylineno
        );
    }
|   NOT expr {
        $$ = new UnaryExprNode(
            std::unique_ptr<ExprNode>($2),
            NOT,
            yylineno
        );
    }
|   expr EQ expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            EQ,
            yylineno
        );
    }
|   expr NEQ expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            NEQ,
            yylineno
        );
    }
|   expr LEQ expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            LEQ,
            yylineno
        );
    }
|   expr GEQ expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            GEQ,
            yylineno
        );
    }
|   expr LT expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            LT,
            yylineno
        );
    }
|   expr GT expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            GT,
            yylineno
        );
    }
|   expr PLUS expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            PLUS,
            yylineno
        );
    }
|   expr MINUS expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            MINUS,
            yylineno
        );
    }
|   expr MULT expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            MULT,
            yylineno
        );
    }
|   expr DIV expr {
        $$ = new BinaryExprNode(
            std::unique_ptr<ExprNode>($1),
            std::unique_ptr<ExprNode>($3),
            DIV,
            yylineno
        );
    }
|   LPAREN expr RPAREN { $$ = $2; }
|   ID { $$ = new IdentifierNode($1, yylineno); free($1); }
|   INT_LITERAL { $$ = new IntLiteralNode($1, yylineno); }
|   FLOAT_LITERAL { $$ = new FloatLiteralNode($1, yylineno); }
|   STRING_LITERAL { $$ = new StringLiteralNode($1, yylineno); free($1); }
|   TRUE { $$ = new BooleanLiteralNode(true, yylineno); }
|   FALSE { $$ = new BooleanLiteralNode(false, yylineno); }
;

%%
