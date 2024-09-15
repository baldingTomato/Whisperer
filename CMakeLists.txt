cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FLAGS_COMMON -Wall)
set(FLAGS_LINUX -Wextra -Werror -Wconversion -pedantic)
set(FLAGS_WINDOWS /W4 /WX /we)

set(SOURCES src/linux_listener.cpp src/file_input_device.hpp)
set(TESTS tests/listener_ut.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(whisperer)
enable_testing()

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
add_executable(${PROJECT_NAME}-ut ${TESTS} ${SOURCES})

if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS_COMMON} ${FLAGS_LINUX})
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS_COMMON} ${FLAGS_WINDOWS})
endif()

target_link_libraries(${PROJECT_NAME}-ut gtest gmock_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)

