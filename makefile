# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Paths
SRCDIR = src
LIBDIR = libs/tinyrenderer
BUILDDIR = build

# Files
TARGET = main
MAIN_SRC = $(SRCDIR)/main.cpp
TGA_SRC = $(LIBDIR)/tgaimage.cpp
MODEL_SRC = $(LIBDIR)/model.cpp
MAIN_OBJ = $(BUILDDIR)/main.o
TGA_OBJ = $(BUILDDIR)/tgaimage.o
MODEL_OBJ = $(BUILDDIR)/model.o

# Rules
.PHONY: all clean tga main

# Default rule: Build the program
all: $(TARGET)

# Build the main executable
$(TARGET): $(MAIN_OBJ) $(TGA_OBJ) $(MODEL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile main.cpp into main.o
main: $(MAIN_OBJ)

$(MAIN_OBJ): $(MAIN_SRC)
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile tgaimage.cpp into tgaimage.o
tga: $(TGA_OBJ)

$(TGA_OBJ): $(TGA_SRC)
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

model: $(MODEL_OBJ)

$(MODEL_OBJ): $(MODEL_SRC)
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean all generated files
clean:
	rm -rf $(BUILDDIR) $(TARGET)
