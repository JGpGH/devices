# Simple Makefile to build and run all tests in this folder
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = 

# Find all test source files ending with _test.cpp
test_srcs := $(wildcard *_test.cpp)
test_bins := $(test_srcs:.cpp=)

all: $(test_bins)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

run: all
	@for t in $(test_bins); do \
		echo "Running $$t..."; \
		./$$t; \
	done

clean:
	rm -f $(test_bins)
