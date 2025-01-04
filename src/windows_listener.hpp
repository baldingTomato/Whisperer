#pragma once

#include "clipboard_reader.hpp"
#include "listener.hpp"
#include "windows_api.hpp"

#include <memory>

class WindowsListener : public Listener {
    std::string capturedText_ = "";
    std::unique_ptr<ClipboardReader> reader_;
    std::unique_ptr<WindowsApi> windowsApi_;

public:
    WindowsListener(std::unique_ptr<ClipboardReader> reader, std::unique_ptr<WindowsApi> windowsApi)
        : reader_(std::move(reader)), windowsApi_(std::move(windowsApi)){};

    virtual std::string reactToSelection() override;
    virtual std::string listenForShortcut() override;

    void simulateCopy();
};
