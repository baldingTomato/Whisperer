#include "windows_listener.hpp"

#include <windows.h>
#include <chrono>
#include <stdexcept>
#include <thread>

void WindowsListener::simulateCopy() {
    INPUT inputs[4] = {};

    // Press Ctrl
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    // Press C
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'C';

    // Release C
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'C';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    // Release Ctrl
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    windowsApi_->sendInput(4, inputs, sizeof(INPUT));
}

std::string WindowsListener::reactToSelection() {
    simulateCopy();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return reader_->getClipboardText();
}

std::string WindowsListener::listenForShortcut() {
    if (!windowsApi_->registerHotKey(nullptr, 1, MOD_CONTROL, VK_OEM_3)) {  // Ctrl + `
        throw std::runtime_error("Failed to register the hotkey");
    }

    MSG msg = {0};
    try {
        while (windowsApi_->getMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                capturedText_ = reactToSelection();
                windowsApi_->unregisterHotKey(nullptr, 1);  // Unregister after handling
                return capturedText_;
            }
        }
    } catch (...) {
        windowsApi_->unregisterHotKey(nullptr, 1);  // Ensure cleanup on exception
        throw;
    }

    windowsApi_->unregisterHotKey(nullptr, 1);
    return "";
}
