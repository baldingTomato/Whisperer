#pragma once

#include "../../core/clipboard_reader.hpp"

class WindowsClipboardReader : public ClipboardReader {
public:
    std::string getClipboardText() const override;
};
