#include "translator.hpp"

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

std::string Translator::translate(const std::string& unknownPhrase) {
    try {
        std::string translatedPhrase = api_->requestTranslation(unknownPhrase);
        auto jsonResponse = nlohmann::json::parse(translatedPhrase);

        if (jsonResponse.contains("translation")) {
            return jsonResponse["translation"].get<std::string>();
        } else {
            throw std::runtime_error("Missing translation key in the JSON response");
        }
    } catch (const std::exception& e) {
        std::cerr << "Translation failed: " << e.what() << std::endl;
        return "";
    }
}
