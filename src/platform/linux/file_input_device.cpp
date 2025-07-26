#include "file_input_device.hpp"

#include <fcntl.h>
#include <unistd.h>

int FileInputDevice::openDevice(const char* device, int flags) {
    return open(device, flags);
}

ssize_t FileInputDevice::readEvent(int fd, void* buffer, size_t count) {
    return read(fd, buffer, count);
}

int FileInputDevice::closeDevice(int fd) {
    return close(fd);
}
