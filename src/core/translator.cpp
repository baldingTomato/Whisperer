#include "translator.hpp"

#include <cpr/cpr.h>
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>

std::string Translator::translate(const std::string& textToTranslate) const {
    try {
        const std::string translatedText = api_->requestTranslation(textToTranslate);
        const auto jsonResponse = nlohmann::json::parse(translatedText);

        if (jsonResponse.contains("translation")) {
            return jsonResponse.at("translation").get<std::string>();
        }

        throw std::runtime_error("Missing translation key in the JSON response");

    } catch (const std::exception& e) {
        std::cerr << "Translation failed: " << e.what() << std::endl;
        return "";
    }
}
