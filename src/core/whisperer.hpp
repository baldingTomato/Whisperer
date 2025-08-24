#pragma once

#include "listener.hpp"
#include "popup.hpp"
#include "translator.hpp"

#include <atomic>
#include <memory>

class Whisperer {
    std::unique_ptr<Listener> listener_;
    std::unique_ptr<Translator> translator_;
    std::unique_ptr<Popup> popup_;
    std::atomic<bool> keepListening_{ true };

public:
    Whisperer(std::unique_ptr<Listener> listener, std::unique_ptr<Translator> translator, std::unique_ptr<Popup> popup)
        : listener_(std::move(listener)), translator_(std::move(translator)), popup_(std::move(popup)){};

    void startListening();
    void displayTranslationPopup(const std::string& translatedText) const;
};
