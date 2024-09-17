#include "input_device.hpp"

class FileInputDevice : public InputDevice {
public:
    int openDevice(const char* device, int flags) override;
    ssize_t readEvent(int fd, void* buffer, size_t count) override;
    int closeDevice(int fd) override;
};
