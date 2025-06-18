# Simple Makefile to build and run all tests in this folder
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = 

# Find all test source files ending with _test.cpp
test_srcs := $(wildcard *_test.cpp)
test_bins := $(test_srcs:.cpp=)

# Tests that require UNIT_TEST (mocked Serial)
unit_test_bins := serialrpc_test

all: $(test_bins)

serialrpc_test: serialrpc_test.cpp
	$(CXX) $(CXXFLAGS) -DUNIT_TEST $< -o $@ $(LDFLAGS)

# Default rule for other tests
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

run: all
	@for t in $(test_bins); do \
		echo "Running $$t..."; \
		./$$t; \
	done

clean:
	rm -f $(test_bins)
