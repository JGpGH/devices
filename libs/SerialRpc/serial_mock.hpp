#ifndef SERIAL_MOCK_HPP
#define SERIAL_MOCK_HPP
#include <vector>
#include <queue>
#include <cstdint>
#include <cstring>

class SerialMock {
public:
    std::vector<uint8_t> written; // All bytes written out
    std::queue<uint8_t> to_read;  // Bytes to be read by Serial.readBytes

    void clear() {
        written.clear();
        std::queue<uint8_t> empty;
        std::swap(to_read, empty);
    }

    // Simulate Serial.write
    size_t write(const uint8_t* buf, size_t len) {
        written.insert(written.end(), buf, buf + len);
        return len;
    }
    size_t write(uint8_t b) {
        written.push_back(b);
        return 1;
    }

    // Simulate Serial.readBytes
    size_t readBytes(uint8_t* buf, size_t len) {
        size_t n = 0;
        while (n < len && !to_read.empty()) {
            buf[n++] = to_read.front();
            to_read.pop();
        }
        return n;
    }

    // Simulate Serial.available
    size_t available() const {
        return to_read.size();
    }

    // Helper: push bytes to be read
    void push_bytes(const uint8_t* buf, size_t len) {
        for (size_t i = 0; i < len; ++i) to_read.push(buf[i]);
    }
};

#ifdef UNIT_TEST
extern SerialMock Serial;
#endif

#endif // SERIAL_MOCK_HPP
