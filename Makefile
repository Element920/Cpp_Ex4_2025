#israelmor555@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
MAIN_TARGET = demo
TEST_TARGET = test_exec
MAIN_SRC = Demo.cpp
TEST_SRC = Test.cpp
DEPS = MyContainer.hpp

# Default target
all: $(MAIN_TARGET) $(TEST_TARGET)

# Build main demo
$(MAIN_TARGET): $(MAIN_SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_SRC)

# Build test executable
$(TEST_TARGET): $(TEST_SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SRC)

# Target required by README: make Main
Main: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Target required by README: make test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Target required by README: make valgrind
valgrind: $(MAIN_TARGET) $(TEST_TARGET)
	@echo "Running valgrind on main demo..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(MAIN_TARGET)
	@echo ""
	@echo "Running valgrind on tests..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TEST_TARGET)

# Run demo without building (if already built)
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Run tests without building (if already built)
run-test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Debug builds
debug: CXXFLAGS += -DDEBUG -O0
debug: $(MAIN_TARGET) $(TEST_TARGET)

# Release builds
release: CXXFLAGS += -O3 -DNDEBUG
release: $(MAIN_TARGET) $(TEST_TARGET)

# Target required by README: make clean
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) *.o *.gch *~ core

# Additional helpful targets
help:
	@echo "Available targets:"
	@echo "  all       - Build both demo and test executables"
	@echo "  Main      - Run the main demo (builds if needed)"
	@echo "  test      - Run the test suite (builds if needed)"
	@echo "  valgrind  - Run valgrind memory check on both executables"
	@echo "  clean     - Remove all generated files"
	@echo "  debug     - Build with debug flags"
	@echo "  release   - Build with optimization flags"
	@echo "  help      - Show this help message"

.PHONY: all Main test valgrind run run-test clean debug release help