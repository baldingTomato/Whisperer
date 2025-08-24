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

void WindowsPopup::popWindow(const QString& translatedText) const {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    popup->setAttribute(Qt::WA_TranslucentBackground);

    QWidget* container = new QWidget(popup);
    container->setObjectName("popupContainer");
    
    QLabel* label = new QLabel(translatedText, container);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font-size: 13pt;"
        "    font-family: 'Montserrat', 'Tahoma', 'Arial';"
        "    padding: 8px 16px;"
        "}");

    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addWidget(label);
    layout->setContentsMargins(16, 12, 16, 12);

    container->setStyleSheet(
        "#popupContainer {"
        "    background-color: rgb(40, 20, 50);"  // solid dark background
        "    border: 2px solid rgba(255, 182, 193, 180);"  // light pink border
        "    border-radius: 17px;"
        "}");

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(popup);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setOffset(0, 6);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    container->setGraphicsEffect(shadowEffect);

    container->adjustSize();
    popup->resize(container->sizeHint());

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - popup->width()) / 2;
    int y = (screenGeometry.height() - popup->height()) / 2;
    popup->move(x, y);

    popup->show();

    QTimer::singleShot(3000, popup, &QWidget::close);
}

