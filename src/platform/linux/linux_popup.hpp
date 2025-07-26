#pragma once
#include "../../core/popup.hpp"

class LinuxPopup : public QObject, public Popup {
    Q_OBJECT
public:
    explicit LinuxPopup(QObject* parent = nullptr)
        : QObject(parent) {}

public slots:
    void popWindow(const QString& translatedText) override;
    QObject* toQObject() override { return this; }
};