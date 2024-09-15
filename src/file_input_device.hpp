#include "input_device.hpp"

#include <fcntl.h>
#include <unistd.h>

class FileInputDevice : public InputDevice {
public:
    int openDevice(const char* device, int flags) override {
        return open(device, flags);
    }

    ssize_t readEvent(int fd, void* buffer, size_t count) override {
        return read(fd, buffer, count);
    }

    int closeDevice(int fd) override {
        return close(fd);
    }
};
