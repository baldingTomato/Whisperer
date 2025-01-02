#pragma once

#include "clipboard_reader.hpp"
#include "listener.hpp"

#include <memory>

class WindowsListener : public Listener {
    std::string capturedText_ = "";
    std::unique_ptr<ClipboardReader> reader_;

public:
    WindowsListener(std::unique_ptr<ClipboardReader> reader)
        : reader_(std::move(reader)) {};

    virtual std::string reactToSelection() override;
    virtual std::string listenForShortcut() override;

    void simulateCopy();
};
