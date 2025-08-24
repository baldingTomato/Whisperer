#pragma once

#include <string>

class TranslationAPI {
public:
    virtual ~TranslationAPI() = default;

    virtual std::string requestTranslation(const std::string& text) const = 0;
};
