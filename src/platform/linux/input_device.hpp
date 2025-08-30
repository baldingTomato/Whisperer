#pragma once

#include <sys/types.h>

class InputDevice {
public:
    virtual ~InputDevice() = default;

    virtual int openDevice(const char* device, int flags) = 0;
    virtual ssize_t readEvent(int fd, void* buffer, size_t count) = 0;
    virtual int closeDevice(int fd) = 0;
};
