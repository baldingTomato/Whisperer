#include "lingva_api.hpp"

#include <cpr/cpr.h>
#include <stdexcept>

std::string LingvaAPI::requestTranslation(const std::string& text) const {
    const std::string url = "https://lingva.ml/api/v1/en/pl/" + cpr::util::urlEncode(text);
    const cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code == HTTP_OK) {
        return r.text;
    }

    throw std::runtime_error("Lingva API request failed with status code " + std::to_string(r.status_code));
}
