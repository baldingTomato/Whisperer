#pragma once

#include "translation_api.hpp"

class LingvaAPI : public TranslationAPI {
    static constexpr int HTTP_OK = 200;

public:
    virtual std::string requestTranslation(const std::string& text) const override;
};
