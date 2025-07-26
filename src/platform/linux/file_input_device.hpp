#pragma once

#include "input_device.hpp"

class FileInputDevice : public InputDevice {
public:
    virtual int openDevice(const char* device, int flags) override;
    virtual ssize_t readEvent(int fd, void* buffer, size_t count) override;
    virtual int closeDevice(int fd) override;
};
