#include "whisperer.hpp"

#include <iostream>

void Whisperer::startListening() {
    std::string textToTranslate = listener_->listenForShortcut();

    if (!textToTranslate.empty()) {
        std::string translatedText = translator_->translate(textToTranslate);

        if (!translatedText.empty()) {
            std::cout << "Original: " << textToTranslate << "\nTranslated: " << translatedText << std::endl;
        } else {
            std::cout << "Translation failed!" << std::endl;
        }

        return;

    } else {
        std::cout << "Something went wrong with capturing the text!" << std::endl;
        return;
    }
}
