# ===============================
# Makefile for cproxy project
# ===============================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Folders
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Executable name
TARGET = $(BUILD_DIR)/cproxy

# Source files (all .c in src/)
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Object files (.o)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Link objects into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^

# Compile each .c file into .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: all
	./$(TARGET)

# Phony targets (not real files)
.PHONY: all clean run