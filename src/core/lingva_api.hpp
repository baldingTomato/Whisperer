#include <cpr/cpr.h>
#include "translation_api.hpp"

class LingvaAPI : public TranslationAPI {
public:
    virtual std::string requestTranslation(const std::string& text) const override;
};
