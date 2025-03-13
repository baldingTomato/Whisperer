#include "linux_listener.hpp"

#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <stdexcept>

std::string LinuxListener::reactToSelection() {
    return reader_->getClipboardText();
}

int LinuxListener::initialize() {
    int fd = device_->openDevice("/dev/input/event4", O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open input device: " << strerror(errno) << std::endl;
        return -1;
    }

    return fd;
}

std::string LinuxListener::listenForShortcut() {
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

                // Check for Ctrl + ` and Ctrl + q
                if (ie.code == KEY_GRAVE && ie.value == 1 && ctrl_pressed) {
                    capturedText_ = reactToSelection();
                    device_->closeDevice(fileDescriptor);
                    return capturedText_;
                } else if (ie.code == KEY_Q && ie.value == 1 && ctrl_pressed) {
                    device_->closeDevice(fileDescriptor);
                    return "AURELIA";  // Special signal indicating exit
                }
            }
        } else {
            std::cerr << "Error reading from input device: " << strerror(errno) << std::endl;
            return "";
        }
    }

    device_->closeDevice(fileDescriptor);
    return "";
}
