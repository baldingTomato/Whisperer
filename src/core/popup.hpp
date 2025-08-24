#pragma once
#include <QObject>
#include <QString>

class Popup {
public:
    virtual ~Popup() = default;

    virtual void popWindow(const QString& translatedText) const = 0;

    virtual QObject* toQObject() = 0;  // Helper for casting while drawing the popup
};