# Simple Makefile to build and run all tests in this folder
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD -MP
LDFLAGS = 

# Find all test source files ending with _test.cpp
test_srcs := $(wildcard *_test.cpp)
test_bins := $(test_srcs:.cpp=)
deps := $(test_srcs:.cpp=.d)

# Tests that require UNIT_TEST (mocked Serial)
unit_test_bins := serialrpc_test

all: $(test_bins)

serialrpc_test: serialrpc_test.cpp
	$(CXX) $(CXXFLAGS) -DUNIT_TEST $< -o $@ $(LDFLAGS)

# Default rule for other tests
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

-include $(deps)

run: all
	@for t in $(test_bins); do \
		echo "Running $$t..."; \
		./$$t; \
	done

clean:
	rm -f $(test_bins) *.d

# Device integration test (not run by default)
device_integrationtest: device_integrationtest.cpp
	$(CXX) $(CXXFLAGS) device_integrationtest.cpp -o device_integrationtest $(LDFLAGS)

.PHONY: integrationtest
integrationtest: device_integrationtest
	@if [ -z "$$DEV" ]; then \
		echo "Usage: make integrationtest DEV=/dev/ttyACM0"; \
		exit 1; \
	fi; \
	./device_integrationtest $$DEV
