#pragma once
#include "../../core/popup.hpp"

class QLabel;
class QWidget;
class QString;

class WindowsPopup : public QObject, public Popup {
    Q_OBJECT
public:
    explicit WindowsPopup(QObject* parent = nullptr)
        : QObject(parent) {}

    static constexpr int FONT_SIZE_PT = 13;
    static constexpr int BORDER_RADIUS_PX = 17;
    static constexpr int SHADOW_BLUR_RADIUS = 20;
    static constexpr int SHADOW_OFFSET_Y = 6;
    static constexpr int POPUP_DURATION_MS = 3000;

public slots:
    void popWindow(const QString& translatedText) const override;
    QObject* toQObject() override { return this; }

private:
    QLabel* createLabel(const QString& text, QWidget* parent) const;
    QWidget* createContainer(QLabel* label, QWidget* parent) const;
    void applyDropShadow(QWidget* container, QWidget* parent) const;
    void centerPopup(QWidget* popup) const;
};
