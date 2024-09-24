#pragma once

#include "listener.hpp"
#include "translator.hpp"

#include <memory>

class Whisperer {
    std::unique_ptr<Listener> listener_;
    std::unique_ptr<Translator> translator_;

public:
    Whisperer(std::unique_ptr<Listener> listener, std::unique_ptr<Translator> translator)
        : listener_(std::move(listener)), translator_(std::move(translator)) {};

    void startListening();
    void displayTranslationPopup(const std::string& translatedText);
};
