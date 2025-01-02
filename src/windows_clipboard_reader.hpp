#pragma once

#include "clipboard_reader.hpp"

class WindowsClipboardReader : public ClipboardReader {
public:
    std::string getClipboardText() const override;
};
