#include "whisperer.hpp"
#include <QApplication>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include <QDebug>
#include <iostream>

void Whisperer::startListening() {
    std::string textToTranslate = listener_->listenForShortcut();

    if (!textToTranslate.empty()) {
        std::string translatedText = translator_->translate(textToTranslate);

        if (!translatedText.empty()) {
            std::cout << "Original: " << textToTranslate << "\nTranslated: " << translatedText << std::endl;
            displayTranslationPopup(translatedText);
        } else {
            std::cout << "Translation failed!" << std::endl;
        }
    } else {
        std::cout << "Something went wrong with capturing the text!" << std::endl;
    }
}

/* void Whisperer::displayTranslationPopup(const std::string& translatedText) {
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // popup->setAttribute(Qt::WA_TranslucentBackground);

    QLabel* label = new QLabel(QString::fromStdString(translatedText), popup);
    label->setAlignment(Qt::AlignCenter);

    // Add margins around the text inside QLabel (left, top, right, bottom)
    int marginSize = 10;
    label->setContentsMargins(marginSize, marginSize, marginSize, marginSize);

    // Flexible size policy to avoid an oversized window
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label->adjustSize();

    // Position
    // int x = 500;
    // int y = 300;
    // popup->setGeometry(x, y, label->width(), label->height());

    popup->show();

    // Automatically close the popup
    QTimer::singleShot(3000, popup, &QWidget::close);
} */

void Whisperer::displayTranslationPopup(const std::string& translatedText) {
    // Create the popup window
    QWidget* popup = new QWidget();
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    popup->setAttribute(Qt::WA_TranslucentBackground); // Enable translucent background
    popup->setStyleSheet(
        "QWidget {"
        "    background-color: rgba(60, 110, 60, 200);" // Semi-transparent dark gray
        "    border: 3px solid rgba(30, 190, 50, 235);" // Greenish border
        "    border-radius: 20px;"                    // Rounded corners
        "}"
    );

    // Create and style the label
    QLabel* label = new QLabel(QString::fromStdString(translatedText), popup);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "QLabel {"
        "    color: white;"                         // White text
        "    font-size: 14pt;"                      // Adjust font size
        "    font-family: 'Montserrat', 'Tahoma', 'Arial';"    // Font style
        "    padding: 15px;"                        // Add padding inside the popup
        "}"
    );

    // Add shadow effect
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(popup);
    shadowEffect->setBlurRadius(15);                 // Soft shadow
    shadowEffect->setOffset(0, 4);                   // Vertical offset
    shadowEffect->setColor(QColor(0, 0, 0, 150));    // Semi-transparent black
    popup->setGraphicsEffect(shadowEffect);

    // Adjust size of the popup to fit the label
    label->adjustSize();
    int width = label->width() + 30;  // Add padding to the width
    int height = label->height() + 30; // Add padding to the height
    popup->resize(width, height);

    // Position the popup at the center of the primary screen
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width) / 2;
    int y = (screenGeometry.height() - height) / 2;
    popup->move(x, y);

    // Show the popup
    popup->show();
    //qDebug() << "Available fonts:" << QFontDatabase().families();

    // Automatically close the popup after 3 seconds
    QTimer::singleShot(3000, popup, &QWidget::close);
}

