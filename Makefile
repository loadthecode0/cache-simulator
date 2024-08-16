# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra --pedantic -std=c++11

# Target executable name
EXEC = cacheSim

# Source files
SRCS = block.cpp set.cpp dll.cpp cacheSim.cpp cache.cpp

# Default target
# all: $(EXEC)

all: $(EXEC)

# Compiling and linking the executable in one step
all:
	$(CXX) $(CXXFLAGS) block.cpp set.cpp dll.cpp cacheSim.cpp cache.cpp -o $(EXEC)

# Clean target
clean:
	rm -f $(EXEC)

test:
	./cacheSim 256 4 16 write-allocate write-back lru < gcc.trace
	./cacheSim 256 4 16 write-allocate write-back fifo < gcc.trace
	./cacheSim 256 4 16 write-allocate write-through lru < gcc.trace
	./cacheSim 256 4 16 write-allocate write-through fifo < gcc.trace
	./cacheSim 256 4 16 no-write-allocate write-through fifo < gcc.trace
	./cacheSim 256 4 16 no-write-allocate write-through lru < gcc.trace

# Phony targets
.PHONY: all cacheSim clean
