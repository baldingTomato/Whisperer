#include "lingva_api.hpp"

std::string LingvaAPI::requestTranslation(const std::string& text) {
    std::string url = "https://lingva.ml/api/v1/en/pl/" + cpr::util::urlEncode(text);
    cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code == 200) {
        return r.text;
    } else {
        throw std::runtime_error("Lingva API request failed with status code " + std::to_string(r.status_code));
    }
}
