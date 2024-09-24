#include "whisperer.hpp"
#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QWidget>
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

void Whisperer::displayTranslationPopup(const std::string& translatedText) {
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
}
