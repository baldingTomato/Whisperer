#include "windows_popup.hpp"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#include <QWidget>


void WindowsPopup::popWindow(const QString& translatedText) {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    popup->setAttribute(Qt::WA_TranslucentBackground);
    popup->setStyleSheet(
        "QWidget {"
        "    background-color: rgba(60, 110, 60, 200);"
        "    border: 3px solid rgba(30, 190, 50, 235);"
        "    border-radius: 20px;"
        "}");

    QLabel* label = new QLabel(translatedText, popup);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font-size: 14pt;"
        "    font-family: 'Montserrat', 'Tahoma', 'Arial';"
        "    padding: 15px;"
        "}");

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(popup);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(0, 4);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    popup->setGraphicsEffect(shadowEffect);

    label->adjustSize();
    int width = label->width() + 30;
    int height = label->height() + 30;
    popup->resize(width, height);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width) / 2;
    int y = (screenGeometry.height() - height) / 2;
    popup->move(x, y);

    popup->show();

    QTimer::singleShot(3000, popup, &QWidget::close);
}
