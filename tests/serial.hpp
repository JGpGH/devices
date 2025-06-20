#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>

class SerialClient {
    int fd;
public:
    SerialClient(const std::string& device, int baudrate = B115200) {
        fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
        if (fd < 0) throw std::runtime_error("Failed to open serial port");
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) throw std::runtime_error("tcgetattr failed");
        cfsetospeed(&tty, baudrate);
        cfsetispeed(&tty, baudrate);
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
        tty.c_iflag &= ~IGNBRK;
        tty.c_lflag = 0;
        tty.c_oflag = 0;
        tty.c_cc[VMIN]  = 0;
        tty.c_cc[VTIME] = 10; // 1s timeout
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_cflag |= (CLOCAL | CREAD);
        tty.c_cflag &= ~(PARENB | PARODD);
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;
        if (tcsetattr(fd, TCSANOW, &tty) != 0) throw std::runtime_error("tcsetattr failed");
    }
    ~SerialClient() { if (fd >= 0) close(fd); }
    void write_bytes(const std::vector<uint8_t>& data) {
        size_t written = 0;
        while (written < data.size()) {
            ssize_t n = ::write(fd, data.data() + written, data.size() - written);
            if (n < 0) throw std::runtime_error("Serial write failed");
            written += n;
        }
    }
    std::vector<uint8_t> read_bytes(size_t n) {
        std::vector<uint8_t> buf(n);
        size_t got = 0;
        while (got < n) {
            ssize_t r = ::read(fd, buf.data() + got, n - got);
            if (r < 0) throw std::runtime_error("Serial read failed");
            if (r == 0) break;
            got += r;
        }
        buf.resize(got);
        return buf;
    }
    int get_fd() const { return fd; }
};

#endif // SERIAL_HPP
