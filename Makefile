# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17

# Files
SOURCES = main.cc parser.cc
OBJECTS = $(SOURCES:.cc=.o)
EXECUTABLE = main

# Additional test files
TEST_SOURCES = parser_test.cc
TEST_OBJECTS = $(TEST_SOURCES:.cc=.o)
TEST_EXECUTABLE = test

# Main target
all: $(EXECUTABLE)

# Main target
$(EXECUTABLE): $(filter-out %_test.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run target
run: $(filter-out %_test.o, $(OBJECTS))
	$(CXX) $(CXXFLAGS) $^ -o $@
	./$(EXECUTABLE)

# Test
$(TEST_EXECUTABLE): $(filter-out main.o, $(OBJECTS)) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lgtest_main -lpthread
	./test

# Compiling
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(EXECUTABLE) $(TEST_EXECUTABLE)

# Phony targets
.PHONY: all clean
