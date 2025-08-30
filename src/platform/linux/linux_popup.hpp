#pragma once
#include "../../core/popup.hpp"

class QLabel;
class QWidget;
class QString;

class LinuxPopup : public QObject, public Popup {
    Q_OBJECT
public:
    explicit LinuxPopup(QObject* parent = nullptr)
        : QObject(parent) {}

    static constexpr int FONT_SIZE_PT = 12;
    static constexpr int LABEL_MARGIN = 10;
    static constexpr int BASE_DURATION_MS = 3000;
    static constexpr int EXTRA_MS_PER_WORD = 300;
    static constexpr int MAX_DURATION_MS = 10000;

public slots:
    void popWindow(const QString& translatedText) const override;
    QObject* toQObject() override { return this; }

private:
    void configureLabel(QLabel* label) const;
    QWidget* createPopupWidget(const QString& text) const;
    int calculateDisplayDuration(const QString& text) const;
};
