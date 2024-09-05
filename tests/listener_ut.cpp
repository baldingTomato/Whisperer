#include <gtest/gtest.h>
#include "../src/linux_listener.hpp"

TEST(LinuxListenerTest, CaptureTextReturnsExpectedString) {
    LinuxListener listener;

    std::string selectedText = listener.reactToSelection();
    EXPECT_EQ(selectedText, "Expected Output");
}

TEST(LinuxListenerTest, ListenForShortcutWorksCorrectly) {
    LinuxListener listener;

    bool result = listener.listenForShortcut();
    EXPECT_TRUE(result);
}
