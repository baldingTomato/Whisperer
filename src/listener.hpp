#pragma once

#include <string>

class Listener {
public:
    virtual ~Listener() = default;

    virtual std::string reactToSelection() = 0;
    virtual bool listenForShortcut() = 0;
};
