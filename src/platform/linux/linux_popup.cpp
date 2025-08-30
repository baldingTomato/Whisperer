#include "linux_popup.hpp"

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QScreen>
#include <QRegularExpression>
#include <QTimer>
#include <QWidget>

void LinuxPopup::popWindow(const QString& translatedText) const {
    QWidget* popup = createPopupWidget(translatedText);
    int displayDurationMs = calculateDisplayDuration(translatedText);
    popup->show();
    QTimer::singleShot(displayDurationMs, popup, &QWidget::close);
}

QWidget* LinuxPopup::createPopupWidget(const QString& text) const {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QLabel* label = new QLabel(text, popup);
    configureLabel(label);

    return popup;
}

void LinuxPopup::configureLabel(QLabel* label) const {
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(QStringLiteral("QLabel { font-size: %1pt; }").arg(FONT_SIZE_PT));
    label->setWordWrap(true);
    label->setContentsMargins(LABEL_MARGIN, LABEL_MARGIN, LABEL_MARGIN, LABEL_MARGIN);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label->adjustSize();
}

int LinuxPopup::calculateDisplayDuration(const QString& text) const {
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    int duration = BASE_DURATION_MS + (words.size() * EXTRA_MS_PER_WORD);
    return std::min(duration, MAX_DURATION_MS);
}
