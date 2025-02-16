#include "linux_popup.hpp"

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#include <QWidget>

void LinuxPopup::popWindow(const std::string& translatedText) {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QLabel* label = new QLabel(QString::fromStdString(translatedText), popup);
    label->setAlignment(Qt::AlignCenter);

    int marginSize = 10;
    label->setContentsMargins(marginSize, marginSize, marginSize, marginSize);

    // Flexible size policy to avoid an oversized window
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label->adjustSize();

    popup->show();

    QTimer::singleShot(3000, popup, &QWidget::close);
}
