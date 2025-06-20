# Simple Makefile to build and run all tests in this folder
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD -MP
LDFLAGS = 
BIN_DIR = bin

# Find all test source files ending with _test.cpp
test_srcs := $(wildcard *_test.cpp)
test_bins := $(addprefix $(BIN_DIR)/, $(test_srcs:.cpp=))
deps := $(test_srcs:.cpp=.d)

# Tests that require UNIT_TEST (mocked Serial)
unit_test_bins := $(BIN_DIR)/serialrpc_test

all: $(BIN_DIR) $(test_bins)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/serialrpc_test: serialrpc_test.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -DUNIT_TEST $< -o $@ $(LDFLAGS)

# Default rule for other tests
$(BIN_DIR)/%: %.cpp | $(BIN_DIR)
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
$(BIN_DIR)/device_integrationtest: device_integrationtest.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) device_integrationtest.cpp -o $@ $(LDFLAGS)

.PHONY: integrationtest
integrationtest: $(BIN_DIR)/device_integrationtest
	@if [ -z "$$DEV" ]; then \
		echo "Usage: make integrationtest DEV=/dev/ttyACM0"; \
		exit 1; \
	fi; \
	./$(BIN_DIR)/device_integrationtest $$DEV
