CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
BUILDDIR = build
SRCDIR = src
TESTDIR = test

SRC = $(SRCDIR)/main.cpp $(SRCDIR)/board.cpp $(SRCDIR)/engine.cpp
OBJ = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SRC))

# Automatically find all test .cpp files
TESTSOURCES = $(wildcard $(TESTDIR)/*.cpp)

# Make a binary for each test file (e.g., test/unmakeTest.cpp → build/unmakeTest)
TESTBINS = $(patsubst $(TESTDIR)/%.cpp, $(BUILDDIR)/%, $(TESTSOURCES))

# Default target
all: $(BUILDDIR)/main

# Build all test binaries
tests: $(TESTBINS)

# Compile regular main program
$(BUILDDIR)/main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile each test .cpp to its own binary
# Links with board.o and engine.o
$(BUILDDIR)/%: $(TESTDIR)/%.cpp $(BUILDDIR)/src/board.o $(BUILDDIR)/src/engine.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Make sure build dirs exist
build:
	mkdir -p $(BUILDDIR)/src $(BUILDDIR)/test

clean:
	rm -rf $(BUILDDIR)
