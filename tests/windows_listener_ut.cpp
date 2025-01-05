#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/windows_listener.hpp"

using ::testing::_;
using ::testing::Return;

class MockClipboardReader : public ClipboardReader {
public:
    MOCK_METHOD(std::string, getClipboardText, (), (const, override));
};

class MockWindowsApi : public WindowsApi {
public:
    MOCK_METHOD(bool, registerHotKey, (HWND hWnd, int id, UINT fsModifiers, UINT vk), (const, override));
    MOCK_METHOD(bool, unregisterHotKey, (HWND hWnd, int id), (const, override));
    MOCK_METHOD(UINT, sendInput, (UINT cInputs, LPINPUT pInputs, int cbSize), (const, override));
    MOCK_METHOD(bool, getMessage, (LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax), (const, override));
};

class WindowsListenerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockClipboardReader> mockReader;
    std::unique_ptr<MockWindowsApi> mockWindowsApi;
    std::unique_ptr<WindowsListener> listener;

    void SetUp() override {
        mockReader = std::make_unique<MockClipboardReader>();
        mockWindowsApi = std::make_unique<MockWindowsApi>();
        listener = nullptr;
    }
};

TEST_F(WindowsListenerTest, ReactToSelectionReturnsClipboardText) {
    EXPECT_CALL(*mockReader, getClipboardText())
        .WillOnce(Return("Expected Clipboard Text"));

    EXPECT_CALL(*mockWindowsApi, sendInput(_, _, _))
        .Times(1);

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    std::string selectedText = listener->reactToSelection();
    EXPECT_EQ(selectedText, "Expected Clipboard Text");
}

TEST_F(WindowsListenerTest, ListenForShortcutDetectsHotkey) {
    EXPECT_CALL(*mockWindowsApi, registerHotKey(_, 1, MOD_CONTROL, VK_OEM_3))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockWindowsApi, getMessage(_, nullptr, 0, 0))
    .WillOnce([](LPMSG msg, HWND, UINT, UINT) {
        if (msg != nullptr) {
            msg->message = WM_HOTKEY;
            msg->wParam = 1;  // Set wParam to match the registered hotkey ID
        }
        return true;
    });

    EXPECT_CALL(*mockWindowsApi, sendInput(_, _, _))
        .Times(1);

    EXPECT_CALL(*mockWindowsApi, unregisterHotKey(_, 1))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockReader, getClipboardText())
        .WillOnce(Return("Expected Clipboard Text"));

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    std::string capturedText = listener->listenForShortcut();
    EXPECT_EQ(capturedText, "Expected Clipboard Text");
}

TEST_F(WindowsListenerTest, SimulateCopySendsCorrectInput) {
    EXPECT_CALL(*mockWindowsApi, sendInput(_, _, _))
        .WillOnce([](UINT cInputs, LPINPUT pInputs, [[maybe_unused]] int cbSize) {
            EXPECT_EQ(cInputs, 4);
            EXPECT_EQ(pInputs[0].ki.wVk, VK_CONTROL);
            EXPECT_EQ(pInputs[1].ki.wVk, 'C');
            return true;
        });

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    listener->simulateCopy();
}

TEST_F(WindowsListenerTest, ReactToSelectionReadsEmptyString) {
    EXPECT_CALL(*mockReader, getClipboardText())
        .WillOnce(Return(""));

    EXPECT_CALL(*mockWindowsApi, sendInput(_, _, _))
        .Times(1);

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    std::string selectedText = listener->reactToSelection();
    EXPECT_EQ(selectedText, "");
}

TEST_F(WindowsListenerTest, ThrowsExceptionWhenHotKeyFailsToRegister) {
    EXPECT_CALL(*mockWindowsApi, registerHotKey(_, 1, MOD_CONTROL, VK_OEM_3))
        .WillOnce(Return(false));

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    EXPECT_THROW({
        listener->listenForShortcut();
    }, std::runtime_error);
}

TEST_F(WindowsListenerTest, DoesNotUnregisterHotKeyWhenRegistrationFails) {
    EXPECT_CALL(*mockWindowsApi, registerHotKey(_, 1, MOD_CONTROL, VK_OEM_3))
        .WillOnce(Return(false));

    EXPECT_CALL(*mockWindowsApi, unregisterHotKey(_, 1))
        .Times(0);

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    EXPECT_THROW({
        listener->listenForShortcut();
    }, std::runtime_error);
}

TEST_F(WindowsListenerTest, UnregistersHotKeyOnGetMessageException) {
    EXPECT_CALL(*mockWindowsApi, registerHotKey(_, 1, MOD_CONTROL, VK_OEM_3))
        .WillOnce(Return(true));

    EXPECT_CALL(*mockWindowsApi, getMessage(_, nullptr, 0, 0))
        .WillOnce(::testing::Invoke([](LPMSG, HWND, UINT, UINT) -> bool {
            throw std::runtime_error("Unexpected error during message retrieval");
        }));

    EXPECT_CALL(*mockWindowsApi, unregisterHotKey(_, 1))
        .WillOnce(Return(true));

    listener = std::make_unique<WindowsListener>(std::move(mockReader), std::move(mockWindowsApi));

    EXPECT_THROW({
        listener->listenForShortcut();
    }, std::runtime_error);
}

