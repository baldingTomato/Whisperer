#include "windows_popup.hpp"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

QLabel* WindowsPopup::createLabel(const QString& text, QWidget* parent) const {
    QLabel* label = new QLabel(text, parent);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setStyleSheet(QString(
        "QLabel {"
        "    color: white;"
        "    font-size: %1pt;"
        "    font-family: 'Montserrat', 'Tahoma', 'Arial';"
        "    padding: 8px 16px;"
        "}").arg(FONT_SIZE_PT));
    return label;
}

QWidget* WindowsPopup::createContainer(QLabel* label, QWidget* parent) const {
    QWidget* container = new QWidget(parent);
    container->setObjectName("popupContainer");

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addWidget(label);
    layout->setContentsMargins(16, 12, 16, 12);

    container->setStyleSheet(QString(
        "#popupContainer {"
        "    background-color: rgb(40, 20, 50);"
        "    border: 2px solid rgba(255, 182, 193, 180);"
        "    border-radius: %1px;"
        "}").arg(BORDER_RADIUS_PX));

    return container;
}

void WindowsPopup::applyDropShadow(QWidget* container, QWidget* parent) const {
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(parent);
    shadowEffect->setBlurRadius(SHADOW_BLUR_RADIUS);
    shadowEffect->setOffset(0, SHADOW_OFFSET_Y);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    container->setGraphicsEffect(shadowEffect);
}

void WindowsPopup::centerPopup(QWidget* popup) const {
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - popup->width()) / 2;
    int y = (screenGeometry.height() - popup->height()) / 2;
    popup->move(x, y);
}

void WindowsPopup::popWindow(const QString& translatedText) const {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    popup->setAttribute(Qt::WA_TranslucentBackground);

    QLabel* label = createLabel(translatedText, popup);
    QWidget* container = createContainer(label, popup);
    applyDropShadow(container, popup);

    container->adjustSize();
    popup->resize(container->sizeHint());

    centerPopup(popup);
    popup->show();

    QTimer::singleShot(POPUP_DURATION_MS, popup, &QWidget::close);
}

