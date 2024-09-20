#pragma once

#include <string>

class Translator {
public:
    Translator() = default;

    std::string translate(const std::string& unknownPhrase);
};
