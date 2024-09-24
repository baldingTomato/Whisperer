cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FLAGS_COMMON -Wall)
set(FLAGS_LINUX -Wextra -Werror -Wconversion -pedantic)
set(FLAGS_WINDOWS /W4 /WX /we)

set(SOURCES src/linux_listener.cpp src/file_input_device.cpp src/wl_clipboard_reader.cpp src/translator.cpp src/whisperer.cpp src/lingva_api.cpp)
set(TESTS tests/listener_ut.cpp tests/translator_ut.cpp)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8 # Stable commit
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

project(whisperer)
enable_testing()

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
add_executable(${PROJECT_NAME}-ut ${TESTS} ${SOURCES})

if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS_COMMON} ${FLAGS_LINUX})
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS_COMMON} ${FLAGS_WINDOWS})
endif()

target_link_libraries(${PROJECT_NAME}-ut gtest gmock_main cpr::cpr nlohmann_json::nlohmann_json Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} cpr::cpr nlohmann_json::nlohmann_json Qt5::Widgets)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)

