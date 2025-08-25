#include "windows_clipboard_reader.hpp"

#include <stdexcept>
#include <windows.h>

std::string WindowsClipboardReader::getClipboardText() const {
    if (!OpenClipboard(nullptr)) {
        throw std::runtime_error("Failed to open clipboard!");
    }

    std::string selectedText;

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        throw std::runtime_error("No clipboard data available after reacting to selection!");
    }

    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        throw std::runtime_error("Failed to lock clipboard data");
    }

    selectedText = pszText;

    GlobalUnlock(hData);
    CloseClipboard();

    return selectedText;
}
