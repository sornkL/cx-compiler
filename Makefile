# Compiler to use
CXX := g++

# Flags to pass to the compiler
CXXFLAGS := -std=c++17 -Wall -Wextra

# Include directories
INCLUDE_DIRS := -I./src

# Source files to compile
SRCS := ./src/lexer/Lexer.cpp \
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
