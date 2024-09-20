#include <gtest/gtest.h>

#include "../src/translator.hpp"

class TranslatorTest : public ::testing::Test {
protected:
    std::unique_ptr<Translator> translator;

    void SetUp() override {
        translator = std::make_unique<Translator>();
    }
};

TEST_F(TranslatorTest, TranslateGivenString) {
    std::string translatedText = translator->translate("Random text");
    EXPECT_EQ("Ok", translatedText);
}
