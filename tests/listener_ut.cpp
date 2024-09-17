#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/linux_listener.hpp"

using ::testing::_;
using ::testing::Return;

class MockInputDevice : public InputDevice {
public:
    MOCK_METHOD(int, openDevice, (const char* device, int flags), (override));
    MOCK_METHOD(ssize_t, readEvent, (int fd, void* buffer, size_t count), (override));
    MOCK_METHOD(int, closeDevice, (int fd), (override));
};

class MockClipboardReader : public ClipboardReader {
public:
    MOCK_METHOD(std::string, getClipboardText, (), (const, override));
};

class LinuxListenerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockInputDevice> mockDevice;
    std::unique_ptr<MockClipboardReader> mockReader;
    std::unique_ptr<LinuxListener> listener;

    void SetUp() override {
        mockDevice = std::make_unique<MockInputDevice>();
        mockReader = std::make_unique<MockClipboardReader>();
        listener = nullptr;
    }
};

TEST_F(LinuxListenerTest, InitializeSuccess) {
    EXPECT_CALL(*mockDevice, openDevice(_, _))
        .WillOnce(Return(1));

    listener = std::make_unique<LinuxListener>(std::move(mockDevice), std::move(mockReader));

    int fd = listener->initialize();
    EXPECT_EQ(fd, 1);
}

TEST_F(LinuxListenerTest, ReactToSelectionReturnsExpectedString) {
    EXPECT_CALL(*mockReader, getClipboardText())
        .Times(1)
        .WillOnce(Return("Expected Output"));

    listener = std::make_unique<LinuxListener>(std::move(mockDevice), std::move(mockReader));

    std::string selectedText = listener->reactToSelection();
    EXPECT_EQ(selectedText, "Expected Output");
}

TEST_F(LinuxListenerTest, ListenerDetectsShortcut) {
    EXPECT_CALL(*mockReader, getClipboardText())
        .Times(1)
        .WillOnce(Return(""));
    
    EXPECT_CALL(*mockDevice, openDevice(_, _))
        .Times(1)
        .WillOnce(Return(0));

    EXPECT_CALL(*mockDevice, readEvent(_, _, _))
        .WillOnce([](int, void* buffer, size_t) {
            struct input_event event;
            event.type = EV_KEY;
            event.code = KEY_LEFTCTRL;
            event.value = 1;  // Ctrl key pressed
            memcpy(buffer, &event, sizeof(event));
            return sizeof(event);
        })
        .WillOnce([](int, void* buffer, size_t) {
            struct input_event event;
            event.type = EV_KEY;
            event.code = KEY_L;
            event.value = 1;  // 'L' key pressed
            memcpy(buffer, &event, sizeof(event));
            return sizeof(event);
        });

    EXPECT_CALL(*mockDevice, closeDevice(_))
        .Times(1)
        .WillOnce(Return(0));

    listener = std::make_unique<LinuxListener>(std::move(mockDevice), std::move(mockReader));

    EXPECT_TRUE(listener->listenForShortcut());
}
