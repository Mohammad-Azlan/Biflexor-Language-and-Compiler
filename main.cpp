#include <iostream>
#include <memory>
#include <cstdio>
#include "parser.h"
#include "ast.h"
#include "semantic_analyzer.h"
#include "ir_generator.h"
#include "ir_executor.h"

// AST root node
extern std::unique_ptr<ProgramNode> ast_root;

// Flex and Bison declarations
extern FILE* yyin;
extern int yyparse(void);

int main(int argc, char* argv[]) {
    // Initialize yyin to stdin by default
    yyin = stdin;

    // If a file is specified, use it as input
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            std::cerr << "Error: Cannot open input file: " << argv[1] << std::endl;
            return 1;
        }

        // Create and run parser
    }

    // Parse input
    int result = yyparse();

    if (result == 0) {
        std::cout << "Parsing OK\n";
        if (ast_root) {
            SemanticAnalyzer semantic_analyzer;
            ast_root->accept(semantic_analyzer);
            ast_root->dump();
            std::cout<<"-----IR-----\n";
            IRGenerator irg;
            ast_root->accept(irg);
            for (const auto& i : irg.instructions)
            std::cout << i.op << " " << i.arg1 << " " << i.arg2 << " " << i.res << "\n";
            std::cout<<"-----Output-----\n";
            IR_Executor executor;
            executor.run(irg.instructions);
        } else {
            std::cout << "No AST produced\n";
        }
    } else {
        std::cerr << "Parsing failed\n";
    }

    // Close input file if we opened one
    if (argc > 1 && yyin) {
        fclose(yyin);
    }

    return result;
}