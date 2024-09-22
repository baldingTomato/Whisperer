#include "whisperer.hpp"
#include "linux_listener.hpp"
#include "file_input_device.hpp"
#include "wl_clipboard_reader.hpp"
#include "lingva_api.hpp"

#include <iostream>

int main() {
    std::unique_ptr<FileInputDevice> device = std::make_unique<FileInputDevice>();
    std::unique_ptr<WlClipboardReader> reader = std::make_unique<WlClipboardReader>();

    std::unique_ptr<LingvaAPI> api = std::make_unique<LingvaAPI>();

    std::unique_ptr<LinuxListener> listener = std::make_unique<LinuxListener>(std::move(device), std::move(reader));
    std::unique_ptr<Translator> translator = std::make_unique<Translator>(std::move(api));

    Whisperer whisperer = Whisperer(std::move(listener), std::move(translator));

    whisperer.startListening();

    return 0;
}
