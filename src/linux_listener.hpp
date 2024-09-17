#pragma once

#include "input_device.hpp"
#include "listener.hpp"
#include "clipboard_reader.hpp"

#include <iostream>
#include <memory>
#include <string>

class LinuxListener : public Listener {
    std::string capturedText_ = "";
    std::unique_ptr<InputDevice> device_;
    std::unique_ptr<ClipboardReader> reader_;

public:
    LinuxListener(std::unique_ptr<InputDevice> device, std::unique_ptr<ClipboardReader> reader)
        : device_(std::move(device)), reader_(std::move(reader)) {};

    int initialize();

    virtual std::string reactToSelection() override;
    virtual bool listenForShortcut() override;
};
