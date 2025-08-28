#include "wl_clipboard_reader.hpp"

#include <array>
#include <memory>
#include <stdexcept>

std::string WlClipboardReader::getClipboardText() const {
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, int (*)(FILE*)> pipe(popen("wl-paste --primary", "r"), pclose);

    if (pipe == nullptr) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}
