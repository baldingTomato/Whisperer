#pragma once
#include "../../core/popup.hpp"

class WindowsPopup : public QObject, public Popup {
    Q_OBJECT
public:
    explicit WindowsPopup(QObject* parent = nullptr)
        : QObject(parent) {}

public slots:
    void popWindow(const QString& translatedText) const override;
    QObject* toQObject() override { return this; }
};
