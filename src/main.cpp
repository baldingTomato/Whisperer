#include "lingva_api.hpp"
#include "whisperer.hpp"

#ifdef _WIN32
#include "windows_api.cpp"
#include "windows_clipboard_reader.hpp"
#include "windows_listener.hpp"
#include "windows_popup.hpp"
#elif __linux__
#include "file_input_device.hpp"
#include "linux_listener.hpp"
#include "linux_popup.hpp"
#include "wl_clipboard_reader.hpp"
#endif

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <thread>

void customMessageHandler(QtMsgType type [[maybe_unused]], const QMessageLogContext& context [[maybe_unused]], const QString& msg) {
    if (msg.contains("UpdateLayeredWindowIndirect failed"))
        return;
    QTextStream(stderr) << msg << "\n";
}

int main(int argc, char* argv[]) {
    qInstallMessageHandler(customMessageHandler);
    QApplication app(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);

#ifdef _WIN32
    auto windowsApi = std::make_unique<WindowsApiImpl>();
    auto reader = std::make_unique<WindowsClipboardReader>();
    auto listener = std::make_unique<WindowsListener>(std::move(reader), std::move(windowsApi));
    auto popup = std::make_unique<WindowsPopup>();
#elif __linux__
    auto device = std::make_unique<FileInputDevice>();
    auto reader = std::make_unique<WlClipboardReader>();
    auto listener = std::make_unique<LinuxListener>(std::move(device), std::move(reader));
    auto popup = std::make_unique<LinuxPopup>();
#endif

    auto api = std::make_unique<LingvaAPI>();
    auto translator = std::make_unique<Translator>(std::move(api));

    Whisperer whisperer(std::move(listener), std::move(translator), std::move(popup));

    // Listening loop in a separate thread
    std::thread listenerThread([&whisperer]() {
        whisperer.startListening();
    });

    // The Qt event loop
    int ret = app.exec();

    listenerThread.detach();

    return ret;
}
