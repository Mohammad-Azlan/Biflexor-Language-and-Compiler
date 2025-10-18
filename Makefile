# =========================================================
# 📦 Makefile for Biflexor Compiler
# Author: Mohammad Azlan
# =========================================================

# ---- Compiler Settings ----
CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra -O2

# ---- Tools ----
LEX       = flex
YACC      = bison

# ---- Project Name ----
TARGET    = biflexor

# ---- Source Files ----
SRCS = main.cpp \
       ast.cpp \
       ir_generator.cpp \
       ir_executor.cpp \
       semantic_analyzer.cpp

OBJS = $(SRCS:.cpp=.o)

# ---- Lexer and Parser Files ----
LEX_FILE   = lexer.l
YACC_FILE  = parser.y
LEX_GEN    = lex.yy.cc
YACC_GEN   = parser.tab.cc
YACC_HDR   = parser.tab.h

# =========================================================
# 🏗️ Default Build
# =========================================================

all: $(TARGET)

$(TARGET): $(YACC_GEN) $(LEX_GEN) $(OBJS)
	@echo "🔗 Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(YACC_GEN) $(LEX_GEN)
	@echo "✅ Build complete: ./$(TARGET)"

# =========================================================
# 🧠 Parser and Lexer Rules
# =========================================================

$(YACC_GEN): $(YACC_FILE)
	@echo "🧩 Generating parser..."
	$(YACC) -d -v $(YACC_FILE) -o $(YACC_GEN) --defines=$(YACC_HDR)

$(LEX_GEN): $(LEX_FILE)
	@echo "⚙️  Generating lexer..."
	$(LEX) -o $(LEX_GEN) $(LEX_FILE)

# =========================================================
# 🧹 Utility Targets
# =========================================================

clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(OBJS) $(LEX_GEN) $(YACC_GEN) $(YACC_HDR) parser.output $(TARGET)
	@echo "✨ Clean complete."

run: all
	@echo "🚀 Running test.bfx..."
	./$(TARGET) test.bfx

.PHONY: all clean run
