#pragma once

#include "input_device.hpp"
#include "listener.hpp"

#include <iostream>
#include <memory>
#include <string>

class LinuxListener : public Listener {
    std::string capturedText_ = "";
    std::unique_ptr<InputDevice> device_;

public:
    LinuxListener(std::unique_ptr<InputDevice> device)
        : device_(std::move(device)) {};

    int initialize();

    virtual std::string reactToSelection() override;
    virtual bool listenForShortcut() override;
};
