#include "windows_listener.hpp"

#include <windows.h>
#include <stdexcept>

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

    SendInput(4, inputs, sizeof(INPUT));
}

std::string WindowsListener::reactToSelection() {
    simulateCopy();

    return reader_->getClipboardText();
}

std::string WindowsListener::listenForShortcut() {
    if (!RegisterHotKey(nullptr, 1, MOD_CONTROL, VK_OEM_3)) {  // Ctrl + `
        throw std::runtime_error("Failed to register the hotkey");
    }

    MSG msg = {0};
    try {
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                capturedText_ = reactToSelection();
                UnregisterHotKey(nullptr, 1);  // Unregister after handling
                return capturedText_;
            }
        }
    } catch (...) {
        UnregisterHotKey(nullptr, 1);  // Ensure cleanup on exception
        throw;
    }

    UnregisterHotKey(nullptr, 1);
    return "";
}
