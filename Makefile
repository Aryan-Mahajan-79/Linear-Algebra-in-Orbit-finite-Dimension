# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -Isrc -Iinclude -Igoogletest/googletest/include


# GoogleTest source
GTEST_DIR = googletest/googletest
GTEST_SRC = $(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJ = build/gtest-all.o

# Project sources
SRCS = \
	src/input_validation.cpp \
	src/input_data.cpp \
	src/pattern_key.cpp \
	src/logger.cpp \
	src/matrix_reduction.cpp \
	src/solving_linear_equations.cpp \
	test/test_input_validation.cpp \
	test/test_matrix_reduction.cpp \
	test/test_solving_linear_equations.cpp
OBJS = $(addprefix build/, $(notdir $(SRCS:.cpp=.o)))

TARGET = run_tests

# Default rule
all: build $(TARGET)

# Create build directory if it doesn't exist
build:
	mkdir -p build

program: build/program

build/program: src/main.cpp src/input_data.cpp src/pattern_key.cpp src/input_validation.cpp src/logger.cpp src/matrix_reduction.cpp src/solving_linear_equations.cpp
	$(CXX) $(CXXFLAGS) -Iinclude $^ -o $@

# Build GoogleTest object
$(GTEST_OBJ): $(GTEST_SRC) | build
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -c $< -o $@

# Compile project sources to object files in build/
# Rule for compiling source files from src/
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for compiling test files from test/
build/%.o: test/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Link everything into the final executable
$(TARGET): $(OBJS) $(GTEST_OBJ)
	$(CXX) $(CXXFLAGS) -pthread -o $@ $^

# Cleanup
clean:
	rm -rf build $(TARGET)
