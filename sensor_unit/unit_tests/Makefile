#Compiler and flags
CXX := gcc
CXXFLAGS := -std=c17 -Wall


# Directories
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

HEADERS := $(wildcard $(INC_DIR)/*.h)

# Default target is "out"
TARGET := out

# Compile and link
$(TARGET): $(OBJS)
	rm -f $(TARGET)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) $^ -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(dir $<) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean

