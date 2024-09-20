#include "whisperer.hpp"

#include <iostream>

void Whisperer::startListening() {
    std::string textToTranslate = listener_->listenForShortcut();

    if (!textToTranslate.empty()) {
        std::string translatedText = translator_->translate(textToTranslate);
        std::cout << "Original: \"" << textToTranslate << "\", translated: \"" << translatedText << "\"" << std::endl;
        return;

    } else {
        std::cout << "Something went wrong with capturing the text or translating it!" << std::endl;
        return;
    }
}
