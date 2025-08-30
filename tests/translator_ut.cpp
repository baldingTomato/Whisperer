#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/core/translator.hpp"

using ::testing::_;
using ::testing::Return;

class MockTranslationAPI : public TranslationAPI {
public:
    MOCK_METHOD(std::string, requestTranslation, (const std::string&), (const override));
};

class TranslatorTest : public ::testing::Test {
protected:
    std::unique_ptr<Translator> translator;
    std::unique_ptr<MockTranslationAPI> mockAPI;

    void SetUp() override {
        mockAPI = std::make_unique<MockTranslationAPI>();
        translator = nullptr;
    }
};

TEST_F(TranslatorTest, TranslateGivenString) {
    EXPECT_CALL(*mockAPI, requestTranslation("Random text"))
        .Times(1)
        .WillOnce(Return("{\"translation\":\"Losowy tekst\"}"));

    translator = std::make_unique<Translator>(std::move(mockAPI));

    std::string translatedText = translator->translate("Random text");
    EXPECT_EQ("Losowy tekst", translatedText);
}
