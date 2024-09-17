#include <iostream>
#include <memory>

#include "file_input_device.hpp"
#include "wl_clipboard_reader.hpp"
#include "linux_listener.hpp"

int main() {
    std::unique_ptr<FileInputDevice> device = std::make_unique<FileInputDevice>();
    std::unique_ptr<WlClipboardReader> reader = std::make_unique<WlClipboardReader>();
    LinuxListener l(std::move(device), std::move(reader));

    l.listenForShortcut();

    return 0;
}
