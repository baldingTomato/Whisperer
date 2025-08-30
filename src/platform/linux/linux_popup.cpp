#include "linux_popup.hpp"

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QScreen>
#include <QRegularExpression>
#include <QTimer>
#include <QWidget>

void LinuxPopup::popWindow(const QString& translatedText) const {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QLabel* label = new QLabel(translatedText, popup);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "QLabel {"
        "    font-size: 12pt;"
        "}");

    label->setWordWrap(true);

    int marginSize = 10;
    label->setContentsMargins(marginSize, marginSize, marginSize, marginSize);

    // Flexible size policy to avoid an oversized window
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label->adjustSize();

    // ---- DYNAMIC DISPLAY DURATION ----
    // 3 seconds base + 300ms per word, capped at 10 seconds total
    int baseDurationMs = 3000;
    QStringList words = translatedText.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    int wordCount = words.size();
    int extraMsPerWord = 300;
    int displayDurationMs = baseDurationMs + (wordCount * extraMsPerWord);

    if (displayDurationMs > 10000) {
        displayDurationMs = 10000;
    }

    popup->show();

    QTimer::singleShot(displayDurationMs, popup, &QWidget::close);
}
