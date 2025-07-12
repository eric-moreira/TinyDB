# TinyDB Makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/tinydb

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Header files
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Link the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful! Executable: $@"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
	@rm -f *.db
	@echo "Clean complete."

# Clean and rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	@echo "Running TinyDB..."
	@./$(TARGET)

# Run with valgrind for memory checking
valgrind: $(TARGET)
	@echo "Running TinyDB with Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Debug build with extra flags
debug: CFLAGS += -DDEBUG -O0
debug: clean all

# Release build with optimizations
release: CFLAGS += -O2 -DNDEBUG
release: clean all

# Test the database file
test: $(TARGET)
	@echo "Testing TinyDB..."
	@./$(TARGET)
	@if [ -f test.db ]; then \
		echo "Database file created successfully!"; \
		echo "File size: $$(stat -f%z test.db 2>/dev/null || stat -c%s test.db 2>/dev/null) bytes"; \
		echo "First 64 bytes (hex):"; \
		hexdump -C test.db | head -4; \
	else \
		echo "❌ Database file not created!"; \
	fi

# Install dependencies (if any)
install-deps:
	@echo "No external dependencies required for TinyDB."

# Show file structure
info:
	@echo "=== TinyDB Project Structure ==="
	@tree . 2>/dev/null || find . -type f -name "*.c" -o -name "*.h" -o -name "Makefile" | sort
	@echo ""
	@echo "=== Build Configuration ==="
	@echo "Compiler: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Source files: $(SOURCES)"
	@echo "Target: $(TARGET)"

# Help target
help:
	@echo "TinyDB Makefile - Available targets:"
	@echo ""
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove build files and databases"
	@echo "  rebuild    - Clean and build"
	@echo "  run        - Build and run the program"
	@echo "  test       - Build, run, and inspect database file"
	@echo "  valgrind   - Run with memory leak detection"
	@echo "  debug      - Build with debug symbols (no optimization)"
	@echo "  release    - Build optimized release version"
	@echo "  info       - Show project structure and build config"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make           # Build the project"
	@echo "  make run       # Build and run"
	@echo "  make test      # Test database creation"
	@echo "  make valgrind  # Check for memory leaks"

# Declare phony targets
.PHONY: all clean rebuild run valgrind debug release test install-deps info help directories