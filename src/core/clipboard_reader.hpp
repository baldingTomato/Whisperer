#pragma once

#include <string>

class ClipboardReader {
public:
    virtual ~ClipboardReader() = default;
    
    virtual std::string getClipboardText() const = 0;
};
