#pragma once

#include "popup.hpp"

class LinuxPopup : public Popup {
    virtual void popWindow(const std::string& translatedText) override;
};
