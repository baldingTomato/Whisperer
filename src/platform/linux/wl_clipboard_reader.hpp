#pragma once

#include "../../core/clipboard_reader.hpp"

class WlClipboardReader : public ClipboardReader {
public:
    virtual std::string getClipboardText() const override;
};
