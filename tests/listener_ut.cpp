#include <X11/keysym.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "../src/input_device.hpp"
#include "../src/linux_listener.hpp"

using ::testing::_;
using ::testing::Return;

class MockInputDevice : public InputDevice {
public:
    MOCK_METHOD(int, openDevice, (const char* device, int flags), (override));
    MOCK_METHOD(ssize_t, readEvent, (int fd, void* buffer, size_t count), (override));
    MOCK_METHOD(int, closeDevice, (int fd), (override));
};

class LinuxListenerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockInputDevice> mockDevice;
    std::unique_ptr<LinuxListener> listener;

    void SetUp() override {
        mockDevice = std::make_unique<MockInputDevice>();
        listener = nullptr;
    }
};

TEST_F(LinuxListenerTest, InitializeSuccess) {
    EXPECT_CALL(*mockDevice, openDevice(_, _))
        .WillOnce(Return(1));

    listener = std::make_unique<LinuxListener>(std::move(mockDevice));

    int fd = listener->initialize();
    EXPECT_EQ(fd, 1);
}

TEST_F(LinuxListenerTest, ReactToSelectionReturnsExpectedString) {
    listener = std::make_unique<LinuxListener>(std::move(mockDevice));

    // Mock or simulate popen/wl-paste output, possibly with a pipe if necessary
    std::string selectedText = listener->reactToSelection();
    EXPECT_EQ(selectedText, "Expected Output\n");
}

TEST_F(LinuxListenerTest, ListenerDetectsShortcut) {
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

    listener = std::make_unique<LinuxListener>(std::move(mockDevice));

    EXPECT_TRUE(listener->listenForShortcut());
}
