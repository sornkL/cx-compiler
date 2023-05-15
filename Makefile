# Compiler to use
CXX := clang++

# Flags to pass to the compiler
CXXFLAGS := -std=c++17 -g -Wall -Wextra \
			`llvm-config --cxxflags --ldflags --system-libs --libs core orcjit native`

# Include directories
INCLUDE_DIRS := -I./src

# Source files to compile
SRCS := ./src/lexer/Lexer.cpp \
		./src/utils/Logger.cpp \
		./src/cx/cx.cpp \
		./src/ast/BinaryExprAST.cpp \
		./src/ast/NumberExprAST.cpp \
		./src/ast/PrototypeAST.cpp \
		./src/ast/FunctionAST.cpp \
		./src/parser/Parser.cpp \
        ./test.cpp

# Object files to build
OBJS := $(SRCS:.cpp=.o)

# Name of the output executable
TARGET := test.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
