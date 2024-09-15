#include <iostream>
#include <memory>

#include "file_input_device.hpp"
#include "linux_listener.hpp"

int main() {
    std::unique_ptr<FileInputDevice> device = std::make_unique<FileInputDevice>();
    LinuxListener l(std::move(device));

    l.listenForShortcut();

    return 0;
}
