#include "linux_listener.hpp"

#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <array>
#include <memory>
#include <stdexcept>

std::string LinuxListener::reactToSelection() {
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, int (*)(FILE*)> pipe(popen("wl-paste --primary", "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

int LinuxListener::initialize() {
    int fd = device_->openDevice("/dev/input/event3", O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open input device: " << strerror(errno) << std::endl;
        return -1;
    }

    return fd;
}

bool LinuxListener::listenForShortcut() {
    int fileDescriptor = initialize();
    struct input_event ie;
    bool ctrl_pressed = false;

    while (true) {
        if (device_->readEvent(fileDescriptor, &ie, sizeof(ie)) > 0) {
            if (ie.type == EV_KEY) {
                // Track the state of the Ctrl key
                if (ie.code == KEY_LEFTCTRL || ie.code == KEY_RIGHTCTRL) {
                    ctrl_pressed = ie.value;  // 1 for press, 0 for release
                }

                // Check for Ctrl + L
                if (ie.code == KEY_L && ie.value == 1 && ctrl_pressed) {
                    capturedText_ = reactToSelection();
                    device_->closeDevice(fileDescriptor);
                    return true;
                }
            }
        } else {
            std::cerr << "Error reading from input device: " << strerror(errno) << std::endl;
            return false;
        }
    }

    device_->closeDevice(fileDescriptor);
    return false;
}
