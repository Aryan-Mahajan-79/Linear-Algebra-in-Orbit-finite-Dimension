# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -Igoogletest/googletest/include

# GoogleTest source
GTEST_DIR = googletest/googletest
GTEST_SRC = $(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJ = gtest-all.o

# Project sources
SRCS = input_validation.cpp input_example.cpp test/test_input_validation.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = run_tests

all: $(TARGET)

$(GTEST_OBJ): $(GTEST_SRC)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -c $< -o $@

$(TARGET): $(OBJS) $(GTEST_OBJ)
	$(CXX) $(CXXFLAGS) -pthread -o $@ $^ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
