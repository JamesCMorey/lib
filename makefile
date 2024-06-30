# Local directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
LIB_DIR := lib
TEST_DIR := test

# Lists of local files
SRCS := $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
LOCAL_HEADER_FILES := $(wildcard $(INCLUDE_DIR)/*.h)
LOCAL_LIB_FILES := $(patsubst $(SRC_DIR)/%.c, $(LIB_DIR)/lib%.a, $(SRCS))

# System directories
HEADER_INSTALL_DIR := /usr/local/include
LIB_INSTALL_DIR := /usr/local/lib

# List of system files that this makefile compiles and installs
SYSTEM_FILES := $(patsubst $(INCLUDE_DIR)/%.h, $(HEADER_INSTALL_DIR)/%.h, $(LOCAL_HEADER_FILES)) \
		$(patsubst $(LIB_DIR)/%.a, $(LIB_INSTALL_DIR)/%.a, $(LOCAL_LIB_FILES))

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2

# Default target
all: install_headers install_libs

# Install header files to system directory
install_headers:
	sudo cp $(LOCAL_HEADER_FILES) $(HEADER_INSTALL_DIR)

# Install library files to system directory
install_libs: $(LOCAL_LIB_FILES)
	sudo cp $^ $(LIB_INSTALL_DIR)

# Compile library (.a) file from object files
$(LIB_DIR)/lib%.a: $(BUILD_DIR)/%.o
	@mkdir -p $(LIB_DIR)
	ar -cr $@ $<

# Compile .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

#TODO: Create rules for compiling tests
# Run tests on library files
test: all $(TEST_OBJS)

$(TEST_DIR)/%.c: 

# uninstall everything moved into system directories
uninstall:
	sudo rm -f $(SYSTEM_FILES)

# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

.PHONY: all clean install
