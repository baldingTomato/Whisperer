#include "whisperer.hpp"

#include <QCoreApplication>
#include <QMetaObject>
#include <iostream>
#include <thread>

void Whisperer::startListening() {
    while (keepListening_) {
        std::string textToTranslate = listener_->listenForShortcut();

        if (textToTranslate == "AURELIA") {
            keepListening_ = false;
            std::cout << "Exit hotkey pressed. Exiting listener loop.\n";
            QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
            break;
        }

        if (!textToTranslate.empty()) {
            std::string translatedText = translator_->translate(textToTranslate);

            if (!translatedText.empty()) {
                std::cout << "Original: " << textToTranslate
                          << "\nTranslated: " << translatedText << std::endl;
                displayTranslationPopup(translatedText);
            } else {
                std::cout << "Translation failed!" << std::endl;
            }
        } else {
            std::cout << "Something went wrong with capturing the text!" << std::endl;
        }

        // Sleep a little to avoid CPU overuse
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Whisperer::displayTranslationPopup(const std::string& translatedText) const {
    QObject* popupObject = popup_->toQObject();
    if (popupObject) {
        QMetaObject::invokeMethod(
            popupObject,
            "popWindow",
            Qt::QueuedConnection,
            Q_ARG(QString, QString::fromStdString(translatedText)));
    } else {
        std::cerr << "Error: Popup::toQObject() returned nullptr!" << std::endl;
    }
}
