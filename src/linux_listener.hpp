#pragma once

#include <iostream>
#include <string>

#include "listener.hpp"

class LinuxListener : public Listener {
    std::string capturedText_ = "";

public:
    LinuxListener() = default;

    virtual std::string reactToSelection() override;
    virtual bool listenForShortcut() override;
};
