# Compiler and flags
CC = gcc
CFLAGS = -Werror -pedantic -I./include

# Directories
SRC_DIR = src
BUILD_DIR = build

# Get all .c files in src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate corresponding .o files in build directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
# Name of the executable
TARGET = bin/program

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile each .c file to .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: all
	./$(TARGET)

# Clean up build files and executable
clear:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all run clear
