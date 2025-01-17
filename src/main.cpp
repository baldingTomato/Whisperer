#include "lingva_api.hpp"
#include "whisperer.hpp"

#ifdef _WIN32
#include "windows_api.hpp"
#include "windows_clipboard_reader.hpp"
#include "windows_listener.hpp"
#elif __linux__
#include "file_input_device.hpp"
#include "linux_listener.hpp"
#include "wl_clipboard_reader.hpp"
#endif

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

#ifdef _WIN32
    auto windowsApi = std::make_unique<WindowsApiImpl>();
    auto reader = std::make_unique<WindowsClipboardReader>();
    auto listener = std::make_unique<WindowsListener>(std::move(reader), std::move(windowsApi));
#elif __linux__
    auto device = std::make_unique<FileInputDevice>();
    auto reader = std::make_unique<WlClipboardReader>();
    auto listener = std::make_unique<LinuxListener>(std::move(device), std::move(reader));
#endif

    auto api = std::make_unique<LingvaAPI>();
    auto translator = std::make_unique<Translator>(std::move(api));

    Whisperer whisperer(std::move(listener), std::move(translator));
    whisperer.startListening();

    return app.exec();
}
