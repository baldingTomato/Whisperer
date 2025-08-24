#pragma once

#include "translation_api.hpp"

#include <string>
#include <memory>

class Translator {
    std::unique_ptr<TranslationAPI> api_;

public:
    Translator(std::unique_ptr<TranslationAPI> api) : api_(std::move(api)) {}

    std::string translate(const std::string& unknownPhrase) const;
};
